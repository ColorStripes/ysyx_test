#include <proc.h>
#include <elf.h>
#include <fs.h>
//extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
//extern size_t get_ramdisk_size();

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

// #if defined(__ISA_AM_NATIVE__)
// # define EXPECT_ENTRY ((void *)0x3000000)
// #elif defined(__ISA_X86__)
// # define EXPECT_ENTRY ((void *)0x3000000)
// #elif defined(__ISA_MIPS32__)
// # define EXPECT_ENTRY ((void *)0x3000000)
// #elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
// # define EXPECT_ENTRY ((void *)0x83000000)
// #else
// # error unsupported ISA __ISA__
// #endif

Elf_Ehdr elf_header ;
Elf_Phdr program_header;


static uintptr_t loader(PCB *pcb, const char *filename) {
  
#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_X86__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_MIPS32__)
# define EXPECT_TYPE EM_MIPS
#elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
# define EXPECT_TYPE EM_RISCV
#else
# error unsupported ISA __ISA__
#endif

  int fd = fs_open(filename, 0, 0);
  //Load the elf_header
  //////////////////ramdisk_read(&elf_header, 0, sizeof(Elf_Ehdr));
  fs_read(fd, &elf_header, sizeof(Elf_Ehdr));
  //match the magic from ramdisk
  assert(*(uint32_t *)elf_header.e_ident == 0x464c457f);
  assert(elf_header.e_machine == EXPECT_TYPE);
  
  for(int i = 0; i < elf_header.e_phnum; i++){
    //Load the program_header
    ///////////////////////ramdisk_read(&program_header, elf_header.e_phentsize * i + elf_header.e_phoff, sizeof(Elf_Phdr));
    fs_lseek(fd, elf_header.e_phentsize * i + elf_header.e_phoff, SEEK_SET);
    fs_read(fd, &program_header, sizeof(Elf_Phdr));
    //analysis the each of program_header
    if(program_header.p_type == PT_LOAD){
      /////////////ramdisk_read((void *)program_header.p_vaddr, program_header.p_offset , program_header.p_memsz);
      fs_lseek(fd, program_header.p_offset, SEEK_SET);
      fs_read(fd, (void *)program_header.p_vaddr, program_header.p_memsz);
      memset((void *)(program_header.p_vaddr + program_header.p_filesz), 0, program_header.p_memsz - program_header.p_filesz);
    }
  }
  fs_close(fd);
  //printf("%lx\n",(uintptr_t)elf_header.e_entry);
  return (uintptr_t)elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %lx", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void (*entry)(void *), void *arg){
  printf("IN kload: %p\n", entry);

  pcb->as.area.start = (void *)pcb->stack;              //
  pcb->as.area.end = pcb->as.area.start + STACK_SIZE;
  pcb->cp = kcontext(pcb->as.area, entry, arg);

  
}


void context_uload(PCB *pcb, const char* filename){
  printf("IN uload: load: %s\n", filename);

  //1.open the kernel stack
  pcb->as.area.start = (void *)pcb->stack;            
  pcb->as.area.end = pcb->as.area.start + STACK_SIZE;

  //2.load the user program
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(&pcb->as, pcb->as.area, (void *)entry);

  //3.set the stack top with heap.end
  pcb->cp->GPRx = (uintptr_t)heap.end; 


}
