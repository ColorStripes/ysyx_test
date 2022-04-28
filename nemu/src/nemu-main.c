#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif


  uint32_t result;
  char exp[33];
  FILE *fp = fopen("/home/xu/ysyx-workbench/nemu/tools/gen-expr/build/input","r");
  if(fp == NULL)
  assert(fp);
  bool b = true;
  do{
    if(fscanf(fp,"%u %s",&result,exp));
    uint32_t q = expr(exp,&b);
    //printf("%u,%s,%u\n",result,exp,q);
    if(q == result)
       printf("yes!\n");
    else
       printf("no!\n");
  }while(fgetc(fp) != EOF);
  fclose(fp);


  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
