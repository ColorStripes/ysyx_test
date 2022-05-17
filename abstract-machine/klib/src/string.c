#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //assert(NULL != s);
  size_t count = 0;
  const char *temp = s;
  while(*temp != '\0'){
  	count ++;
  	temp ++;
  }
  return count;
}

char *strcpy(char *dst, const char *src) {
  assert(NULL != dst);
  assert(NULL != src);
  if(dst == NULL || src == NULL){
      return NULL;
  }
  char *ret = dst;
  while((*dst++ = *src++) != '\0');
  return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
  assert(NULL != dst);
  assert(NULL != src);
  if(dst == NULL || src == NULL){
      return NULL;
  }
  if (n < 0) return NULL;
  char *ret = dst;
  while(n-- && (*dst++ = *src++));
  while(n--) *dst++ = '\0'; 
  return ret;
}

char *strcat(char *dst, const char *src) {
  assert(NULL != dst);
  assert(NULL != src);
  if(dst == NULL || src == NULL){
      return NULL;
  }
  char *ret = dst;
  while(*dst != '\0') dst++;
  while((*dst++ = *src++) != '\0') ;
  return ret;
}

int strcmp(const char *s1, const char *s2) {
  assert(NULL != s1);
  assert(NULL != s2);
  if(s1 == NULL || s2 == NULL) return 0;
  
  int ret = 0;
  while (!(ret = *s1 - *s2) && *s2)
  {
	s1++;
	s2++;
  }

  if (ret < 0)
	ret = -1;
  else if (ret > 0)
	ret = 1;
  return ret;
  
}

int strncmp(const char *s1, const char *s2, size_t n) {
  assert(NULL != s1);
  assert(NULL != s2);
  if(s1 == NULL || s2 == NULL) return 0;
  if (n < 0) return 0;
  
  int ret = 0;
  while(n-- && !(ret = *s1 - *s2) && *s2){
       s1++;
       s2++;
  }
  if (ret < 0)
	ret = -1;
  else if (ret > 0)
	ret = 1;
  return ret;
}

void *memset(void *s, int c, size_t n) {
  if (s == NULL || n < 0)
  {
	return NULL;
  }
  char *pdest = (char *)s;
  while (n-- > 0)
  {
	*pdest++ = c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  if (n == 0) return NULL;
  if (dst == NULL || src == NULL)  return NULL;
 
  char* dst_ = (char*)dst;
  char* src_ = (char*)src;
  if ((dst_ > src_) && (dst_ < src_ + n))//判断内存重叠时的情况反向拷贝（如果正向，会改变模板原来的值）
  {
	printf("发生重叠\n");
	while (n--)
	    *(dst_ + n) = *(src_ + n);
  }
  else //不重叠情况正向拷贝（如果反向，也会改变模板原来的值）
  {
	while(n--)
	    *dst_++ = *src_++;
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  if (n == 0) return NULL;
  if (out == NULL || in == NULL)  return NULL;
  char* out_ = (char*)out;
  char* in_ = (char*)in;
  while(n--) *out_++ = *in_++;
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
   assert(s1);
   assert(s2);
   if (s1 == NULL || s2 == NULL)  return 0;
   if (n == 0) return 0;
   int ret = 0;
   char* s1_ = (char*)s1;
   char* s2_ = (char*)s2;
   while(n-- && !(ret = *s1_ - *s2_)){
        s1_++;
        s2_++;
   }
   if (ret < 0)
	 ret = -1;
   else if (ret > 0)
	 ret = 1;
   return ret;
}

#endif
