#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
static uint32_t freq,channels,samples,init;
static uint32_t sbuf_size = CONFIG_SB_SIZE;
static uint32_t count = 0;            //the number of data had write in buf             

static void audio_fill(void *userdata, uint8_t *stream, int len) {
  //printf("******%s********\n",sbuf[i]);
  //printf("countnow:%d\n",count);
  static int i = 0;
  SDL_memset(stream, 0, len);
  if(count == 0)
      return;
  //printf("****need:%d,have:%d\n",len,count);
  len = (len > count ? count : len);        // whether need read size is bigger than the size of data had in buf
  int len_t = len;
  //SDL_MixAudio(stream, &sbuf[i], len, SDL_MIX_MAXVOLUME);
  while(len_t--){
      //printf("sbNEMU:%d\n",sbuf[i]);
      memcpy(stream, &sbuf[i], 1);
      //SDL_MixAudio(stream, &sbuf[i], 1, SDL_MIX_MAXVOLUME);
      i = (i + 1) % audio_base[3];
      stream ++;
  }
  //printf("***i:%d\n",i);
  count -= len;    
  //printf("******.......********\n");
/*  
  int nread = len;
  if (count < len) nread = count;
  int b = 0;
  while (b < nread) {
    int n = read(rfd, stream, nread);
    if (n > 0) b += n;
  }

  count -= nread;
  if (len > nread) {
    memset(stream + nread, 0, len - nread);
  }
*/  
  
  
}


static void init_sound() {

  freq = audio_base[0];
  channels = audio_base[1];
  samples = audio_base[2];
  
  //printf("%d,,%d,,%d\n",freq,channels,samples);
  
  
  SDL_AudioSpec s = {};
  s.freq = freq;
  s.format = AUDIO_S16SYS;
  s.channels = channels;
  s.samples = samples;
  s.callback = audio_fill;
  s.userdata = NULL;
  int ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
  if (ret == 0) {
    SDL_OpenAudio(&s, NULL);
    SDL_PauseAudio(0);
  } 
}


static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  if (!is_write && offset == 20) {
      audio_base[5] = count;
      //printf("countrrrr:%d\n", count);
  }
  else if(is_write && offset == 20){
      count = audio_base[5];
      //printf("countwwwww:%d\n", count);
  }
  //else if(!is_write && offset == 16){
      //audio_base[4] = init;
  //}
  else if(is_write && offset == 16){
      init = audio_base[4];
      if(init){
          //printf("init\n");
          init_sound();
      }   
  }
  else{
      return;
  }	
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
  audio_base[3] = sbuf_size;
  audio_base[5] = count;
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
  //printf("%s\n",sbuf);
  //while(!init);
  //IFDEF(CONFIG_AUDIO_PLAY_SOUND, init_sound());
  //IFDEF(CONFIG_AUDIO_PLAY_SOUND, memset(sbuf, 0, CONFIG_SB_SIZE));
}
