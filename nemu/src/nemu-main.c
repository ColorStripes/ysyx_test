#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif


  uint32_t result;
  char exp[33]={};
  FILE *fp = fopen("input","r");
  if(fp == NULL)
  assert(fp);
  for(int i = 0; i < 1; i++){
  if(fscanf(fp,"%s %d",exp,&result));
  printf("%d,%s\n",result,exp);
  }
  fclose(fp);


  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
