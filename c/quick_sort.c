#include <stdio.h>

void quick_sort(double* nums, int start, int end) {
	if (start >= end) return;
	int left = start;
	int right = end;

	double k = nums[start];
	while (left < right) {
		while(left < right && nums[right] >= k) --right;
		nums[left] = nums[right];
		while(left < right && nums[left] >= k) --left;
		nums[right] = nums[left];
	}
	nums[left] = k;
	quick_sort(nums, start, left - 1);
	quick_sort(nums, left + 1, end);
}

void main() {
	double nums[4] = { 0.001, 0, 0.999, 1 };
	quick_sort(nums, 0, 3);
	for(int i = 0; i < 4; ++i) {
		printf("%.12lf", nums[i]);
	}
}
