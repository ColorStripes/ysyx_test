#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>

#define PGTABLE_MASK 0x3ffffffffffc00

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {

  uint64_t * root_physical_page =  (uint64_t *)((cpu.satp & 0xfffffffffff) << 12);      //satp's PPN(44 bits) << 12 = 56 physical address
  uint32_t root_pindx = (vaddr >> 30) & 0x1ff;    //virtual_add[39 : 30]
  word_t root_item = paddr_read((uintptr_t)(root_physical_page + root_pindx), sizeof(uintptr_t));

  //printf("*************%ld\n",root_physical_page[root_item]);
  if(!(root_item & 0x1)){
    printf("Root physical page is invalid at vaddr:0x%lx  pc:0x%lx\n", vaddr, cpu.pc);
    assert(0);
  }

  uint64_t * second_physical_page = (uint64_t *)((root_item & PGTABLE_MASK) << 2);
  uint32_t second_pindx = (vaddr >> 21) & 0x1ff;  //virtual_add[29 : 21]
  word_t second_item = paddr_read((uintptr_t)(second_physical_page + second_pindx), sizeof(uintptr_t));

  if(!(second_item & 0x1)){
    printf("Second physical page is invalid at vaddr:0x%lx  pc:0x%lx\n", vaddr, cpu.pc);
    assert(0);
  }

  uint64_t * third_physical_page = (uint64_t *)((second_item & PGTABLE_MASK) << 2);
  uint32_t third_pindx = (vaddr >> 12) & 0x1ff;  //virtual_add[20 : 12]
  word_t third_item = paddr_read((uintptr_t)(third_physical_page + third_pindx), sizeof(uintptr_t));

  if(!(third_item & 0x1)){
    printf("Third physical page is invalid at vaddr:0x%lx  pc:0x%lx\n", vaddr, cpu.pc);
    assert(0);
  }
  return (third_item & PGTABLE_MASK) << 2  | (vaddr & 0xfff);             //item[53 : 10] << 2 | vaddr[11 : 0]  (= 56 bits physical address)
}

int isa_mmu_check(vaddr_t vaddr, int len, int type){
  word_t satp = cpu.satp;
  satp >>= 60;                              //satp's MODE
  switch(satp){
    case 0:
      return MMU_DIRECT;
    case 8:
      return MMU_TRANSLATE;
    case 9:
      return MMU_TRANSLATE;
    default:
      assert(0);
      return MMU_FAIL;
  }
}
