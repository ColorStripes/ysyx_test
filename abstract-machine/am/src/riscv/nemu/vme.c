#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)
#define PGTABLE_MASK 0x3ffffffffffc00
#define VALID_MASK 1

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1); 
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12))); 
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }


  set_satp(kas.ptr);
  vme_enable = 1;


  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
  if(as->ptr == NULL){
    printf("as->ptr is NULL at va: 0x%lx, pa: 0x%lx, prot:%d\n",va, pa, prot);
    assert(as->ptr);
  }
    

  uint64_t virtual_add = (uint64_t)va;                   //39 bits virtual address
  uint32_t first_vindx = (virtual_add >> 30) & 0x1ff;    //virtual_add[39 : 30]
  uint64_t * first_vpage = (uint64_t*)as->ptr;           //the start virtual page content(64 bits)
  

  if(!(first_vpage[first_vindx] & 0x1)){                 //invalid in talbe item
    void * new_ppage = pgalloc_usr(PGSIZE);              //apply the new physical page
    first_vpage[first_vindx] =  (((uint64_t)new_ppage & 0xfffffffffff000) >> 2) | VALID_MASK;       //the [55 : 11] of physical page address + 10 bits flags | 1 bits valid
  }

  uint32_t second_vindx = (virtual_add >> 21) & 0x1ff;    //virtual_add[29 : 21]
  uint64_t * second_vpage = (uint64_t*)((first_vpage[first_vindx] & PGTABLE_MASK) << 2);            //the address of the second virtual page content(64 bits)
  
  if(!(second_vpage[second_vindx] & 0x1)){                 //invalid in talbe item
    void * new_ppage = pgalloc_usr(PGSIZE);                //apply the new physical page
    second_vpage[second_vindx] = (((uint64_t)new_ppage & 0xfffffffffff000) >> 2) | VALID_MASK; 
  }

  uint32_t third_vindx = (virtual_add >> 12) & 0x1ff;     //virtual_add[20 : 12]
  uint64_t * third_vpage = (uint64_t*)((second_vpage[second_vindx] << 2) & PGTABLE_MASK);           //the address of the second virtual page content(64 bits)
  
  if(!(third_vpage[third_vindx] & 0x1)){                  //invalid in talbe item
    //void * new_ppage = pgalloc_usr(PGSIZE);             //apply the new physical page
    third_vpage[third_vindx] = (((uint64_t)pa & 0xfffffffffff000) >> 2) | VALID_MASK;               //the address of the third virtual page is pa
  }

}


Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  printf("IN ucontext: %p\n", entry);
  
  Context * context_make = (Context *)(kstack.end - sizeof(Context));
  context_make->pdir = as->ptr; 
  context_make->mstatus = 0xa00001880;
  context_make->mepc = (uintptr_t)entry;

  return context_make;
}
