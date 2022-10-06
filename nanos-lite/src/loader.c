#include <proc.h>
#include <elf.h>
#include <fs.h>
// extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
// extern size_t get_ramdisk_size();

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
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

Elf_Ehdr elf_header;
Elf_Phdr program_header;

static uintptr_t loader(PCB *pcb, const char *filename)
{

#if defined(__ISA_AM_NATIVE__)
#define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_X86__)
#define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_MIPS32__)
#define EXPECT_TYPE EM_MIPS
#elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
#define EXPECT_TYPE EM_RISCV
#else
#error unsupported ISA __ISA__
#endif

  int fd = fs_open(filename, 0, 0);
  // Load the elf_header
  //////////////////ramdisk_read(&elf_header, 0, sizeof(Elf_Ehdr));
  fs_read(fd, &elf_header, sizeof(Elf_Ehdr));
  // match the magic from ramdisk
  assert(*(uint32_t *)elf_header.e_ident == 0x464c457f);
  assert(elf_header.e_machine == EXPECT_TYPE);

  for (int i = 0; i < elf_header.e_phnum; i++)
  {
    // Load the program_header
    ///////////////////////ramdisk_read(&program_header, elf_header.e_phentsize * i + elf_header.e_phoff, sizeof(Elf_Phdr));
    fs_lseek(fd, elf_header.e_phentsize * i + elf_header.e_phoff, SEEK_SET);
    fs_read(fd, &program_header, sizeof(Elf_Phdr));
    // analysis the each of program_header
    if (program_header.p_type == PT_LOAD)
    {
      /////////////ramdisk_read((void *)program_header.p_vaddr, program_header.p_offset , program_header.p_memsz);
      fs_lseek(fd, program_header.p_offset, SEEK_SET);
      fs_read(fd, (void *)program_header.p_vaddr, program_header.p_memsz);
      memset((void *)(program_header.p_vaddr + program_header.p_filesz), 0, program_header.p_memsz - program_header.p_filesz);
    }
  }
  fs_close(fd);
  // printf("%lx\n",(uintptr_t)elf_header.e_entry);
  return (uintptr_t)elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename)
{
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %lx", entry);
  ((void (*)())entry)();
}

void context_kload(PCB *pcb, void (*entry)(void *), void *arg)
{
  printf("IN kload: %p\n", entry);

  pcb->as.area.start = (void *)pcb->stack; //
  pcb->as.area.end = pcb->as.area.start + STACK_SIZE;
  pcb->cp = kcontext(pcb->as.area, entry, arg);
}

// void context_uload(PCB *pcb, const char* filename){
//   printf("IN uload: load: %s\n", filename);

//   //1.open the kernel stack
//   pcb->as.area.start = (void *)pcb->stack;
//   pcb->as.area.end = pcb->as.area.start + STACK_SIZE;

//   //2.load the user program
//   uintptr_t entry = loader(pcb, filename);
//   pcb->cp = ucontext(&pcb->as, pcb->as.area, (void *)entry);

//   //3.set the stack top with heap.end
//   pcb->cp->GPRx = (uintptr_t)heap.end;

// }

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[])
{
  printf("IN uload: load: %s\n", filename);

  //// 1.open the kernel stack
  pcb->as.area.start = new_page(8);
  pcb->as.area.end = pcb->as.area.start + STACK_SIZE;


  //// 2. args loading
  uintptr_t argc = 0, envc = 0;
  while (argv && argv[argc])
    argc++;                                           // the number of argc
  while (envp && envp[envc])
    envc++;                                           // the number of envp

  void *current_sp = pcb->as.area.end - 1;            // user stack end
  //push the stack
  // a. Unspecified
  int Unspecified_1 = sizeof(uintptr_t);
  current_sp -= Unspecified_1;
  // b. string area
  uintptr_t arg_pointer[argc];
  for (int i = 0; i < argc; i++)
  {
    current_sp -= (strlen(argv[i]) + 1);           //+1 for '\0'
    arg_pointer[i] = (uintptr_t)current_sp;
    strcpy(current_sp, argv[i]);                   // strcpy is +sp, so first sp is current_sp - strlen
  }
  uintptr_t env_pointer[envc];
  for (int j = 0; j < envc; j++)
  {
    current_sp -= (strlen(envp[j]) + 1);
    env_pointer[j] = (uintptr_t)current_sp;
    strcpy(current_sp, envp[j]);                   // strcpy is +sp, so first sp is current_sp - strlen
  }
  // c. Unspecified
  int Unspecified_2 = sizeof(uintptr_t);
  current_sp -= Unspecified_2;
  // d. NULL
  current_sp -= 1;
  // e. pointer of the envp                                      //opposite order
  for (int j = envc - 1; j >= 0; j--)
  {
    current_sp -= sizeof(uintptr_t *);
    *(uintptr_t *)current_sp = env_pointer[j];
  }
  // f. NULL
  current_sp -= 1;
  // g. pointer of the argv                                      //opposite order
  for (int i = argc - 1; i >= 0; i--)
  {
    current_sp -= sizeof(uintptr_t *);
    *(uintptr_t *)current_sp = arg_pointer[i];
  }
  // h. argc
  current_sp -= sizeof(uintptr_t);
  *(uintptr_t *)current_sp = argc;


  //// 3.load the user program
  pcb->as.area.end = current_sp;                              //stack end can't cover the args' stack
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(&pcb->as, pcb->as.area, (void *)entry);


  //// 4.set the stack top with argc's pointer
  pcb->cp->GPRx = (uintptr_t)current_sp;

}
