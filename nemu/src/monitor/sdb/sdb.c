#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/vaddr.h>
#include "sdb.h"

static int is_batch_mode = false;


void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);


static int cmd_si(char *args){
   uint64_t n;
   //printf("%s",args);
    if(args == NULL) 
       n = 1;
    else
       sscanf(args,"%ld",&n);
       //printf("%ld\n",n);
    cpu_exec(n);
    return 0;
}

static int cmd_info(char *args){
    if(strcmp(args, "r") == 0) 
        isa_reg_display();
    else if(strcmp(args, "w") == 0){
        print_wp();
    }
    else 
        printf("Unknown parameter '%s'\n", args);
           //return -1;
   
    return 0;
}

static int cmd_x(char *args){
   //for (char *str; args != NULL; )
    char *str_end_x = args + strlen(args);
    uint64_t addr;
    char *agum = strtok(args, " ");
    char *add = agum + strlen(agum) + 1;  
    
    int n;
    sscanf(agum,"%d",&n);         
    if (add >= str_end_x) {
      add = NULL;
      printf("You need to put Address(0x) to set the start of the search\n");
      return -1;
    }
    else {
      if(strncmp(add, "0x",2) == 0){
          sscanf(add+2,"%lx",&addr);
          while(n--){
              printf("0x%lx: %08lx\n",addr,vaddr_read(addr, 4));
              addr+=4;
          }
      }
      else{ 
          printf("Address must be 0x....\n");
          return -1;
      }
    }
    return 0;
}

static int cmd_p(char *args){
  bool success = true;
  uint64_t expre = expr(args,&success);
  if(success){
     printf("The '%s' result is %lu.\n",args,expre);
  }
  else{
     printf("The EXPR has error.\n");
  }
  return 0;
}


static int cmd_w(char *args){
     
     bool success = true;
     set_wp(args,&success);
     if(success){
       return 0;
     }
     else{
       printf("The EXPR have error.\n");
       return -1;
     }
     
     
}


static int cmd_d(char *args){
     int n;
     sscanf(args,"%d",&n);
     delete_wp(n);
     printf("The watchpoint-%-d was delete.\n",n);
     return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single Step Execution", cmd_si },
  { "info", "info r: Print register status\n       info w: Print monitors information", cmd_info },
  { "x", "x N EXPR: To scan memory", cmd_x },
  { "p", "x EXPR: To evaluation", cmd_p },
  { "w", "w EXPR: To set watchpoint", cmd_w },
  { "d", "d N: To delete watchpoint", cmd_d }
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;           
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }              //Command Call
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
