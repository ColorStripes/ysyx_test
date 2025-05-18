#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

// SDL_Surface* IMG_Load(const char *filename) {
//   printf("dddddddddddddddddddddddddddddddddddddddddddd\n");
//   int fd = open(filename, "r+");
//   int size = lseek(fd, 0, SEEK_END);
//   char *buf = (char *)malloc(size);
//   read(fd, buf, size);
//   SDL_Surface* ret = STBIMG_LoadFromMemory(buf, size);
//   close(fd);
//   free(buf);
//   return ret;
// }

SDL_Surface* IMG_Load(const char *filename) {

  FILE* fp = fopen(filename, "r");
  assert(fp);
  fseek(fp, 0, SEEK_END);
  int sz = ftell(fp);
  
  char* buf = malloc(sz+1);
  assert(buf);
  fseek(fp, 0, SEEK_SET);
  
  for(int i = 0; i < sz; i++) fscanf(fp, "%c", buf+i);
  
  SDL_Surface* s = STBIMG_LoadFromMemory(buf, sz);
  
  fclose(fp);
  free(buf);
  
  return s;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
