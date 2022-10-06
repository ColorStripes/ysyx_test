#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;


void call_main(uintptr_t *args) {

  int argc = *args;
  char** argv = (char**)(args + 1);
  char** envp = (char**)(args + 1 + argc + 1);      //argv + the number of argv , then +1 is jump the NULL pointer the envp


  char *empty[] =  {NULL };
  environ = empty;
  //exit(main(0, empty, empty));
  exit(main(argc, argv, envp));
  assert(0);
}
