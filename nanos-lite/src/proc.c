#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void (*entry)(void *), void *arg);
extern void context_uload(PCB *pcb, const char* filename);

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  //switch_boot_pcb();

  Log("Initializing processes...");

  // load program here

  //context_kload(&pcb[0], hello_fun, "one");
  context_uload(&pcb[0], "/bin/pal");
  context_uload(&pcb[1], "/bin/hello");

  switch_boot_pcb();


  //naive_uload(NULL, "/bin/pal");
  
}

Context* schedule(Context *prev) {
  printf("IN schedule\n");

  // save the context pointer
  current->cp = prev;
  // always select pcb[0] as the new process
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  // then return the new context
  return current->cp;

  //return NULL;
}
