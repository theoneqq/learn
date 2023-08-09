#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void s_handler1(int sig) {
	int old_errno = errno;
	printf("come here\n");
	int status;
	if ((wait(&status)) < 0) {
		printf("wait pid error!\n");
	}

	printf("handler reaped child\n");
	//sleep(1);
	errno = old_errno;
}

void s_handler2(int sig) {
	int old_errno = errno;
	int pid;
	printf("come here\n");
	while ((pid = waitpid(-1, NULL, 0)) > 0) {
		printf("handler reaped child %d\n", pid);
	}

	if (errno != ECHILD) {
		printf("wait pid error!\n");
	}

	sleep(1);
	errno = old_errno;
}

int main() {
	int i, n;
	char buf[100];
	if (signal(SIGCHLD, s_handler1) == SIG_ERR) {
		printf("signal error\n");
		exit(0);
	}

	for (i = 0; i <= 3; ++i) {
		if (fork() == 0) {
			printf("hello from child %d\n", (int) getpid());
			sleep(2 * i);
			exit(0);
		}
	}

	if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
		printf("read error\n");
		exit(0);
	}

	printf("parent processing input\n");
	while(1)
		;

	exit(0);
}
