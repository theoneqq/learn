#include <stdio.h>

double F(double r) {
	int i = 1;
	double answer = 1.0;
	double t = 1.0;
	while (i < 6) {
		t = t * r * r / (2 * i - 1) / (2 * i);
		answer += t * (i % 2 == 0? 1 : -1);
		++i;
	}

	return answer;
}

int main() {
	printf("F(0.0)%.12lf\n", F(0.0));
	printf("F(3.14 / 6)%.12lf\n", F(3.14 / 6));
	printf("F(0.78)%.12lf\n", F(0.78));
	return 0;
}
