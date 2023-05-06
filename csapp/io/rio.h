#ifdef RIO_H
#else
#define RIO_H
#define RIO_BUFFER_SIZE 8192
typedef struct {
	int rio_fd;
	int rio_cnt;
	char* rio_cur;
	char rio_buf[RIO_BUFFER_SIZE];
} rio_t;

#endif
