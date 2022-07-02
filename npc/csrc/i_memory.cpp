#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>
typedef uint64_t paddr_t;
#define CONFIG_MSIZE 1000
#define CONFIG_MBASE 0x80000000

#if defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] = {};
#endif

//read.bin
void read_inst(char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    printf("Can not open this file!\n");
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  size = fread(pmem, size, 1, fp);
  fclose(fp);
}

//pmem_read
uint8_t *guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }

static inline paddr_t host_read(void *addr, int len)
{
  switch (len)
  {
    case 1:
     return *(uint8_t *)addr;
    case 2:
      return *(uint16_t *)addr;
    case 4:
      return *(uint32_t *)addr;
    case 8:
      return *(uint64_t *)addr;
    default:
    {
      assert(0);
      return 0;
    }
  }
}

static inline void host_write(void *addr, int len, uint64_t data)
{
  switch (len)
  {
  case 1:
    *(uint8_t *)addr = data;
    return;
  case 2:
    *(uint16_t *)addr = data;
    return;
  case 4:
    *(uint32_t *)addr = data;
    return;
  case 8:
    *(uint64_t *)addr = data;
    return;
  default:
    assert(0);
  }
}

uint64_t pmem_read(paddr_t addr, int len)
{
  // printf("addr:%lx\n",addr);
  uint64_t ret = host_read(guest_to_host(addr), len);
  return ret;
}

void pmem_write(paddr_t addr, int len, uint64_t data)
{
  host_write(guest_to_host(addr), len, data);
}
