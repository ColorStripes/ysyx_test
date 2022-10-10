#include <memory.h>
#include <proc.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void* heap_end = pf;
  pf += nr_page * PGSIZE;
  memset(heap_end, 0, nr_page * PGSIZE);
  return heap_end;
}

#ifdef HAS_VME

static void* pg_alloc(int n) {
  void * addr = new_page((n + PGSIZE - 1) / PGSIZE);
  memset(addr, 0, n);                                  //zero the page
  //printf("IN PGALLOC:%p\n",addr);
  return addr;
}

#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  if(current->max_brk >= brk) return 0;
  while(current->max_brk < brk){
    void *ppage = new_page(1);
    map(&current->as, (void *)current->max_brk, ppage, 0);
    current->max_brk += PGSIZE; 
  }
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
