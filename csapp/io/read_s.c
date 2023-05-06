#include <unistd.h>
int main(){ 
	int fd = 0;
	char buf[10];
	read(fd, buf, 10);
	return 0;
}
