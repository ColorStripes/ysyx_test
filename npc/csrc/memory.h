uint8_t *guest_to_host(paddr_t paddr);
uint64_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, uint64_t data);
