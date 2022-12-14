typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

uint8_t *guest_to_host(paddr_t paddr);
uint64_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, uint64_t data);

extern uint8_t pmem[];

#define CONFIG_MSIZE 1000
#define CONFIG_PC_RESET_OFFSET 0
#define CONFIG_MBASE 0x80000000
#define RESET_VECTOR (CONFIG_MBASE + CONFIG_PC_RESET_OFFSET)


