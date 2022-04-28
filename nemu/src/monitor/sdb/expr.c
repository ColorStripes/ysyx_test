#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/vaddr.h>


enum {
  TK_NOTYPE = 256, TK_EQ = 255, TK_NQ = 254, TK_NUMBER = 253, TK_HEXNUM = 252, TK_REG = 251, TK_ASS = 250, DEREF = 249, TK_AND = 248, NEGAT = 247, TK_PC = 246,

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
  {"!=", TK_NQ},
  {"&&", TK_AND},
  {"::=", TK_ASS},
  {"-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  {"\\(", '('},
  {"\\)", ')'},
  {"\\b0[xX][0-9a-fA-F]+\\b",TK_HEXNUM},
  {"[0-9]+", TK_NUMBER},
  {"\\$[$arsgt0][ap]*[0-9]*\\b",TK_REG},
  {"\\$[Pp][Cc]",TK_PC},
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
	  case TK_NUMBER:
		  tokens[nr_token].type = TK_NUMBER; 
		  strncpy(tokens[nr_token].str, substr_start, substr_len);
		  tokens[nr_token++].str[substr_len] = '\0';
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
	  case TK_EQ:
                  tokens[nr_token ++].type = TK_EQ;
	          break;
	  case TK_NQ:
                  tokens[nr_token ++].type = TK_NQ;
	          break;
	  case TK_AND:
                  tokens[nr_token ++].type = TK_AND;
	          break;
	  case TK_HEXNUM:
                  tokens[nr_token].type = TK_HEXNUM;
                  strncpy(tokens[nr_token].str, substr_start, substr_len);
		  tokens[nr_token++].str[substr_len] = '\0';
	          break;
	  case TK_REG:
                  tokens[nr_token].type = TK_REG;
	          strncpy(tokens[nr_token].str, substr_start, substr_len);
		  tokens[nr_token++].str[substr_len] = '\0';
	          break;
	  case TK_ASS:
                  tokens[nr_token ++].type = TK_ASS;
	          break;
	  case TK_PC:
	          tokens[nr_token ++].type = TK_PC;
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
		printf("The expression is illegal.\n");
		//assert(0);

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
                else if(tokens[i].type == TK_EQ || tokens[i].type == TK_NQ || tokens[i].type == TK_AND || tokens[i].type == DEREF || tokens[i].type == NEGAT){
                        //printf("*%d,%d,%d\n",i,op,tokens[op].type);
                        if(tokens[i].type == DEREF || tokens[i].type == NEGAT){
                                 op = i;
                                 break;
                        }
                        else if((tokens[i].type == TK_EQ || tokens[i].type == TK_NQ) && (op == -1 || tokens[op].type != TK_AND))
                        {
                                 op = i;
                        }
                        else if(tokens[i].type == TK_AND ){
                                 op = i;
                        }
                        
                }
                else if((tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' || tokens[i].type == '/')&&
                        (op == -1 || tokens[op].type == '+' || tokens[op].type == '-' || tokens[op].type == '*' || tokens[op].type == '/'  ))
                {
                        if(tokens[i].type == '+' || tokens[i].type == '-'){
                                op = i;
                                //printf("**%d,%d,%d\n",i,op,tokens[op].type);
                        }
                        else{//printf("%d,%d,%d\n",i,op,tokens[op].type);
                                if(op == -1)
                                        op = i;
				else if(tokens[op].type == '*' || tokens[op].type == '/')
					op = i;
			        //printf("***%d,%d,%d\n",i,op,tokens[op].type);
                        }
                        
                }
                else{
                        continue;
                }

        }

        return op;
}


uint64_t eval(int p,int q,bool *success) {
  if (p > q) {
    printf("p is %d > q is %d\n",p,q);
    assert(0);
    //return 0;    
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */

     if(tokens[p].type == TK_NUMBER) {
	     uint64_t n;
	     sscanf(tokens[p].str,"%lu",&n);
	     return n;
     }
     else if(tokens[p].type == TK_HEXNUM){
             uint64_t n;
             sscanf(tokens[p].str+2,"%lu",&n);
             return n;
     }
     else if(tokens[p].type == TK_REG){
             //bool succe=true;
             uint64_t reg = isa_reg_str2val(tokens[p].str+1,success);
             if(*success)
                return reg;
             else
                assert(0);
     }
     else if(tokens[p].type == TK_PC){
             return cpu.pc;
     }
     else{
	     printf("(fuhao)\n");
	     assert(0);
	     return 0;
     }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
     //printf("true:%d %d\n",p,q);
    return eval(p + 1, q - 1,success);
  }
  else {
    /* We should do more things here. */
      int op = Primary_op(p,q);                  // the position of 主运算符 in the token expression;
      //printf("%d,%c,%d,%d\n",op,tokens[op].type,p,q);
      //assert(0);
      long int val1 = 0;
      if(tokens[op].type != DEREF && tokens[op].type != NEGAT){
               val1 = eval(p, op - 1,success);
      }
      long int val2 = eval(op + 1, q,success);
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
      case TK_EQ:
                 if(val1 == val2) return 1;
                 else return 0;
      case TK_NQ:
                 if(val1 != val2) return 1;
                 else return 0;
      case TK_AND:
                 if(val1 && val2) return 1;
                 else return 0;
      case NEGAT:
                 return -val2;
      case DEREF:
                 return vaddr_read(val2, 4);
      default: assert(0);
	       return 0;
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    printf("***** ***\n");
    return 0;
  }
  //printf("9 is %s\n",tokens[9].str);
  //for(int i = 0; i < 32;i++)
  //printf("%d:%d\n",i,tokens[i].type);
  //printf("%ld\n",sizeof(tokens)/sizeof(tokens[0]));
  
  /* TODO: Insert codes to evaluate the expression. */
     //printf("long:%d\n",nr_token);
     
  //printf("*****%d ***\n",*success);  
  for (int i = 0; i < nr_token; i ++) {
  	if (tokens[i].type == '*' && 
  	   (i == 0 || tokens[i - 1].type == '(' || 
  	   (tokens[i - 1].type != ')' && tokens[i - 1].type != TK_NUMBER && tokens[i - 1].type != TK_HEXNUM && tokens[i - 1].type != TK_REG)))
        {
   	      tokens[i].type = DEREF;
  	}
  	else if (tokens[i].type == '-' && 
  	   (i == 0 || tokens[i - 1].type == '(' || 
  	   (tokens[i - 1].type != ')' && tokens[i - 1].type != TK_NUMBER && tokens[i - 1].type != TK_HEXNUM && tokens[i - 1].type != TK_REG)))
  	{
   	      tokens[i].type = NEGAT;
  	}
  }

  return eval(0,nr_token-1,success);



     //for(int i = 0; i < 32;i++)
     //printf("%d:%c\n",i,tokens[i].type);
     //uint64_t a = eval(0,nr_token-1);
     //printf("a:%lu\n",a);
     //return a;

}
