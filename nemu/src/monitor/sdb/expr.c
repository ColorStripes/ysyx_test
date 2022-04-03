#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUMBER = 9,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  {"\\(", '('},
  {"\\)", ')'},
  {"[0-9]+", TK_NUMBER},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
	  case 9:
		  tokens[nr_token].type = TK_NUMBER; 
		  strncpy(tokens[nr_token].str, substr_start, substr_len);
		  tokens[nr_token++].str[substr_len] = '\n';
		  break;
	  case '+':
		  tokens[nr_token ++].type = '+';
		  break;
	  case '-':
                  tokens[nr_token ++].type = '-';
	          
	          break;
	  case '*':
                  tokens[nr_token ++].type = '*';
	          
		  break;
	  case '/':
                  tokens[nr_token ++].type = '/';
	          
	          break;
	  case '(':
		  tokens[nr_token++].type = '(';
		  //strcpy(tokens[nr_token++].str, substr_start);
	          break;
	  case ')':
                  tokens[nr_token++].type = ')';
		  //strcpy(tokens[nr_token++].str, substr_start);
                  break;
	 
          default:
		  tokens[nr_token ++].type = 256;
		  break;

        }
        //printf("%d,%c\n",i,rules[i].token_type);
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  

  return true;
}



bool check_parentheses(int p , int q){

	char tmp[32]={};
	int cnt = 0;
	int NW_paren = 0;
	if(tokens[p].type != '{' && tokens[p].type != '[' && tokens[p].type != '(') {
		NW_paren = 1;
	}
	for (int i = p; i <= q; i++) {
		if (tokens[i].type == '{' || tokens[i].type == '[' || tokens[i].type == '(') {
			tmp[cnt++] = tokens[i].type;
			//printf("cnt :%c\n",tmp[cnt-1]);
		}
		else if (cnt != 0 && ((tokens[i].type == ')' && tmp[cnt - 1] == '(') ||
		                      (tokens[i].type == ']' && tmp[cnt - 1] == '[') || 
		                      (tokens[i].type == '}' && tmp[cnt - 1] == '{')))   {
		//printf("wc :%d\n",q);
		if((cnt == 1)&&(i != q))  
		        NW_paren = 1; 
		//printf("wc :%d ,%d, %d\n",i,q,cnt);
			
			cnt--;
			
		}
	}

	if (cnt != 0)
		assert("The expression is illegal.\n");

	if(NW_paren){
                //printf("flase:%d %d\n",p,q);
		return false;
		}
	else{
	        //printf("true:%d %d\n",p,q);
		return true;
		}
}



int Primary_op(int p , int q){
        int op = -1;
        for(int i = p; i <= q; i++){
                if(tokens[i].type == '{' || tokens[i].type == '[' || tokens[i].type == '('){
                        for(int j = q ;j > p; j--){
                                if(check_parentheses(i , j)){
                                        i = j;
                                        break;
                                }
                        }
                }
                else if(tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' || tokens[i].type == '/'){
                        if(tokens[i].type == '+' || tokens[i].type == '-')
                                op = i;
                        else{
                                if(op == -1)
                                        op = i;
				else if(tokens[op].type == '*' || tokens[op].type == '/')
					op = i;
                        }
                }
                else{
                        continue;
                }
        }
        if(op == -1)
                assert("no main option.\n");
        return op;
}


uint32_t eval(int p,int q) {
  if (p > q) {
    printf("p is %d > q is %d\n",p,q);
    //assert(0);
    return 0;    
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */

     if(tokens[p].type == TK_NUMBER) {
	     uint32_t n;
	     sscanf(tokens[p].str,"%u",&n);
	     return n;
     }
     else{
	     assert("(fuhao)\n");
	     return 0;
     }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
     //printf("true:%d %d\n",p,q);
    return eval(p + 1, q - 1);
  }
  else {
    /* We should do more things here. */
      int op = Primary_op(p,q);                  // the position of 主运算符 in the token expression;
      //printf("%d,%c,%d,%d\n",op,tokens[op].type,p,q);
      //assert(0);
      int val1 = eval(p, op - 1);
      int val2 = eval(op + 1, q);
      //unsigned int cc = 13;
      //unsigned int dd = -2;
      //uint32_t b= cc / dd;
      //printf("www:%u %u %u\n",cc,dd,b);
      //uint32_t b= val1 / val2;
      //printf("www:%u %u %u\n",val1,val2,b);
      //printf("val1:%d, op:%c, val2:%d   ops:%d\n",val1,tokens[op].type,val2,op);

    switch (tokens[op].type) {
      case '+': //printf("val1:%d, op:%c, val2:%d   ops:%d    res:%u   p:%d,  q:%d\n",val1,tokens[op].type,val2,op,val1+val2,p,q);
      return val1 + val2;
      case '-': //printf("val1:%d, op:%c, val2:%d   ops:%d    res:%u   p:%d,  q:%d\n",val1,tokens[op].type,val2,op,val1-val2,p,q);
      return val1 - val2;
      case '*': //printf("val1:%d, op:%c, val2:%d   ops:%d    res:%u   p:%d,  q:%d\n",val1,tokens[op].type,val2,op,val1*val2,p,q);
      return val1 * val2;
      case '/': if(val2 == 0){
			printf("####This is division by 0 expression.#####\n");
		        return 0;
		}
		else{
		        //printf("val1:%d, op:%c, val2:%d   ops:%d    res:%u  p:%d,  q:%d\n",val1,tokens[op].type,val2,op,val1/val2,p,q);
		        return val1 / val2;
		        }
      default: assert(0);
	       return 0;
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  //printf("9 is %s\n",tokens[9].str);
  //for(int i = 0; i < 32;i++)
  //printf("%d:%d\n",i,tokens[i].type);
  //printf("%ld\n",sizeof(tokens)/sizeof(tokens[0]));
  
  /* TODO: Insert codes to evaluate the expression. */
     //printf("long:%d\n",nr_token);
     
     
     //for(int i = 0; i < 32;i++)
     //printf("%d:%c\n",i,tokens[i].type);
     uint32_t a = eval(0,nr_token-1);
     //printf("a:%u\n",a);
     return a;

  return 0;
}
