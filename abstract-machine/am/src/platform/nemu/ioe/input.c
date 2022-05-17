#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t Keycode = inl(KBD_ADDR);
  if(Keycode){ 
      if(Keycode & KEYDOWN_MASK){
     	 kbd->keydown = 1;
     	 kbd->keycode = Keycode - KEYDOWN_MASK;    
      }
      else{
         kbd->keydown = 0;
         kbd->keycode = Keycode; 
      }
  }
  else{
       kbd->keydown = 0;
       kbd->keycode = AM_KEY_NONE;
  }
  
}
