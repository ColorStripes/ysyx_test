#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);

Context* __am_irq_handle(Context *c) {
  assert(c);
  //printf("c->np: 0x%lx\n",c->np);
  __am_get_cur_as(c);
  if (user_handler) {
    Event ev = {0};
    //printf("c->mcause:%lx\n",c->mcause);
    //printf("c->ptr:%p\n",c->pdir);
    switch (c->mcause) {
      case 11:
        c->mepc = c->mepc + 4; 
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
                  break;
      	}
        break;
      case 0x8000000000000007:
        ev.event = EVENT_IRQ_TIMER;
        break;

      default:
        break;

    }
    
    c = user_handler(ev, c);  //do_event
    assert(c != NULL);
  }
  __am_switch(c);
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
  context_make->mstatus = 0xa00001880;
  context_make->mepc = (uintptr_t)entry;
  context_make->GPR2 = (uintptr_t)arg;      //a0
  context_make->pdir = NULL;
  context_make->np = 0;

  //asm volatile("csrwi mscratch, 0");

  return context_make;
}

void yield() {
  asm volatile("li a7, -1; ecall");
  //printf("yield pc\n");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
