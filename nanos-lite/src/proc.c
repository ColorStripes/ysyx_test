#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void (*entry)(void *), void *arg);
extern void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  printf("dddd\n");
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here

  //context_kload(&pcb[0], hello_fun, "one");
  //context_uload(&pcb[0], "/bin/hello", NULL, NULL);
  context_uload(&pcb[0], "/bin/hello", NULL, NULL);
  //context_uload(&pcb[1], "/bin/nterm", NULL, NULL);

  char* argv[] = {
    //"pal --skip",
     "--skip",
    // "hello",
    //"/bin/exec-test",
    NULL
  };

  context_uload(&pcb[3], "/bin/nterm", NULL, NULL);
  context_uload(&pcb[2], "/bin/nterm", NULL, NULL);
  context_uload(&pcb[1], "/bin/nterm", argv, NULL);
  context_uload(&pcb[0], "/bin/menu", NULL, NULL);

  // if(pcb[0].cp->np == 0){
  //   asm volatile("csrwi mscratch, 0");
  // }

  switch_boot_pcb();


  //naive_uload(NULL, "/bin/pal");
  
}


int fg_pcb = 0;
// int prev_pcb = 1;
// int hang_up_pcb = 2;
// int cnt = 0;
Context* schedule(Context *prev) {

  assert(prev);
  // static int i = 0;
  // i++;
  // current->cp = prev;
  // if(current == &pcb_boot) {
  //   printf("leave boot\n");
  //   current = &pcb[0];
  // }
  // if(i % 100 == 0) current = &pcb[3];
  // else{
  //     current = &pcb[fg_pcb];
  // }

  //printf("IN schedule\n");

  // if(current == &pcb[fg_pcb]){
  //   cnt += 1;
  // }
  // else{
  //   cnt = 0;
  // } 

  // save the context pointer
  //current->cp = prev;

  // if(cnt % 50 == 0){
  //   current = &pcb[prev_pcb];
  // }
  // else{
  //   current = &pcb[fg_pcb];
  // }

  // always select pcb[0] as the new process
  current = &pcb[fg_pcb];
  //current = (current == &pcb[0] ? &pcb[fg_pcb] : &pcb[0]);
  //current = (current == &pcb[0] ? &pcb[1] : &pcb[1] ? &pcb[2] : &pcb[3]);
  //current = &pcb[0];
  //printf("current:current->cp->np:%d, sp:%lx\n",current->cp->np, current->cp->gpr[2]);
  
  // then return the new context
  return current->cp;

  //return NULL;
}


void set_next_pcb(int id){

  // if(id != fg_pcb){
  //   if(id == hang_up_pcb){
  //     hang_up_pcb = prev_pcb;
  //   }
  //   prev_pcb = fg_pcb;
    fg_pcb = id;
  //}
  //printf("fg_pcb:%d, prev_pcb:%d, hang_up_pcb:%d\n",fg_pcb, prev_pcb, hang_up_pcb);
}