#include "kernel/types.h"
#include <user/user.h>

void next(int p[]) {
	close(p[1]);
	int n;
	if (read(p[0], &n, sizeof(int)) > 0) {
		printf("prime %d\n", n);
		int p2[2];
		pipe(p2);
		if (fork() == 0) {
			close(p[0]);
			close(p2[1]);
			next(p2);
		} else {
			close(p2[0]);
			int m;
			while (read(p[0], &m, sizeof(int)) > 0) {
				//printf("read: %d, pid: %d\n", m, getpid());
				if (m % n != 0) {
					write(p2[1], &m, sizeof(int));	
				}
			}
			close(p2[1]);
			close(p[0]);
			wait(0);
		}
	}
	exit(0);
}

int main(int argc, char *argv[]) {
	int p[2];
	pipe(p);
	if (fork() == 0) {
		next(p);
	} else {
		close(p[0]);
		for (int i = 2; i <= 35; ++i) {
			write(p[1], &i, sizeof(int));
		}
		close(p[1]);
		wait(0);
	}
	exit(0);
}
