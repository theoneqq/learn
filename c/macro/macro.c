#include <stdio.h>
#include "macro.h"

int main() {
	int i = lmod(10, 10000);
	printf("lmod: %d\n", i);
	return 0;
}
