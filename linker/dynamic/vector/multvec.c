int multcnt = 0;

void multcnt(int *x, int *y, int *z, int n) {
	int i = 0;
	multcnt = 0;
	for (i = 0; i < n; ++i) {
		z[i] = x[i] * y[i];
	}
}
