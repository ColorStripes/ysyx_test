#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  //return paddr_read(addr, len);
  if(isa_mmu_check(addr, len, 0) == MMU_DIRECT){
    return paddr_read(addr, len);
  }
  else if(isa_mmu_check(addr, len, 0) == MMU_TRANSLATE){
    paddr_t paddr = isa_mmu_translate(addr, len, 0);
    return paddr_read(paddr, len);
  }
  else
    assert(0);
}

word_t vaddr_read(vaddr_t addr, int len) {
  
  if(isa_mmu_check(addr, len, 0) == MMU_DIRECT){
    return paddr_read(addr, len);
  }
  else if(isa_mmu_check(addr, len, 0) == MMU_TRANSLATE){
    paddr_t paddr = isa_mmu_translate(addr, len, 0);
    return paddr_read(paddr, len);
  }
  else
    assert(0);

}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  
  if(isa_mmu_check(addr, len, 0) == MMU_DIRECT){
    return paddr_write(addr, len, data);
  }
  else if(isa_mmu_check(addr, len, 0) == MMU_TRANSLATE){
    paddr_t paddr = isa_mmu_translate(addr, len, 0);
    return paddr_write(paddr, len, data);
  }
  else
    assert(0);

}
