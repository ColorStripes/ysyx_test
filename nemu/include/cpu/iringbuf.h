typedef struct{
	char *buff;     //buf_pool
	uint32_t read_pointer;
	uint32_t write_pointer;
	uint8_t read_mirror : 1;
	uint8_t write_mirror : 1;
	uint32_t buff_size;
}iringbuf;

void ringbuf_init(iringbuf *rb, char *buf, uint32_t size);

uint32_t Remain_size(iringbuf *rb);

uint32_t ringbuf_write(iringbuf *rb, char *str, uint32_t length);

uint32_t ringbuf_read(iringbuf *rb, char *str, uint32_t length);

void printf_iring();







