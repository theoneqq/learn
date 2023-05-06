#include <stdio.h> //printf
#include <stdlib.h> //malloc

#include <sys/types.h> //open
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //read write close

#include <errno.h> //errno
#include <string.h> //memcpy
#include "rio.h"

int open_file(const char *file_name, int flags, mode_t mode) {
	int fd = open(file_name, flags, mode);	
	if (fd < 0) {
		printf("open file failed: %s\n", file_name);
	} else {
		printf("open file: %s file id: %d\n", file_name, fd);
	}
	return fd;
}

static ssize_t __rio_read(rio_t *rio, void *buf, size_t n) {
	while (rio->rio_cnt <= 0) {
		rio->rio_cnt = read(rio->rio_fd, rio->rio_buf, sizeof(rio->rio_buf));
		if (rio->rio_cnt < 0) {
			if (errno != EINTR)
				return -1;
		} else if (rio->rio_cnt == 0) {
			return 0;
		} else {
			rio->rio_cur = rio->rio_buf;
		}
	}

	int read_cnt = n;
	if (rio->rio_cnt < read_cnt)
		read_cnt = rio->rio_cnt;

	char *to_buf = buf;
	memcpy(to_buf, rio->rio_cur, read_cnt);

	rio->rio_cnt -= read_cnt;
	rio->rio_cur += read_cnt;
	
	return read_cnt;
}

void rio_init(rio_t *rio, int fd) {
	rio->rio_fd = fd;
	rio->rio_cnt = 0;
	rio->rio_cur = rio->rio_buf;
}

ssize_t rio_readn(rio_t *rio, void *buf, size_t n) {
	ssize_t has_read_cnt;
	size_t left_cnt = n;
	char *buf_pos = buf;
	while (left_cnt > 0) {
		has_read_cnt = __rio_read(rio, buf, n);
		if (has_read_cnt < 0)
			return -1;
		else if (has_read_cnt == 0)
			break;
		buf += has_read_cnt;
		left_cnt -= has_read_cnt;
	}
	return n - left_cnt;
}

ssize_t rio_readline(rio_t *rio, void *buf, size_t n) {
}


int main() {
	rio_t rio;
	int fd = open_file("f.txt", O_RDWR, 0);

	char buf[10];
	rio_init(&rio, fd);
	rio_readn(&rio, buf, 10);

	printf("read content: %s", buf);
}
