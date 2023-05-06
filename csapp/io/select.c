#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>


#define TIMEOUT 5
#define BUF_LEN 10

void do_select() {
	struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	int ret;
CALL:
	ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv); //register

	if (ret == -1) {
		if (errno != EINTR) {
			printf("select error!");
		} else {
			goto CALL;
		}
	} else if (!ret) {
		printf("maybe timeout!");
	} else {
		if (FD_ISSET(STDIN_FILENO, &readfds)) { // some fd has ready, ask STDIN_FILENO is ok?
			char buf[BUF_LEN + 1];
			int len = read(STDIN_FILENO, buf, BUF_LEN);
			if (len < 0) {
				printf("read error!");
			} else {
				//buf[len] = '\0';
				printf("read data: %s\n", buf);
			}
		}
	}

}

void do_sleep() {
	long int sleep_time = 500;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = sleep_time;

	select(0, NULL, NULL, NULL, &tv); //sleep 500 millseconds

	printf("sleep %ld millseconds\n", sleep_time); // can not use tv.tv_usec, it can be zero?
}

int main() {
	do_sleep();
	return 0;
}
