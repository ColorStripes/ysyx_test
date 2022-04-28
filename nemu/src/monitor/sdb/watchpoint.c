#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char Type[16];
  char What[32];
  uint64_t result;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i+1;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
  if(head == NULL){
     head = free_;
  }
  else{
     free_ = free_->next;
  }
  if(free_ == NULL)
     assert(0);
     
  return free_;
}


void free_wp(WP *wp){
 if(head == NULL){
   printf("There is no watchpoint");
   return;
 }
 else{
   WP *ispoint = head;          //is this point want to cut?
   WP *prpoint = NULL;          //the present point of which want to cut 
   while(ispoint != free_->next){
       if(wp == ispoint){
          prpoint->next = ispoint->next;       //delet
          
          ispoint->next = free_->next;          //insert
          free_->next = ispoint;
          
          break;
       }   
       else{
          prpoint = ispoint;
          ispoint = ispoint->next;
       }   
   }
 }

}


void print_wp(){
  if(head == NULL){
     printf("No watchpoints!\n");
     return;
  }
  else{
     WP *now = head;
     while(now != free_->next){
         printf("NUM                TYPE                        What              \n");
         printf("%-15d%-32s%-32s\n",now->NO,now->Type,now->What);
         now = now->next;
     }
  }
}

void delete_wp(int n){
    free_wp(&wp_pool[n-1]);
}

void set_wp(char *expre,bool *successed){
    bool success = true;
    expr(expre,&success);
    if(success){
        WP* NEW = new_wp();
        if(strncmp(expre,"$pc",3) != 0 && strcmp(expre,"$PC") != 0 && strcmp(expre,"$PC") != 0){
            strcpy(NEW->Type, "HW watchpoint");
            strcpy(NEW->What, expre);
            printf("Watchpoint %d: %s\n",NEW->NO,NEW->What);
        }
        else{
            strcpy(NEW->Type, "Breakpoint");
            strcpy(NEW->What, expre);
            printf("Breakpoint %d: %s\n",NEW->NO,NEW->What);
        }

    }
      
    successed = &success;  
   
}




static void print_s_wp(WP *wp,uint64_t new_result){
    printf("%-16s %-d: %-s\n",wp->Type,wp->NO,wp->What);
    printf("Old value = %-ld\nNew value = %-ld\n",wp->result,new_result);
    wp->result = new_result;
}

void scan_wp(bool *isdebug){
    if(head == NULL){
       *isdebug = false;
    }
    else{
       WP *now = head;
       while(now != free_->next){
           bool success=true;
           uint64_t expre = expr(now->What,&success);
           if(now->result == expre){
               continue;
           }
           else{
               
               print_s_wp(now,expre);
               *isdebug = true;
               printf("\n");
               //now->result = expre;
           }
           now = now->next;
       }
    }
}



