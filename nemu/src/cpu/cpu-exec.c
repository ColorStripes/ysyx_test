#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/difftest.h>
#include <cpu/iringbuf.h>
#include <locale.h>


/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INST_TO_PRINT 10
//ftrace
#define MAX_FTRACE 1024
char ftbuff[MAX_FTRACE];

//itrace

#ifdef CONFIG_ITRACE

#define P_INS_NUM 300
#define SIZEOF_INS 64
#define MAX_RINGBUFF P_INS_NUM*SIZEOF_INS
char buff[MAX_RINGBUFF];
iringbuf rb;

#endif



CPU_state cpu = {};
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;

void device_update();
void scan_wp(bool *isdebug);
void sdb_mainloop();

static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_ITRACE_COND
  if (ITRACE_COND) { log_write("%s\n", _this->logbuf); }    
#endif
  if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
  
#ifdef CONFIG_WATCHPOINT
  bool isdebug=false;
  scan_wp(&isdebug);
  if(isdebug){
     nemu_state.state = NEMU_STOP;
     printf("Debug on there.\n");
     sdb_mainloop();
  }
#endif

}

static void exec_once(Decode *s, vaddr_t pc) {
  s->pc = pc;
  s->snpc = pc;
  isa_exec_once(s);
  cpu.pc = s->dnpc;
#ifdef CONFIG_ITRACE
  char *p = s->logbuf;
  p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
  int ilen = s->snpc - s->pc;
  int i;
  uint8_t *inst = (uint8_t *)&s->isa.inst.val;
  for (i = 0; i < ilen; i ++) {
    p += snprintf(p, 4, " %02x", inst[i]);
  }
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;

  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
      MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
     
  ringbuf_write(&rb, s->logbuf, SIZEOF_INS);
#endif

/*
  //TODO()
  //printf("**%s**\n",s->logbuf+18);
  if(!strcpy(s->logbuf + 32, "jal") || !strcpy(s->logbuf + 32, "jalr")){
  	char nbuff[20];
  	char cbuff[20];
  	memcpy(nbuff, s->logbuf, 19);
  	sprintf(cbuff,"0x%lx",s->dnpc);  	 
  }
  
*/ 
}

static void execute(uint64_t n) {
  Decode s;
  for (;n > 0; n --) {
    exec_once(&s, cpu.pc);
    g_nr_guest_inst ++;
    trace_and_difftest(&s, cpu.pc); 
    if (nemu_state.state != NEMU_RUNNING) break;
    IFDEF(CONFIG_DEVICE, device_update());
  }
}

static void statistic() {
  IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%ld", "%'ld")
  Log("host time spent = " NUMBERIC_FMT " us", g_timer);
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
  if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
  else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg() {
  isa_reg_display();
  statistic();
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  g_print_step = (n < MAX_INST_TO_PRINT);
  switch (nemu_state.state) {
    case NEMU_END: case NEMU_ABORT:
      printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
      return;
    default: nemu_state.state = NEMU_RUNNING;
  }

  uint64_t timer_start = get_time();
  
#ifdef CONFIG_ITRACE  
  ringbuf_init(&rb, buff, MAX_RINGBUFF);
#endif
  
  execute(n);
  uint64_t timer_end = get_time();
  g_timer += timer_end - timer_start;

  switch (nemu_state.state) {
    case NEMU_RUNNING: nemu_state.state = NEMU_STOP; break;

    case NEMU_END: case NEMU_ABORT:
      Log("nemu: %s at pc = " FMT_WORD,
          (nemu_state.state == NEMU_ABORT ? ASNI_FMT("ABORT", ASNI_FG_RED) :
           (nemu_state.halt_ret == 0 ? ASNI_FMT("HIT GOOD TRAP", ASNI_FG_GREEN) :
            ASNI_FMT("HIT BAD TRAP", ASNI_FG_RED))),
          nemu_state.halt_pc);
      //printf_iring();
      // fall through
    case NEMU_QUIT: statistic();
  }
}



#ifdef CONFIG_ITRACE


void ringbuf_init(iringbuf *rb, char *buf, uint32_t size){
	rb->buff = buf;
	rb->buff_size = size;

	rb->read_mirror = rb->read_pointer = 0;
	rb->write_mirror = rb->write_pointer = 0;
}


uint32_t Remain_size(iringbuf *rb){

	if(rb->read_mirror == rb->write_mirror)
		return rb->buff_size - (rb->write_pointer - rb->read_pointer);
	else
		return rb->read_pointer - rb->write_pointer;

}

uint32_t ringbuf_write(iringbuf *rb, char *str, uint32_t length){
	if(length > rb->buff_size){
		str = &str[length - rb->buff_size];
		length = rb->buff_size;
	}

	uint32_t remain_size = Remain_size(rb);


	if(length < rb->buff_size - rb->write_pointer){                       //have not arrive 0
		memcpy(&rb->buff[rb->write_pointer], str, length);
		rb->write_pointer += length;

		if(length > remain_size)
			rb->read_pointer = rb->write_pointer; //read_pointers always point the oldest one.
		
	}
	else{

		memcpy(&rb->buff[rb->write_pointer], str, rb->buff_size - rb->write_pointer);
                memcpy(&rb->buff[0], &str[rb->buff_size - rb->write_pointer], length - (rb->buff_size - rb->write_pointer));
        	rb->write_pointer = length - (rb->buff_size - rb->write_pointer);



		//in mirror
	        rb->write_mirror = ~rb->write_mirror;            //length is not enough,so in mirror
		if(length > remain_size){                        //wirte catch up the read
        	        rb->read_pointer = rb->write_pointer;     
               		rb->read_mirror = ~rb->read_mirror;      //to distinguish the read and write is not at the same mirror
                }


	}
	return length + rb->buff_size;
}

uint32_t ringbuf_read(iringbuf *rb, char *str, uint32_t length){

	
        if(length < rb->buff_size - rb->read_pointer){                       //have not arrive 0
                memcpy(str, &rb->buff[rb->read_pointer], length);
                rb->read_pointer += length;

        }
        else{

                memcpy(str, &rb->buff[rb->read_pointer], rb->buff_size - rb->read_pointer);
                memcpy(&str[rb->buff_size - rb->read_pointer], &rb->buff[0], length - (rb->buff_size - rb->read_pointer));
                rb->read_pointer = length - (rb->buff_size - rb->read_pointer);



                //in mirror
                rb->read_mirror = ~rb->read_mirror;            //length is not enough,so in mirror
                


        }
        return length;
}

void printf_iring()
{       
        if(*buff != '\0'){
        	char *now = buff;
       	 	int count = 0;
       		while(count != P_INS_NUM){
              	      if(now == rb.buff + rb.write_pointer - 1*SIZEOF_INS)
                		printf("      -----> %s\n",now);
              	      else
        			printf("             %s\n",now);
        	      now += SIZEOF_INS;
        	      count++;
       		}
        }
}

#endif
