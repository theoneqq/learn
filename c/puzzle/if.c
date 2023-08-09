void f(int c) {
	int x;
	if (c > 0) {
		x = 1;
	} else {
		x = x + 1;
	}
}

int main() {
	f(1);
	f(0); // x = 2?
	return 0;
}
