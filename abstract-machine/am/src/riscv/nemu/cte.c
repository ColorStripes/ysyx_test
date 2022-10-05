#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    //printf("c->mcause:%lx\n",c->mcause);
    switch (c->mcause) {
      case 11: 
      	switch(c->GPR1){
      		case -1: ev.event = EVENT_YIELD; 
      		 	break;
      		default: 
              		if(c->GPR1 >= 0 || c->GPR1 <= 19){
                 		ev.event = EVENT_SYSCALL; break;
              		}
              		else{
                		ev.event = EVENT_ERROR; break;
              		}
      	}
      	c->mepc = c->mepc + 4;
      break;
    }

    c = user_handler(ev, c);  //do_event
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;
  //printf("user:%p\n",user_handler);

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  printf("IN kcontext: %p\n", entry);
  Context * context_make = (Context *)(kstack.end - sizeof(Context));      
  context_make->mstatus = 0xa00001800;
  context_make->mepc = (uintptr_t)entry;
  context_make->GPR2 = (uintptr_t)arg;      //a0
  return context_make;
}

void yield() {
  //printf("ddddddddddddddddd\n");
  asm volatile("li a7, -1; ecall");
  //printf("yield pc\n");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
