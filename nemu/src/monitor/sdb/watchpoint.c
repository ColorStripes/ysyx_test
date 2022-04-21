#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
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




