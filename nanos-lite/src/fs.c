#include <fs.h>
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;  //new
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENT, FD_DISPINFO, FD_FB, FD_SB, FD_SBCTL};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

#define FILE_NUM (sizeof(file_table)/sizeof(file_table[0]))
extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);
extern size_t fsb_write(const void *buf, size_t offset, size_t len);
extern size_t fsbctl_read(void *buf, size_t offset, size_t len);
extern size_t fsbctl_write(const void *buf, size_t offset, size_t len);

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]    = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT]   = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR]   = {"stderr", 0, 0, invalid_read, serial_write},
  [FD_EVENT]    = {"/dev/events", 0, 0, events_read, invalid_write},
  [FD_DISPINFO] = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  [FD_FB]       = {"/dev/fb", 0, 0, invalid_read, fb_write},
  [FD_SB]       = {"/dev/sb", 0, 0, invalid_read, fsb_write},
  [FD_SBCTL]    = {"/dev/sbctl", 0, 0, fsbctl_read, fsbctl_write},
  #include "files.h"  //Load files.h (3 + .h)
  
};

int fs_open(const char *pathname, int flags, int mode){
  //Log("Open the file <-----: %s",pathname);
  int i = 0;
  int file_num = FILE_NUM;
  for(; i < file_num; i ++){
    if(strcmp(file_table[i].name,pathname) == 0){
      file_table[i].open_offset = 0;
      break;
    }
    else
      if(i == file_num - 1){
        //panic("Can not open this file.\n");
        printf("Can not open this file: %s\n",pathname);
        return -2;
      }
        
        
  }
  return i;
}

size_t fs_read(int fd, void *buf, size_t len){  
  size_t size = file_table[fd].size;
  //whether or not had out of bound?
  if(file_table[fd].read == NULL){
      if(file_table[fd].open_offset >= size || len == 0)
        return 0;
      if(file_table[fd].open_offset + len >= size)
        len = size - file_table[fd].open_offset;
      if(fd != FD_STDIN && fd != FD_STDOUT && fd != FD_STDERR){
        ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
        file_table[fd].open_offset += len;
        return len;
      }
      else{
        return 0;
      }
  }
  else{
      return file_table[fd].read(buf, file_table[fd].open_offset, len); 
  }

}

size_t fs_write(int fd, const void *buf, size_t len){

  size_t size = file_table[fd].size;
  
  if(file_table[fd].write == NULL){
      //whether or not had out of bound?
      if(file_table[fd].open_offset >= size || len == 0)
        return 0;
      if(file_table[fd].open_offset + len >= size)
        len = size - file_table[fd].open_offset;
      ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
      file_table[fd].open_offset += len;
      return len;
  }
  else{
      return file_table[fd].write(buf, file_table[fd].open_offset, len);
  }  
  
}

size_t fs_lseek(int fd, size_t offset, int whence){
  size_t ret = -1;
  switch(whence){
    case SEEK_SET:
        if(offset >= 0 && offset <= file_table[fd].size){
          file_table[fd].open_offset = offset;
          ret = offset;
        }
        break;
    case SEEK_CUR:
        if(offset >= 0 && offset + file_table[fd].open_offset <= file_table[fd].size){
          file_table[fd].open_offset += offset;
          ret = file_table[fd].open_offset;
        }
        break;
    case SEEK_END:
        file_table[fd].open_offset = file_table[fd].size + offset;
        ret = file_table[fd].open_offset;
  }
  return ret;
}

int fs_close(int fd){
  file_table[fd].open_offset = 0;
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  AM_GPU_CONFIG_T gpu = io_read(AM_GPU_CONFIG);
  file_table[FD_FB].size = gpu.height * gpu.width * 4;
}
