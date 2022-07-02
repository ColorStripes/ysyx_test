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
#include "verilated_dpi.h"
typedef uint64_t paddr_t;
uint64_t pmem_read(paddr_t addr, int len);
void read_inst(char *filename);
extern "C" void init_disasm(const char *triple);
extern "C" void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

#define CONFIG_ITRACE 1

VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;
static Vname *top;
static vluint64_t main_time = 0;
static const vluint64_t sim_time = 1000;

using namespace std;

// DPI-C
bool isebreak = false;
void E(int a)
{
  if (a == 1){
    isebreak = false;
    printf("ddddddddddddddddddddd\n");
  }
  else
    isebreak = false;
}

uint64_t *cpu_gpr = NULL;
extern "C" void set_gpr_ptr(const svOpenArrayHandle r)
{
  cpu_gpr = (uint64_t *)(((VerilatedDpiOpenVar *)r)->datap());
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
    // printf("log: %s\n", log);
    fputs(log, fpw);
    fputs("\n", fpw);
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
    printf("gpr[%2d]=  %-15ld ", i, cpu_gpr[i]);
    if ((i + 1) % 4 == 0)
      printf("\n");
  }
}
static int cmd_info(char *args)
{
  // printf("********%s,%d\n",args,strcmp(args, "r"));
  if (strcmp(args, "r") == 0)
  {
    printf("********\n");
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
      }
    }
    step_and_dump_wave();

    if (isebreak)
    {
      printf("____Isbreak____\n");
      break;
    }
  }
  printf("pc:0x%lx, instr:0x%08lx\n", top->pc, pmem_read(top->pc, 4));
  if (main_time < 15)
    printf("Reset!\n");
  return 0;
}

static int cmd_c(char *args)
{
  while (!contextp->gotFinish() && main_time < sim_time)
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

      if (main_time % 10 == 0)
        printf("pc:0x%lx, instr:0x%08lx\n", top->pc, pmem_read(top->pc, 4));
    }
    step_and_dump_wave();
    if (isebreak)
    {
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
#ifdef CONFIG_ITRACE
  init_disasm("riscv64-pc-linux-gnu");
  fpw = fopen("file.txt", "w+");
#endif
  parse_args(argc, argv);
  read_inst(img_file);
  Verilated::commandArgs(argc, argv);
  sim_init();

  while (1)
  {
    sdb_mainloop();
    if (isebreak)
    {
      break;
    }
  }

  sim_exit();
  return 0;
}
