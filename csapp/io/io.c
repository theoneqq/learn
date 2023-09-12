#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

int __open_file(const char* file_name, int flags, mode_t mode) {
	//printf("begin read file %s\n", file_name);
	int fd = open(file_name, flags, mode);	
	if (fd < 0) {
		printf("open file failed: %s\n", file_name);
	} else {
		printf("open file: %s file id: %d\n", file_name, fd);
	}
	return fd;
}

void __read_file(int fd, void* buf, size_t n) {
	size_t has_read_bytes = read(fd, buf, n);
	printf("has read bytes: %d\n", (int) has_read_bytes);
}


void __write_file(int fd, void* buf, size_t n) {
	size_t has_write_bytes = write(fd, buf, n);
	printf("has write byts: %d\n", (int) has_write_bytes);
}


size_t virtual_to_physical(size_t vm_addr) {
}

int main() {
	int size = 3;
	char* buf = (char*) malloc(sizeof(char) * size);
	char write_buf[3] = { '3', '0', '0' };

	int fd = __open_file("f.txt", O_RDWR, 0);
	pid_t pid = fork();

	if (pid == 0) {
		int child_fd = __open_file("f.txt", O_RDWR, 0);
		printf("child pid: %d\n", pid);
		__read_file(child_fd, buf, (size_t) size);
		printf("child has read data: %s\n", (char*) buf);
		//__write_file(fd, "400", (size_t) size);


		write_buf[1] = '9';
		printf("child write buff address: %s\n", write_buf);
		close(fd); // child close fd
		exit(0);
	}

	sleep(5);
	printf("parent pid: %d\n", pid);
	__read_file(fd, buf, (size_t) size);
	printf("parent has read data: %s\n", (char*) buf);
	//__write_file(fd, write_buf, (size_t) size);

	write_buf[2] = '8';
	printf("parent write buff address: %s\n", write_buf);

	close(fd); //parent close fd
	
	exit(0);
}
