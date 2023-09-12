#include "kernel/types.h"
#include <user/user.h>


int main(int argc, char *argv[]) {
	if(argc < 1) {
		printf("no sleep time!\n");
	} else {
		int sleep_time = atoi(argv[1]);
		sleep(sleep_time);
	}
	exit(0);
}
