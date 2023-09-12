#include "kernel/types.h"
#include <user/user.h>

int main(int argc, char *argv[]) {
	int parent_pipe[2];
	int child_pipe[2];
	pipe(parent_pipe);
	pipe(child_pipe);

	int child_pid;
	
	if((child_pid = fork()) == 0) {
		char buf[1];
		close(parent_pipe[1]);
		close(child_pipe[0]);
		read(parent_pipe[0], buf, 1);
		printf("%d: received ping\n", getpid());
		buf[0] = 'c';
		write(child_pipe[1], buf, 1);
		exit(0);
	} else {
		char buf[1];
		buf[0] = 'p';
		close(parent_pipe[0]);
		close(child_pipe[1]);
		write(parent_pipe[1], buf, 1);
		read(child_pipe[0], buf, 1);
		printf("%d: received pong\n", getpid());
	}

	wait(&child_pid);
	exit(0);
}
