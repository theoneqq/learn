#include <stdio.h>

int main() {
	float a = 1.0;

	int size = (int) sizeof(float);
	char* p = (char*) &a;
	for (int i = 0; i < size; ++i, ++p) {
//		char c = (char) *(&a + i);
//		printf("%c", c);
		printf("%p, %.2x\n", p, *p);
	}

	return 0;
}
