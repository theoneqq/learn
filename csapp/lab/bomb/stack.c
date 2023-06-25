#include <stdlib.h>
int f(int* a) {
	return a[0] + a[5];	
}
int main() {
	int* a = (int*) malloc(sizeof(int) * 6);
	a[0] = 1;
	a[5] = 99;
	f(a);
	return 0;
}
