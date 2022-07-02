  #define Vname V##top
  #include "Vtop.h"
  
  #include "verilated.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include <fstream>
  #include <assert.h>
  #include <getopt.h>
  #include "verilated_vcd_c.h"
  #include "svdpi.h"
  #include "Vtop__Dpi.h"

#define CONFIG_MSIZE 1000
#define CONFIG_MBASE 0x80000000
typedef uint64_t paddr_t;

#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] = {};
#endif


  VerilatedContext* contextp = NULL;
  VerilatedVcdC* tfp = NULL;
  static Vname* top;
  static vluint64_t main_time = 0;


bool isebreak = false;
void E(int a){
	if(a == 1)
		isebreak = true;
	else
		isebreak = false;
}

uint8_t* guest_to_host(paddr_t  paddr) { return pmem + paddr - CONFIG_MBASE; }

static inline paddr_t host_read(void *addr, int len) {
	//printf("addr1:%p\n",addr);
  switch (len) {
    case 1: return *(uint8_t  *)addr;
    case 2: return *(uint16_t *)addr;
    case 4: return *(uint32_t *)addr;
    case 8: return *(uint64_t *)addr;
    default: {
		assert(0); 
		return 0;
	  }
  }
}

static inline void host_write(void *addr, int len, uint64_t data) {
  switch (len) {
    case 1: *(uint8_t  *)addr = data; return;
    case 2: *(uint16_t *)addr = data; return;
    case 4: *(uint32_t *)addr = data; return;
    case 8: *(uint64_t *)addr = data; return;
    default: assert(0);
  }
}

static uint64_t pmem_read(paddr_t  addr, int len) {
  //printf("addr:%lx\n",addr);
  uint64_t ret = host_read(guest_to_host(addr), len);
  return ret;
}

static void pmem_write(paddr_t addr, int len, uint64_t data) {
  host_write(guest_to_host(addr), len, data);
}

void cpu_sim(){
    printf("pc:%lx,instr:%lx\n",top->pc,top->instr);
        if(main_time % 10 == 0){top->clock = 0,top->eval();}
        else if(main_time %10 == 5){top->clock = 1,top->eval();}
    top->instr = pmem_read(top->pc,8);
}


static char *img_file = NULL;
static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:", table, NULL)) != -1) {
    switch (o) {
	  case 1: {img_file = optarg; return 0;}
      default:
        exit(0);
    }
  }
  return 0;
}

void read_inst( char* filename)
{
  FILE *fp = fopen(filename, "rb");
  if( fp == NULL ) {
		printf( "Can not open this file!\n" );
		exit(1);
  }
  
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  size = fread(pmem, size, 1, fp);
  fclose(fp);
}



int main(int argc, char **argv){
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    top = new Vtop{contextp};
       parse_args(argc, argv);
       read_inst(img_file);

    Verilated::traceEverOn(true);

    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("waveform.vcd");

    while(1){  

    if( main_time < 15 )
    {
  	  top->reset = 1;
    }
    else
    {
      top->reset = 0;
    }    

    tfp->dump(main_time++);
    cpu_sim();

    }
    
    tfp->close();
    delete top;
    delete contextp;
    return 0;
}