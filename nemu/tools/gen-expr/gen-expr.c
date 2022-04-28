#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define EXP_LONG  30;

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";


int n = 0;
int q = EXP_LONG;

static uint32_t choose(uint32_t n){

	int i= rand() % n;
	//printf("q%dq",i);
	return i;
}
static void gen(char f){
	if(f == '(')
		q--;
	buf[n++] = f;
}

static void gen_rand_op(){
    if(q - n > 0){
      
	switch (choose(4)){
	  case 0: buf[n++] = '+'; break;
          case 1: buf[n++] = '-'; break;
 	  case 2: buf[n++] = '*'; break;
	  case 3: buf[n++] = '/'; break;
	  default: buf[n++] = ' '; break;
	}
    }
}

static void gen_num(){
//printf("ssb:%s\n",buf);
//printf("yn:%d\n",n);
	uint32_t num = (rand() % (1 << 8)) % (10*(q-n+1));
	//printf("*%d*\n",num);
	//printf("st:%ld\n",strlen(buf));
	char*now_buff = &buf[n]; 
	sprintf(now_buff, "%d", num);
	//printf("num:%s\n",now_buff);
	n += strlen(&buf[n]);
	//printf("n:%d\n",n);
}

static void gen_rand_expr( ) {
  //buf[0] = '\0'; 
  //printf("gens:%c\n",buf[1]);
  //printf("genb:%ld\n",strlen(buf));

    if(q - n == 1||q - n == 0){
    	    gen_num();
    }
    else if(q < n){
    }
    else{
        switch (choose(3)) {
        case 0: gen_num(); break;
        case 1: gen('('); gen_rand_expr(); gen(')'); break;
        default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
        }
	
    }
    if(q > n){ gen_rand_op(); gen_rand_expr();}

	    
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    n = 0;
    q = EXP_LONG;    
    gen_rand_expr();
    buf[n] = '\0';
   // printf("*%d %d*\n",n,q);
   // printf("buff:%s\n",buf);
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    if( fscanf(fp, "%d", &result));
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
