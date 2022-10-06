#include <common.h>
#include <sys/time.h>
#include <proc.h>
#include "syscall.h"
//if strace?
#define CONFIG_STRACE 0

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);
extern void naive_uload(PCB *pcb, const char *filename);
extern void switch_boot_pcb();
extern void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);



// size_t syscall_write(int fd, const void *buf, size_t count){
//   size_t n = 0;
//   for(int i = 0; (i < count && buf != NULL); i++)
//     if(((char *)buf)[i] != '\0') {
//       putch(((char *)buf)[i]);
//       n ++;
//     }
//   return n;
// }

int syscall_gettimeofday(struct timeval *tv, struct timezone *tz){
  long time_us = io_read(AM_TIMER_UPTIME).us;
  if(tv != NULL){
    tv->tv_usec = time_us % 1000000;
    tv->tv_sec  = time_us / 1000000;  
  }
  else{
    return -1;
  }
  if(tz != NULL){

  }
  return 0;
}


int syscall_execve(const char *pathname, char *const argv[], char *const envp[]){
  // naive_uload(NULL, pathname);
  // panic("Can not to next program!!!!\n");

  //check
  int open = fs_open(pathname, 0, 0);
  if(open == -2){
    return open;
  }

  context_uload(current, pathname, argv, envp);
  switch_boot_pcb();
  yield();
  return 0;
}




void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    //case SYS_exit:  halt(a[1]); break;
    case SYS_exit:  c->GPRx = syscall_execve("/bin/menu", NULL, NULL); break;
    case SYS_yield: c->GPRx = 0; yield();  break;
    case SYS_open:  c->GPRx = fs_open((char *)a[1], (int)a[2], (int)a[3]); break;
    case SYS_read:  c->GPRx = fs_read((int)a[1], (void *)a[2], (size_t)a[3]); break;
    case SYS_write: c->GPRx = fs_write((int)a[1], (const void *)a[2], (size_t)a[3]); break;
    case SYS_close: c->GPRx = fs_close((int)a[1]); break;
    case SYS_lseek: c->GPRx = fs_lseek((int)a[1], (size_t)a[2], (int)a[3]); break;
    
    case SYS_brk: c->GPRx = 0; break;
    case SYS_execve: c->GPRx = syscall_execve((const char *)a[1], (char *const*)a[2], (char *const*)a[3]); break;
    case SYS_gettimeofday: c->GPRx = syscall_gettimeofday((struct timeval *)a[1], (struct timezone *)a[2]); break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }

#if CONFIG_STRACE
if(c->GPR1 == SYS_open)
  //printf("****System Call:open, openfile:%s, fd:%d\n", c->GPR1, c->GPR2, c->GPRx);
printf("****System Call:%d, a0:%d, a1:%d, a2:%d, ret:%d****\n", c->GPR1, c->GPR2, c->GPR3, c->GPR4, c->GPRx);
#endif
  
}
