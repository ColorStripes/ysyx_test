#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool eqreg = true; 
  for(int i = 0; i < 32; i++)
      if(ref_r->gpr[i] != gpr(i)){
      	  eqreg = false;
      	  printf("%s is different after executing instruction at PC = 0x%lx\n", reg_name(i, 4), pc);
      	  printf("Right %s = %ld,but now is %s = %ld\n", reg_name(i, 4), ref_r->gpr[i], reg_name(i, 4), gpr(i) );
      }
  
  if(ref_r->pc != cpu.pc){
      eqreg = false;
      printf("The next pc is different after executing instruction at PC = 0x%lx\n", pc);
  }
      
  	
  //printf("***%lx***%lx\n",ref_r->pc,cpu.pc);
  return eqreg;
}

void isa_difftest_attach() {
}
