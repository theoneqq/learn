#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int count = 10;
	pid_t pid = fork();
	if (pid == 0) {
		count--;
		printf("child path: %d\n", count);
		//exit(EXIT_SUCCESS);  without this code, then child execute from here to the end
	}
	count++;
	printf("parent path: %d\n", count);
	exit(EXIT_SUCCESS);
}
