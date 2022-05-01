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


int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  
  int count = 0;
  char buf[65];
  char *s;
  int d;
  char c;
  memset(buf, 0, sizeof(buf));
  va_list ap;
  va_start(ap, fmt);
  while(*fmt){
    //if(*fmt == '%'){
        //fmt++;
  	switch(*fmt){
  	    case 's':
  	        s = va_arg(ap, char *);
  	    	memcpy(out, s, strlen(s));
  	    	out += strlen(s);
  	    	count += strlen(s);
  	    	break;
  	    case 'd':
  	        d = va_arg(ap, int);
  	        if(d < 0){
  	            *out = '-';
  	            out ++;
  	            d = -d;
  	            count++;
  	        }
  	        itoa(d, buf);
  	        memcpy(out, buf, strlen(buf));
  	        out += strlen(buf);
  	        count += strlen(buf);
  	        break;
  	    case 'c':
  	        c = (char)va_arg(ap, int);
  	        *out = c;
  	        out ++;
  	        count++;
  	        break;
  	}
    //}
    //else{
  	 //*out++ = *fmt++;
  	 //count++;
    //}  
  }	    	
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
