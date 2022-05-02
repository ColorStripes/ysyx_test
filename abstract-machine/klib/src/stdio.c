#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void itoa(unsigned int n, char * buf)
{
        int i;
        
        if(n < 10)
        {
                buf[0] = n + '0';
                buf[1] = '\0';
                return;
        }
        itoa(n / 10, buf);

        for(i=0; buf[i]!='\0'; i++);
        
        buf[i] = (n % 10) + '0';
        
        buf[i+1] = '\0';
}


//conversions
void xtoa(unsigned int n, char * buf)
{
        int i;
        
        if(n < 16)
        {
                if(n < 10)
                {
                        buf[0] = n + '0';
                }
                else
                {
                        buf[0] = n - 10 + 'a';
                }
                buf[1] = '\0';
                return;
        }
        xtoa(n / 16, buf);
        
        for(i = 0; buf[i] != '\0'; i++);
        
        if((n % 16) < 10)
        {
                buf[i] = (n % 16) + '0';
        }
        else
        {
                buf[i] = (n % 16) - 10 + 'a';
        }
        buf[i + 1] = '\0';
}

void Xtoa(unsigned int n, char * buf)
{
        int i;
        
        if(n < 16)
        {
                if(n < 10)
                {
                        buf[0] = n + '0';
                }
                else
                {
                        buf[0] = n - 10 + 'A';
                }
                buf[1] = '\0';
                return;
        }
        xtoa(n / 16, buf);
        
        for(i = 0; buf[i] != '\0'; i++);
        
        if((n % 16) < 10)
        {
                buf[i] = (n % 16) + '0';
        }
        else
        {
                buf[i] = (n % 16) - 10 + 'A';
        }
        buf[i + 1] = '\0';
}

void otoa(unsigned int n, char * buf)
{
        int i;
        if(n < 8)
        {
        	buf[0] = n + '0';
                buf[1] = '\0';
                return;
        }
        xtoa(n / 8, buf);
        for(i = 0; buf[i] != '\0'; i++);
        buf[i] = (n % 8) + '0';
        buf[i + 1] = '\0';
}


int isnum(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  
  char *pout = out;
  int count = 0;
  char buf[65];
  char *s;
  int d;
  char c;
  int len;
  int num;
  int numindex = 0;
  char Num[16] = {'0'};
  memset(buf, 0, sizeof(buf));
  va_list ap;
  va_start(ap, fmt);
  while(*fmt){
    if(*fmt == '%'){
        fmt++;
  	switch(*fmt++){
  	    case 's':
  	        s = va_arg(ap, char *);
  	    	memcpy(out, s, strlen(s));
  	    	out += strlen(s);
  	    	break;
  	    case 'd':
  	        d = va_arg(ap, int);
  	        if(d < 0){
                    *out = '-';
                    out ++;
                    d = -d;
                }
                itoa(d, buf);
  	        memcpy(out, buf, strlen(buf));
  	        out += strlen(buf);
  	        break;
  	    case 'c':
  	        c = (char)va_arg(ap, int);
  	        *out = c;
  	        out ++;
  	        break;
  	    case 'x': /*0x*/
  	        d = va_arg(ap, int);
                xtoa(d, buf);
                memcpy(out, buf, strlen(buf));
                out += strlen(buf);
                break;
  	    case 'X':
                d = va_arg(ap, int);
                Xtoa(d, buf);
                memcpy(out, buf, strlen(buf));
                out += strlen(buf);
                break;
            case 'o':
                d = va_arg(ap, int);
                otoa(d, buf);
                memcpy(out, buf, strlen(buf));
                out += strlen(buf);
                break;
            case 'f':
                break;
            case '%': /*输出%*/
                    *out = '%';
                    out++;                    
                    break;
            case '0':
                    if(*fmt != '-'){
                        numindex = 0;
                        while(isnum(*fmt)){
                            Num[numindex++] = *fmt++;
                        }
                        Num[numindex++] = '\n';
                        num = atoi(Num);
                    }
                    else{
                        fmt++;
                        break;
                    }
                    switch(*fmt++){
                        case 'd':
                            d = va_arg(ap, int);
  	                    if(d < 0){
                                *out = '-';
                                out ++;
                                d = -d;
                            }
                            itoa(d, buf);
                            len = strlen(buf);
                            if(len >= num){
                               memcpy(out, buf, len);
                               out += strlen(buf);  
                            }
                            else{
                               memset(out, '0', num - len);
                               out += num - len;
                               memcpy(out, buf, len);
                               out += len;
                            }
                            break;
                         case 'o':
                            d = va_arg(ap, int);
                            otoa(d, buf);
                            len = strlen(buf);
                            if(len >= num){
                               memcpy(out, buf, len);
                               out += len;  
                            }
                            else{
                               memset(out, '0', num - len);
                               out += num - len;
                               memcpy(out, buf, len);
                               out += len;
                            }
                            break;
                         
                    }
                    break;
            case '-':
                    numindex = 0;
                    while(isnum(*fmt)){
                        Num[numindex++] = *fmt++;
                    }
                    Num[numindex++] = '\n';
                    num = atoi(Num);
                    break;
            case '#':
                    switch(*fmt++){
                        case 'o':
                             d = va_arg(ap, int);
                             otoa(d, buf);
                             if(*buf != '0'){
                                *out = '0';
                                out ++;
                             }
                             memcpy(out, buf, strlen(buf));
                             out += strlen(buf);
                             break;
                         case 'x':
                             *out++ = '0';
                             *out++ = 'x'; 
                             d = va_arg(ap, int);
                             xtoa(d, buf);
                             memcpy(out, buf, strlen(buf));
                             out += strlen(buf);
                             break;
                          case 'X':
                             *out++ = '0';
                             *out++ = 'X'; 
                             d = va_arg(ap, int);
                             Xtoa(d, buf);
                             memcpy(out, buf, strlen(buf));
                             out += strlen(buf);
                             break;
                             
                    }
  	    default:
  	        return -1;
  	        
  	}
    }
    else{
  	 *out++ = *fmt++;
    }  
  }
  *out = '\0';
  count = out - pout;	    	
  va_end(ap);
  return count;

}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
