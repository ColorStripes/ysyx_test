#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <cpu/iringbuf.h>
#include <isa.h>

/*
#if CONFIG_MTRAC
#define MTRAC_NUM 28*1000
    char buf[MTRAC_NUM];
    char *b = buf;
    uint32_t n = 1;
    void mtrac(char *buf, uint64_t add, char *o){
        if(n%1000 == 0){
        	*b = buf;
        }
    	sprintf(buf, "%-7d %-6s: 0x%lx\n",n, o, add);
    	n++;
    	b += strlen(buf) + 1;
    }
#endif
*/


#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
#endif

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static word_t pmem_read(paddr_t addr, int len) {
  word_t ret = host_read(guest_to_host(addr), len);
#if CONFIG_MTRAC
  printf("[NEMU] Read:  paddr 0x%x, len %d, data 0x%lx\n",addr, len, ret);
#endif
  return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
#if CONFIG_MTRAC
  printf("[NEMU] Write: paddr 0x%x, len %d, data 0x%lx  ",addr, len, data);
#endif
  host_write(guest_to_host(addr), len, data);
#if CONFIG_MTRAC
  printf("finish!\n");
#endif
}

static void out_of_bound(paddr_t addr) {
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR ") at pc = " FMT_WORD,
      addr, CONFIG_MBASE, CONFIG_MBASE + CONFIG_MSIZE, cpu.pc);
}

void init_mem() {
#if   defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
#ifdef CONFIG_MEM_RANDOM
  uint32_t *p = (uint32_t *)pmem;
  int i;
  for (i = 0; i < (int) (CONFIG_MSIZE / sizeof(p[0])); i ++) {
    p[i] = rand();
  }
#endif
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]",
      (paddr_t)CONFIG_MBASE, (paddr_t)CONFIG_MBASE + CONFIG_MSIZE);
}

word_t paddr_read(paddr_t addr, int len) {
#if CONFIG_MTRAC
  //mtrac(buf, addr, "Read");
#endif
  if (likely(in_pmem(addr))) return pmem_read(addr, len);
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
#if CONFIG_MTRAC
  //mtrac(buf, addr, "Write");
#endif
  if (likely(in_pmem(addr))) { pmem_write(addr, len, data); return; }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
  out_of_bound(addr);
}
