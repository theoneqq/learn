#include <stdio.h>
#include <stdlib.h> //exit
#include <signal.h>
#include <unistd.h> //pause

//typedef void (*sighandler_t)(int);
//sighandler_t signal(int signum, sighandler_t handler);
int global = 99;
void sigint_handler(int sig) {
	global = 999;
	printf("catch SIGINT! and global is %d\n", global);
	//exit(0);
}

int main() {
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		printf("signal error!\n");
		exit(0);
	}

	pause();

	printf("execute and global is %d\n", global);

	return 0;
}
