typedef struct{
	char *buff;     //buf_pool
	uint32_t read_pointer;
	uint32_t write_pointer;
	uint8_t read_mirror : 1;
	uint8_t write_mirror : 1;
	uint32_t buff_size;
}iringbuf;

void ringbuf_init(struct iringbuf *rb, char *buf, uint32_t size){
	rb->buff = buf;
	rb->size = size;

	rb->read_mirror = rb->read_pointer = 0;
	rb->write_mirror = rb->write_pointer = 0;
}


uint32_t Remain_size(struct iringbuf *rb){

	if(rb->read_mirror = rb->write_mirror)
		return rb->size - (rb->write_pointer - rb->read_pointer);
	else
		return rb->read_pointer - rb->write_pointer;

}

uint32_t ringbuf_write(struct iringbuf *rb, char *str, uint32_t length){

	if(length > rb->buff_size){
		str = &str[length - rb->buff_size];
		length = rb->buff_size;
	}

	uint32_t remain_size = Remain_size(rb);


	if(length < rb->buff_size - rb->write_pointer){                       //have not arrive 0
		memcpy(&rb->buf[rb->write_pointer], str, length);
		rb->write_pointer += length;

		if(length > remain_size)
			rb->read_pointers = rb->write_pointers; //read_pointers always point the oldest one.
		
	}
	else{

		memcpy(&rb->buf[rb->write_pointer], str, rb->buff_size - rb->write_pointer);
                memcpy(&rb->buf[0], &str[rb->buff_size - rb->write_pointer], length - (rb->buff_size - rb->write_pointer));
        	rb->write_pointer = length - (rb->buff_size - rb->write_pointer);



		//in mirror
	        rb->write_mirror = ~rb->write_mirror;            //length is not enough,so in mirror
		if(length > remain_size){                        //wirte catch up the read
        	        rb->read_pointer = rb_write_pointer;     
               		rb->read_mirror = ~rb->read_mirror;      //to distinguish the read and write is not at the same mirror
                }


	}
	return length;
}

uint32_t ringbuf_read(struct iringbuf *rb, char *str, uint32_t length){

        if(length > rb->buff_size){
                str = &str[length - rb->buff_size];
                length = rb->buff_size;
        }

        uint32_t remain_size = Have_size(rb);


        if(length < rb->buff_size - rb->read_pointer){                       //have not arrive 0
                memcpy(srt, &rb->buf[rb->read_pointer], length);
                rb->read_pointer += length;

        }
        else{

                memcpy(str, &rb->buf[rb->read_pointer], rb->buff_size - rb->read_pointer);
                memcpy(&str[rb->buff_size - rb->read_pointer], &rb->buf[0], length - (rb->buff_size - rb->read_pointer));
                rb->read_pointer = length - (rb->buff_size - rb->read_pointer);



                //in mirror
                rb->read_mirror = ~rb->read_mirror;            //length is not enough,so in mirror
                


        }
        return length;
}






