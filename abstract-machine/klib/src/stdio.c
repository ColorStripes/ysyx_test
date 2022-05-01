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
