#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

#define TIMEOUT 2
void do_poll() {
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN | POLLPRI;
	//fds[1].fd = STDOUT_FILENO;
	//fds[1].events = POLLOUT;

	int ret;
	//scanf("%d", &ret);
CALL:
	ret = poll(fds, 1, TIMEOUT * 1000);

	if (ret == -1)  {
		if (errno != EINTR) {
			printf("pool error!\n");
		} else goto CALL;
	} else if (!ret) {
		printf("timeout!\n");
	} else {
		if ((fds[0].revents & POLLIN) || (fds[0].revents & POLLPRI) ) { //how to make this true?
			printf("stdin is ok!\n");
		}
		/*if (fds[1].revents & POLLOUT) {
			printf("stdout is ok!\n");
		}*/
	}
	return;
}

int main() {
	do_poll();
	return 0;
}
