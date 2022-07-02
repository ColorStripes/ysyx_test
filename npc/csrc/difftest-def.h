#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <getopt.h>


typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

struct CPU_state
{
  uint64_t gpr[32];
  uint64_t pc;
};
CPU_state cpu;


enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };


#define DIFFTEST_REG_SIZE (sizeof(uint64_t) * 33) // GRPs + pc


