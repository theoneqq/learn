#include <stdio.h>
#include <setjmp.h>

int main() {
	jmp_buf buf;
	int count = 0;
	int r;
	r = setjmp(buf);
	printf("r: %d\n", r);
	
	if (count > 10)
		return 0;
	if (r <= 1) {
		count++;
		printf("if count: %d\n", count):
	} else {
		count += 2;
		printf("else count: %d\n", count);
		longjmp(buf, 1);
	}

	longjmp(buf, 2);
	return 0;
}
