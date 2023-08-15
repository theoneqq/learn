#include <stdio.h>
#include <stdlib.h>

int count = 99;

void f() {
	printf("count1: %d\n", count);
	__asm__ __volatile__ (
		"mov $count,%eax\n\t"
		"movl $0x123, (%eax)\n\t"		
	);
	printf("count2: %d\n", count);
	exit(0);
}
