#include <isa.h>
#define IRQ_TIMER 0x8000000000000007  // for riscv64


word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */   


  cpu.mepc = epc;
  cpu.mcause = NO;

  //printf("in mstatus: 0x%lx, pc: 0x%lx\n",cpu.mstatus, cpu.pc);


  int MIE = (cpu.mstatus & 0x8ll) >> 3;
  cpu.mstatus &= ~0x8ll;
  if (MIE)
  {
    cpu.mstatus |= MIE << 7;
  }
  else
  {
    cpu.mstatus &= ~0x80ll;
  }

  //printf("out mstatus: %lx\n",cpu.mstatus);
  

#ifdef CONFIG_ETRACE
  char *log = NULL;  
  log = "Environment call from M-mode\n";   
  printf("exception: %s\n",log);
#endif

  return cpu.mtvec;
}

// word_t isa_query_intr() {
//   return INTR_EMPTY;
// }



word_t isa_query_intr() {
  if (cpu.INTR && (cpu.mstatus & 0x8ll)) {
    cpu.INTR = false;
    return IRQ_TIMER;
  }
  return INTR_EMPTY;
}
