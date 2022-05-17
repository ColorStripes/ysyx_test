#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)


void __am_gpu_init() {
/*
  int i;
  int w = 400;  // TODO: get the correct width
  int h = 300;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
*/
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t w_h = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w_h >> 16, .height = w_h & 0xffff,
    .vmemsz = cfg->width * cfg->height
  };
  //printf("wid:%d,,hei:%d\n",cfg->width,cfg->height);
}




void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  //printf("%d\n",ctl->sync);
  if(ctl->sync){
      outl(SYNC_ADDR, 1);
  } 


  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t * pixels = (uint32_t *)ctl->pixels;
      
      
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *fb_t;  
  for(int j = y; j < y + h; j++){
      fb_t = fb + x + io_read(AM_GPU_CONFIG).width * j;
      for(int i = 0; i < w; i++){
           fb_t[i] = *pixels++;
      }
  }     
 
 
/*
  if (ctl->sync) {
  //panic("++++++++++\n");
    uint32_t coordinate = ctl->x << 16 | ctl->y;
    outl(SYNC_ADDR, coordinate);
    printf("%d,,%d,,%d,,%d\n",ctl->x,ctl->y,ctl->h,ctl->w);
    //panic("++++++++++\n");
    uint32_t rect = ctl->w << 16 | ctl->h;
    outl(SYNC_ADDR+4, rect);
  }
 printf("%d,,%d,,%d,,%d,,%lx\n",ctl->x,ctl->y,ctl->h,ctl->w,ctl->pixels);
  //FB_ADDR
  //int x, y; void *pixels; int w, h; bool sync
  */
  
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
