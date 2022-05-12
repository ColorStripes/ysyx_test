#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int isnum(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}



void itoa(unsigned long long d, char * buf)
{
        int i;
        
        if(d < 10)
        {
                buf[0] = d + '0';
                buf[1] = '\0';
                return;
        }
        itoa(d / 10, buf);

        for(i=0; buf[i]!='\0'; i++);
        
        buf[i] = (d % 10) + '0';
        
        buf[i+1] = '\0';
}


void ftoa(long double f, char * buf, int max_p){
        
        
        if(max_p == 0){
            int only_i = ((int)(f + 0.5));
            itoa(only_i, buf);
        }
        else{
            	int it = (int) f;
		itoa(it, buf);
	 
		buf = buf + strlen(buf);
	
		*buf++ = '.';
	        
		double ff = f;
		int n = 0;
		while(n < max_p-1) {
	   	   ff *= 10;
	   	   itoa( ((int)ff) % 10, buf);
	    	   buf ++;
	  	   n++;
		}
		int ru = ((int)(ff * 10 + 0.5)) % 10;    //   .45ru
		itoa(ru, buf);
        
        }	
}


void etoa(long double f, char * buf, int max_p){
    int point = 0;
    if(f >= 1){
    	while(f >= 10){
    		point ++;
    		f /= 10;
   	}
   	 ftoa(f, buf, max_p);
   	 buf = buf + strlen(buf);
  	  *buf = 'e';
  	  *(buf+1) ='+';
  	  
    }    
    else{
    	 while(f < 1){
    	 	point ++;
    	 	f *= 10;
    	 }
    
    	ftoa(f, buf, max_p);
    	buf = buf + strlen(buf);
    	*buf = 'e';
    	*(buf+1) ='-';
    }
    
    
    if(point < 100 && point > 10){
    	*(buf+2) = '0';
    	itoa(point, buf+3);
    }
    else if(point < 10){
    	*(buf+2) = '0'; 
    	*(buf+3) = '0';
    	itoa(point, buf+4); 
    }
    else{
    	itoa(point, buf+2);
    }
}

void Etoa(long double f, char * buf, int max_p){
    int point = 0;
    if(f >= 1){
    	while(f >= 10){
    		point ++;
    		f /= 10;
   	}
   	 ftoa(f, buf, max_p);
   	 buf = buf + strlen(buf);
  	  *buf = 'E';
  	  *(buf+1) ='+';
  	  
    }    
    else{
    	 while(f < 1){
    	 	point ++;
    	 	f *= 10;
    	 }
    
    	ftoa(f, buf, max_p);
    	buf = buf + strlen(buf);
    	*buf = 'E';
    	*(buf+1) ='-';
    }
    
    
    if(point < 100 && point > 10){
    	*(buf+2) = '0';
    	itoa(point, buf+3);
    }
    else if(point < 10){
    	*(buf+2) = '0'; 
    	*(buf+3) = '0';
    	itoa(point, buf+4); 
    }
    else{
    	itoa(point, buf+2);
    }
}

//conversions
void xtoa(unsigned long long x, char * buf)
{
        int i;
        
        if(x < 16)
        {
                if(x < 10)
                {
                        buf[0] = x + '0';
                }
                else
                {
                        buf[0] = x - 10 + 'a';
                }
                buf[1] = '\0';
                return;
        }
        xtoa(x / 16, buf);
        
        for(i = 0; buf[i] != '\0'; i++);
        
        if((x % 16) < 10)
        {
                buf[i] = (x % 16) + '0';
        }
        else
        {
                buf[i] = (x % 16) - 10 + 'a';
        }
        buf[i + 1] = '\0';
}

void Xtoa(unsigned long long x, char * buf)
{
        int i;
        
        if(x < 16)
        {
                if(x < 10)
                {
                        buf[0] = x + '0';
                }
                else
                {
                        buf[0] = x - 10 + 'A';
                }
                buf[1] = '\0';
                return;
        }
        xtoa(x / 16, buf);
        
        for(i = 0; buf[i] != '\0'; i++);
        
        if((x % 16) < 10)
        {
                buf[i] = (x % 16) + '0';
        }
        else
        {
                buf[i] = (x % 16) - 10 + 'A';
        }
        buf[i + 1] = '\0';
}

void otoa(unsigned long long o, char * buf)
{
        int i;
        if(o < 8)
        {
        	buf[0] = o + '0';
                buf[1] = '\0';
                return;
        }
        xtoa(o / 8, buf);
        for(i = 0; buf[i] != '\0'; i++);
        buf[i] = (o % 8) + '0';
        buf[i + 1] = '\0';
}

void ptoa(void *p, char * buf){
        buf[0] = '0';
        buf[1] = 'x';
	size_t n = (size_t)p;
	xtoa(n, buf + 2);
}


void gtoa(long double f, char * buff, char * buf, int max_p){

	
	double t = f;
	int decimal_digits = 0;
	int precision = max_p;
	while(decimal_digits < 100){
	
	       	if((int)t == t){
		     precision = decimal_digits; 
		     break;
		}
		t *= 10;
		decimal_digits ++;
		
	}
	
	
	int radix_digits = 0;
	int negative_digits = 0;
	if(f >= 1){
	        int tt = (int)f;
		while(tt != 0){
		        radix_digits ++;
			tt /= 10;	
		}
	
	}
	else{
	        double tt = f;
		while(tt < 1){
			negative_digits ++;
			tt *= 10;
		}
	}
	
	
        if( (radix_digits-1 >= max_p) || (negative_digits > 4) ){
                if(f >= 1){
                    if(precision >= max_p-1)
        	   	etoa(f, buf, max_p-1);
        	    else
        	   	etoa(f, buf, precision);
        	}    
        	else{
        	    if( (precision - (negative_digits-1)) >= max_p)
        	         etoa(f, buf, max_p-1);
        	    else
        	         etoa(f, buf, (precision - (negative_digits-1)) );
        	}          
        }       	
        else{
                if(f >= 1){
                    if((max_p - radix_digits) >= precision)
        	         ftoa(f, buf, precision);
        	    else
        	         ftoa(f, buf, (max_p - radix_digits));  
        	}    
        	else{
        	    if((negative_digits-1 + max_p) >= precision)
        	         ftoa(f, buf, precision);
        	    else
        	         ftoa(f, buf, (negative_digits-1 + max_p));
        	}
        	
/*        	
        	int i;
       		for(i = 0; buf[i] != '.'; i++);
            
                int now = strlen(buf)-i-1;   //now pre
                if(precision == 0){
                	memcpy(buff, buf, i);
                	memcpy(buff+i, buf+i+1, 1);
                	int pr = (atoi(buff) + 5) / 10;
                	itoa(pr, buff);
                	
                }
                else if(now <= precision){
                        memcpy(buff, buf, strlen(buf));
                         
                        char *nbuff = buff + strlen(buf);
                        for(int i = 0; i < (precision - now); i++)
                        	*nbuff++ = '0';
                        *nbuff = '\0';		
  	        }
  	        else{
  	        	strcpy(buff, buf+i+1);
  	        	int pr = (atoi(buff) + 5) / 10;
  	        	memcpy(buff, buf, i+1);      //1.
  	        	itoa(pr, buff+i+1);          //.1
  	                
  	        } 
  	        
  	        while( *(buff+strlen(buff)-1) == '0' ) 
  	              *(buff + strlen(buff) -1 ) = '\0';
*/  	        	
        }
        memcpy(buff, buf, strlen(buf));     
}



int printf(const char *fmt, ...) {
  char pbuf[1024];
  memset(pbuf, '\0', sizeof(pbuf));
  
  int i = 0;
  va_list ap;
  va_start(ap, fmt);
  int ret = vsprintf(pbuf, fmt, ap);
  va_end(ap);
  
  while( i <= strlen(pbuf) ){
     putch(pbuf[i]);
     i++; 
  }
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *pout = out;
  int count = 0;
    
  char buff[128];
  char buf[64];
  
 
  char * s;
  void *p;
  char c;
  long long d;
  unsigned long long u;
  long double f;
  
  char flag;
  int width;  
  int numindex = 0;
  char Num[16] = {'\0'};
  
  int precision = -1;
  int length = -1;
  
  
  while(*fmt){
  
  
    if(*fmt == '%'){
        memset(buff, 0, sizeof(buf));
        memset(buf, 0, sizeof(buf));
        fmt++;
        
        //flag
        flag = '+';
        while(*fmt == '-' || *fmt == '+' || *fmt == ' ' || *fmt == '#' || *fmt == '0'){
            switch(*fmt++){
                case '-':
                         flag = '-';
                         break;
                case '+':
                         flag = '+';
                         break;
                case ' ':
                         flag = ' ';
                         break;
                case '#':
                         flag = '#';
                         break;
                case '0':
                         flag = '0';
                         break;
             }
        }
        
        //width
        width = -1;
        numindex = 0;
        while(isnum(*fmt)){
                Num[numindex++] = *fmt++;
        }
        Num[numindex++] = '\n';
        width = atoi(Num);
        
        //precision
        precision = -1;
        if(*fmt == '.'){
        	fmt++;
        	numindex = 0;
                while(isnum(*fmt)){
                	Num[numindex++] = *fmt++;
                }
        	Num[numindex++] = '\n';
        	precision = atoi(Num);
        }
   
        //length
        length = -1;
        bool isdoub  = false;
        while(*fmt == 'h' || *fmt == 'l' || *fmt == 'q' || *fmt == 'L' || *fmt == 'j' || *fmt == 'z' || *fmt == 'Z' || *fmt == 't'){
        	if(isdoub){
           	   switch(*fmt++){
           	           case 'h':
           	                     length = 2;
           	                     break;
           	           case 'l':
           	                     length = 3;
           	                     break;
           	   }
           	}
           	else{
           	    switch(*fmt++){
           	           case 'h':
           	                     length = 0;
           	                     break;
           	           case 'l':
           	                     length = 1;
           	                     break;
           	           case 'q':
           	                     length = 3;
           	                     break;
           	           case 'L':
           	                     length = 4;
           	                     break;
           	           case 'j':
           	                     length = 5;
           	                     break;
           	           case 'z':
           	                     length = 6;
           	                     break;
           	           case 'Z':
           	                     length = 6;
           	                     break;
           	           case 't':
           	                     length = 7;
           	                     break;
           	      }
           	      isdoub = true;
           	}
        }
        
        
        
        //conversions
        char *nbuff = buff;
  	switch(*fmt++){ 
  	    case 's':
  	        switch(length){
  	                //case 1:
  	                        //s = va_arg(ap, wchar_t *);
  	        	        //break;
  	                default:
  	                        s = va_arg(ap, char *);
  	        	        break;
  	        }
                if(precision == -1)
  	    	    memcpy(buff, s, strlen(s)+1);
  	    	else
  	    	    memcpy(buff, s, precision);
  	    	break;
  	    case 'c':
  	        switch(length){	
  	                //case 1:
  	                        //c = va_arg(ap, wint_t);
  	        	        //break;
  	                default:
  	                        c = (char)va_arg(ap, int);
  	        	        break;
  	        }
  	        *buff = c;
  	        *(buff+1) = '\0';
  	        break;
  	    case '%': /*输出%*/
                *buff = '%';
                *(buff+1) = '\0';                    
                break;
            case 'p':
                p = va_arg(ap, void *);
                ptoa(p, buf);
                break;
            case 'n':
            	switch(length){
  	        	case 0:{
  	        	        short* ip = (short*)va_arg(ap, int *);
  	        	        *ip = (out - pout);
  	        	        break;
  	        	}
  	                case 1:{
  	                        long* ip = va_arg(ap, long*);
  	                        *ip = (out - pout);
  	        	        break;
  	        	}
  	                case 2:{ 
  	                        char* ip = (char*)va_arg(ap, int*);
  	                        *ip = (out - pout);
  	                        break;
  	                }
  	                case 3:{
  	                        long long* ip = va_arg(ap, long long*);
  	                        *ip = (out - pout);
  	                        break;
  	                }
  	                case 5:{ 
  	                        intmax_t * ip = va_arg(ap, intmax_t *);
  	                        *ip = (out - pout);
  	                        break;
  	                }
  	                case 6:{
  	                        //size_t * ip = va_arg(ap, size_t *);
  	                        //*ip = (out - pout);
  	                        //break;
  	                }
  	                case 7:
  	                case 8:{
  	                        ptrdiff_t * ip = va_arg(ap, ptrdiff_t *);
  	                        *ip = (out - pout);
  	                        break;
  	                }
  	                default:{
  	                        int * ip = va_arg(ap, int *);
  	                        *ip = (out - pout);
  	        	        break;
  	        	}
  	        } 
  	          
  	        break;
  	
  	
  	
  	    case 'i':     
  	    case 'd':
  	        switch(length){
  	        	case 0:
  	        	        d = (short)va_arg(ap, int);
  	        	        break;
  	                case 1:
  	                        d = va_arg(ap, long);
  	        	        break;
  	                case 2: 
  	                        d = (char)va_arg(ap, int);
  	                        break;
  	                case 3:
  	                        d = va_arg(ap, long long);
  	                        break;
  	                case 5: 
  	                        d = va_arg(ap, intmax_t);
  	                        break;
  	                //case 6:
  	                        //d = va_arg(ap, ssize_t);
  	                        //break;
  	                case 8:
  	                        d = va_arg(ap, ptrdiff_t);
  	                        break;
  	                default:
  	                        d = va_arg(ap, int);
  	        	        break;
  	        }   
  	        if(d < 0){
  	            
                    *nbuff++ = '-';
                    *nbuff = '\0';
                    d = -d;
                }
                itoa(d, buf);
  	        if(strlen(buf) < precision){
  	                int num_zero =  precision - strlen(buf);
  	        	for(int i = 0; i < num_zero; i++)
  	        		*nbuff++ = '0';
  	                *nbuff = '\0';
  	        }	
  	        strcat(buff, buf);
  	        break;  
  	    case 'u':
  	    	switch(length){
  	        	case 0:
  	        	        u = (unsigned short)va_arg(ap, unsigned int);
  	        	        break;
  	                case 1:
  	                        u = va_arg(ap, unsigned long);
  	        	        break;
  	                case 2: 
  	                        u = (unsigned char)va_arg(ap, unsigned int);
  	                        break;
  	                case 3:
  	                        u = va_arg(ap, unsigned long long);
  	                        break;
  	                case 5: 
  	                        u = va_arg(ap, uintmax_t);
  	                        break;
  	                case 6:
  	                        u = va_arg(ap, size_t);
  	                        break;
  	                case 8:
  	                        u = va_arg(ap, ptrdiff_t);
  	                        break;
  	                default:
  	                        u = va_arg(ap, unsigned int);
  	        	        break;
  	        }
  	    	itoa(u, buf);
  	        if(strlen(buf) < precision){
  	                int num_zero =  precision - strlen(buf);
  	        	for(int i = 0; i < num_zero; i++)
  	        		*nbuff++ = '0';
  	                *nbuff = '\0';
  	        }	
  	        strcat(buff, buf);
  	        break; 
  	    case 'o':
  	        switch(length){
  	        	case 0:
  	        	        u = (unsigned short)va_arg(ap, unsigned int);
  	        	        break;
  	                case 1:
  	                        u = va_arg(ap, unsigned long);
  	        	        break;
  	                case 2: 
  	                        u = (unsigned char)va_arg(ap, unsigned int);
  	                        break;
  	                case 3:
  	                        u = va_arg(ap, unsigned long long);
  	                        break;
  	                case 5: 
  	                        u = va_arg(ap, uintmax_t);
  	                        break;
  	                case 6:
  	                        u = va_arg(ap, size_t);
  	                        break;
  	                case 8:
  	                        u = va_arg(ap, ptrdiff_t);
  	                        break;
  	                default:
  	                        u = va_arg(ap, unsigned int);
  	        	        break;
  	        }
  	        
  	        otoa(u, buf);
  	        if(flag == '#'){
  	        	*nbuff++ = '0';
  	        }

                
                if(strlen(buf) < precision){
  	                int num_zero =  precision - strlen(buf);
  	        	for(int i = 0; i < num_zero; i++)
  	        		*nbuff++ = '0';
  	                *nbuff = '\0';
  	        }	
  	        strcat(buff, buf);
                break;
  	    case 'x': /*0x*/
  	        switch(length){
  	        	case 0:
  	        	        u = (unsigned short)va_arg(ap, unsigned int);
  	        	        break;
  	                case 1:
  	                        u = va_arg(ap, unsigned long);
  	        	        break;
  	                case 2: 
  	                        u = (unsigned char)va_arg(ap, unsigned int);
  	                        break;
  	                case 3:
  	                        u = va_arg(ap, unsigned long long);
  	                        break;
  	                case 5: 
  	                        u = va_arg(ap, uintmax_t);
  	                        break;
  	                case 6:
  	                        u = va_arg(ap, size_t);
  	                        break;
  	                case 8:
  	                        u = va_arg(ap, ptrdiff_t);
  	                        break;
  	                default:
  	                        u = va_arg(ap, unsigned int);
  	        	        break;
  	        }
                xtoa(u, buf);
                if(flag == '#'){
  	        	*nbuff++ = '0';
  	        	*nbuff++ = '#';
  	        }
                if(strlen(buf) < precision){
  	                int num_zero =  precision - strlen(buf);
  	        	for(int i = 0; i < num_zero; i++)
  	        		*nbuff++ = '0';
  	                *nbuff = '\0';
  	        }	
  	        strcat(buff, buf);
                break;
  	    case 'X':
                switch(length){
  	        	case 0:
  	        	        u = (unsigned short)va_arg(ap, unsigned int);
  	        	        break;
  	                case 1:
  	                        u = va_arg(ap, unsigned long);
  	        	        break;
  	                case 2: 
  	                        u = (unsigned char)va_arg(ap, unsigned int);
  	                        break;
  	                case 3:
  	                        u = va_arg(ap, unsigned long long);
  	                        break;
  	                case 5: 
  	                        u = va_arg(ap, uintmax_t);
  	                        break;
  	                case 6:
  	                        u = va_arg(ap, size_t);
  	                        break;
  	                case 8:
  	                        u = va_arg(ap, ptrdiff_t);
  	                        break;
  	                default:
  	                        u = va_arg(ap, unsigned int);
  	        	        break;
  	        }
                Xtoa(u, buf);
                if(flag == '#'){
  	        	*nbuff++ = '0';
  	        	*nbuff++ = '#';
  	        }
                if(strlen(buf) < precision){
  	                int num_zero =  precision - strlen(buf);
  	        	for(int i = 0; i < num_zero; i++)
  	        		*nbuff++ = '0';
  	                *nbuff = '\0';
  	        }	
  	        strcat(buff, buf);
                break;    
  	        
  	        
  	    case 'e':
  	        switch(length){
  	                case 4:
  	                        f = va_arg(ap, long double);
  	        	        break;
  	                default:
  	                        f = va_arg(ap, double);
  	        	        break;
  	        }
  	        
                if(precision == -1) etoa(f, buf, 6);
  	        else etoa(f, buf, precision);
		memcpy(buff, buf, strlen(buf)+1);	
                break;
            case 'E':
                switch(length){
  	                case 4:
  	                        f = va_arg(ap, long double);
  	        	        break;
  	                default:
  	                        f = va_arg(ap, double);
  	        	        break;
  	        }
  	        
  	        if(precision == -1) etoa(f, buf, 6);
  	        else etoa(f, buf, precision);
		memcpy(buff, buf, strlen(buf)+1);

/*  	        
                if(precision == -1) Etoa(f, buf, 6);
  	        else Etoa(f, buf, precision);
  	        
  	        
                for(i = 0; buf[i] != '+' ; i++);
                
                if(precision == 0){
                	memcpy(buff, buf, 1);
                	memcpy(buff+1, buf+2, 1);
                	int pr = (atoi(buff) + 5) / 10;
                	itoa(pr, buff);
                	memcpy(buff+1, buf+i, strlen(buf+i));
                }
                else if((i-2) <= precision){
                        memcpy(buff, buf, i);
                        memset(buff+i, '\0', 1); 
                        nbuff = buff + strlen(buff);
                        for(int i = 0; i < (precision - (i-2)); i++)
                        	*nbuff++ = '0';
                        *nbuff = '\0';	
                        nbuff = buf + i;
                        strcat(buff, nbuff);	
  	        }
  	        else{
  	        	memcpy(buff, buf+2, 1+precision);
  	        	int pr = (atoi(buff) + 5) / 10;
  	        	memcpy(buff, buf, 2);      //1.
  	        	itoa(pr, buff+2);          //.1
  	        	memcpy(buff+2+precision, buf + i, strlen(buf+i)); //+     
  	        }  */
                break;    
            case 'f':
                switch(length){
  	                case 4:
  	                        f = va_arg(ap, long double);
  	        	        break;
  	                default:
  	                        f = va_arg(ap, double);
  	        	        break;
  	        }
  	        
  	        
  	        if(precision == -1) ftoa(f, buf, 6);
  	        else ftoa(f, buf, precision);
                
                
                
                memcpy(buff, buf, strlen(buf)+1);
  	       // panic(buff);
                break;
            case 'g':
                switch(length){
  	                case 4:
  	                        f = va_arg(ap, long double);
  	        	        break;
  	                default:
  	                        f = va_arg(ap, double);
  	        	        break;
  	        }
  	        
  	        int precisions = precision;
  	        if(precision == -1) precisions = 6;
  	        if(precision == 0) precisions = 1;
  	        
  	        if(flag == '#'){
  	            uint64_t t = 1;
	            int tt = 0;
	            while(tt < precision){
		          t *= 10;
		          tt ++;
	            }
	
                    if((f >= t) || (long int)f * 1e4 != f * 1e4){
                    	etoa(f, buf, precision); 
                    }
                    else{
                    	ftoa(f, buf, precision);
                    }
                }
                else{	
                    gtoa(f, buff, buf, precisions);
                }
                
                
                break;  
  	    default:
  	        return -1;        
        }
        
        
        
        int input_width = width - strlen(buff);
        if(input_width > 0){
        	switch(flag){
        	       case '-':
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             while(input_width--) *out++ = ' ';
                             break;
                       case ' ':
                       case '+':
                             while(input_width--) *out++ = ' ';
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             break;
                       case '0':
                             while(input_width--) *out++ = '0';
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             break;
                       default:
                       	     while(input_width--) *out++ = ' ';
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             break;
                }
        }
        else{
        	switch(flag){
        	       case ' ':
        	             *out++ = ' ';
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             break;
                       case '+':
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             break;
                       default:
                             memcpy(out, buff, strlen(buff)+1);
                             out = out + strlen(buff);
                             break;
                 }
        }
        
    }   
    else{
  	 *out++ = *fmt++;
  	 *out = '\0';
    }    
  
  
  
  }
  count = out - pout;
  return count;
}



int sprintf(char *out, const char *fmt, ...) {

    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  char sbuf[1024];
  memset(sbuf, '\0', sizeof(sbuf));
  
  va_list ap;
  va_start(ap, fmt);
  int ret = vsnprintf(out, n, fmt, ap);
  va_end(ap);
  
  return ret;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  char sbuf[1024];
  memset(sbuf, '\0', sizeof(sbuf));
  
  int vsp = vsprintf(sbuf, fmt, ap);
  if(vsp < n){
        memcpy(out, sbuf, vsp+1);  
  }
  else{
  	memcpy(out, sbuf, n-1);
  	*(sbuf+n-1) = '\0';
  }
  
  return (vsp + 1);
}

#endif
