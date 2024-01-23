#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

int main() {
	struct timespec ts;
	int rc = clock_gettime(CLOCK_MONOTONIC, &ts);
	sleep(5);
	printf("result: %d\n", rc);
	printf("tv_sec: %llu tv_nsec: %llu\n", (long long unsigned int) ts.tv_sec, (long long unsigned int) ts.tv_nsec);
	return 0;
}
