#define Vname V##top
#include "Vtop.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <getopt.h>
#include "difftest-def.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "svdpi.h"
#include "Vtop__Dpi.h"
#include "verilated_dpi.h"
typedef uint64_t paddr_t;

uint64_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, uint64_t data);

long read_inst(char *filename);
void difftest_step(paddr_t pc, paddr_t npc);
void init_difftest(char *ref_so_file, long img_size, int port);

extern "C" void init_disasm(const char *triple);
extern "C" void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

#define CONFIG_ITRACE 1

VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;
static Vname *top;
static vluint64_t main_time = 0;
static const vluint64_t sim_time = 1000;

using namespace std;
bool is_exit = false;

struct CPU_state
{
  uint64_t gpr[32];
  uint64_t pc;
}cpuu;


// DPI-C
bool isebreak = false;
void E(int a)
{
  if (a == 1){
    isebreak = true;
  }
  else
    isebreak = false;
}

uint64_t *cpu_gpr = NULL;
extern "C" void set_gpr_ptr(const svOpenArrayHandle r)
{
  cpu_gpr = (uint64_t *)(((VerilatedDpiOpenVar *)r)->datap());
  //cpuu.pc = top->pc;
}

FILE *fpw;
void Inst(int instruct)
{
  char log[128];
  char *p = log;
  p += snprintf(p, sizeof(log), "%lx: ", top->pc);
  if (instruct != 0 && !isebreak)
  {
    disassemble(p, log + sizeof(log) - p, top->pc, (uint8_t *)&instruct, 4);
    printf("log: %s\n", log);
    fputs(log, fpw);
    fputs("\n", fpw);
  }
}





// DPI-C
extern "C" void mem_read(long long raddr, long long *rdata)
{  //printf("read_addr:%lx,pc:%llx, ena:%d\n",raddr,top->pc, top->mem_ena);
  if(raddr<0x88000000 && raddr > 0x80000000 ){
  if(top->mem_ena && !top->mem_wr){
  // 总是读取地址为`raddr & ~0x7ull`的8字节返回给`rdata`
     // pmem_read(      *(uint64_t *)(raddr & ~0x7ull) ;//;
     //printf("rdata:%d\n",pmem_read((raddr & ~0x7ull), 8));
    // printf("read_addr:%lx,pc:%llx\n",raddr,top->pc);
    *rdata = pmem_read((raddr & ~0x7ull), 8) >> ((raddr & 0x7ull) * 8);
     //printf("rdata2:%d\n",*rdata);
  }
  }
}
extern "C" void mem_write(long long waddr, long long wdata, char wmask)
{
  // 总是往地址为`waddr & ~0x7ull`的8字节按写掩码`wmask`写入`wdata`
  // `wmask`中每比特表示`wdata`中1个字节的掩码,
  // 如`wmask = 0x3`代表只写入最低2个字节, 内存中的其它字节保持不变
  if(waddr<0x88000000 && waddr > 0x80000000 ){
  if(top->mem_ena && top->mem_wr){
    long long mask = 0;
    for(int i = 0; i < 8; i++){
      if((wmask >> i) & 0x01){
        //printf("&wmask:%d, i: %d\n",wmask >> i, i);
        long long f = 0xff; 
        f = f << (i * 8);
        mask |= f;
        //printf("...mask:%lld,  ff: %lld\n",mask, f);
      }
    }
    mask = ~mask;
    //printf("wmask:%d, mask:%lld\n",wmask, mask);

    long long wdata_z = wdata | (pmem_read((waddr & ~0x7ull), 8) & mask);

    //printf("w_data_z:%lld,w_data:%lld, mask:%lld\n",wdata_z,wdata, mask);

    pmem_write((waddr & ~0x7ull), 8, wdata_z);
  }
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// read .bin
static char *img_file = NULL;
static int parse_args(int argc, char *argv[])
{
  const struct option table[] = {};
  int o;
  while ((o = getopt_long(argc, argv, "-bhl:d:p:", table, NULL)) != -1)
  {
    switch (o)
    {
    case 1:
    {
      img_file = optarg;
      return 0;
    }
    default:
      exit(0);
    }
  }
  return 0;
}

void step_and_dump_wave()
{

  top->eval();
  contextp->timeInc(1);
  tfp->dump(main_time);
  if (main_time % 10 == 0)
  {
    top->eval();
    top->clock = 0, top->eval();
  }
  if (main_time % 10 == 5)
  {
    top->eval();
    top->clock = 1, top->eval();
  }
  cpuu.pc = top->pc;
  main_time++;
}

void sim_exit()
{

  step_and_dump_wave();
  tfp->close();
  fclose(fpw);
  delete top;
  delete contextp;
}

void exit_now(){
  is_exit = true;
}

void sim_init()
{
  contextp = new VerilatedContext;
  contextp->traceEverOn(true);
  top = new Vtop;
  tfp = new VerilatedVcdC;

  top->trace(tfp, 99);
  tfp->open("wave.vcd");
}

// CMD
void dump_gpr()
{
  printf("The all regs is: \n");
  for (int i = 0; i < 32; i++)
  {
    printf("%2s =  %-15ld ", reg_name(i), cpuu.gpr[i]);
    if ((i + 1) % 4 == 0)
    printf("\n");
  }
}
static int cmd_info(char *args)
{
  // printf("********%s,%d\n",args,strcmp(args, "r"));
  if (strcmp(args, "r") == 0)
  {
    //printf("********\n");
    dump_gpr();
  }

  // else if(strcmp(args, "w") == 0){
  // print_wp();
  //}
  else
    printf("Unknown parameter '%s'\n", args);
  // return -1;

  return 0;
}

static uint64_t refpc = 0;

static int cmd_si(char *args)
{
  uint64_t n;
  if (args == NULL)
    n = 10;
  else
  {
    sscanf(args, "%ld", &n);
    n *= 10;
  }

  while (n--)
  {

    if (main_time < 15)
    {
      top->reset = 1;
    }
    else
    {
      top->reset = 0;
      if (top->fetch_enb == 1)
      {
        top->instr = pmem_read(top->pc, 4);
        //printf("222pc:0x%lx, instr:0x%08lx\n", top->pc, pmem_read(top->pc, 4));
      }
    }  
    step_and_dump_wave();
    if (isebreak || is_exit)
    {
      if(isebreak)
        printf("____Isbreak____\n");
      break;
    }
    
  }

  printf("pc:0x%lx, instr:0x%08lx\n", top->pc, pmem_read(top->pc, 4));
  if(top->fetch_enb == 1){
    //printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");
    if(refpc == 0){
      refpc = top->pc;
    }
    else{
      for(int i = 0; i < 32; i++)
        cpuu.gpr[i] = cpu_gpr[i];
      difftest_step(refpc, top->pc);
      refpc = top->pc;
    }
      
     
  }
  if (main_time < 15)
    printf("Reset!\n");
  return 0;
}


static int cmd_c(char *args)
{
  while (!contextp->gotFinish()) //&& main_time < sim_time)
  {

    if (main_time < 15)
    {
      top->reset = 1;
    }
    else
    {
      top->reset = 0;
      top->eval();
      if (top->fetch_enb == 1)
      {
        top->instr = pmem_read(top->pc, 4);
        
      }

      if (main_time % 10 == 0){
        printf("pc:0x%lx, instr:0x%08lx\n", top->pc, pmem_read(top->pc, 4));
        if(top->fetch_enb == 1){
          if(refpc == 0){
            refpc = top->pc;
          }
          else{
            for(int i = 0; i < 32; i++)
              cpuu.gpr[i] = cpu_gpr[i];
            difftest_step(refpc, top->pc);
            refpc = top->pc;
          }
      
     
        }
      }
    }
    //printf("*******************\n");
    step_and_dump_wave();
    if (isebreak || is_exit)
    {
      if(isebreak)
        printf("____Isbreak____\n");
      break;
    }
  }

  return 0;
}

#define NR_CMD 3
static struct
{
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"c", "Continue the execution of the program", cmd_c},
    //{ "q", "Exit NEMU", cmd_q },
    {"si", "Single Step Execution", cmd_si},
    {"info", "info r: Print register status\n       info w: Print monitors information", cmd_info}
    /* TODO: Add more commands */

};
char str[10];
void sdb_mainloop()
{

  printf("Please input cmd:\n");
  char ch;
  int i = 0;
  while ((ch = getchar()) != '\n') //
  {
    str[i] = ch;
    i++;
  }
  str[i] = '\0';
  // scanf("%s",str);
  printf("ss:%s\n", str);
  while (str[0] != '\0')
  {
    // printf("********\n");
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    // if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end)
    {
      args = NULL;
    }

    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0)
      {
        if (cmd_table[i].handler(args) < 0)
        {
          return;
        } // Command Call
        break;
      }
    }
    if (i == NR_CMD)
    {
      printf("Unknown command '%s'\n", cmd);
    }

    for (int i = 0; i < 10; i++)
      str[i] = '\0';
  }
}

int main(int argc, char **argv)
{
  parse_args(argc, argv);
  //printf("log: %s\n", img_file);
  long img_size = read_inst(img_file);

#ifdef CONFIG_ITRACE
  init_disasm("riscv64-pc-linux-gnu");
  fpw = fopen("file.txt", "w+");
#endif
  
  char str[] = "/home/xu/ysyx-workbench/nemu/build/riscv64-nemu-interpreter-so";
  static char *diff_so_file = str;
  static int difftest_port = 1234;
  init_difftest(diff_so_file, img_size, difftest_port);
  
  //pmem_read((*(uint64_t *)0x80008fdc),8);
  Verilated::commandArgs(argc, argv);
  sim_init();

  while (1)
  {
    sdb_mainloop();
    if (isebreak || is_exit)
    {
      if(cpuu.gpr[10] != 0){
        assert(0);
      }
      break;
    }
  }
//printf("ssssss\n");
  sim_exit();
  return 0;
}
