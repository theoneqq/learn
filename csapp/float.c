#include <stdio.h>
#include "../../tools/byte.h"
typedef unsigned float_bits;

//test print by bytes
void print_bytes() {
	float a = 1.0;

	int size = (int) sizeof(float);
	unsigned char* p = (unsigned char*) &a; //must be unsigned
	//char* p = (char*) &a; 
	for (int i = 0; i < size; ++i, ++p) {
		printf("%p, %.2x\n", p, *p);
	}

	printf("%c\n", -20);
	printf("%c\n", 20);

	char c = -1;
	printf("%x\n", c);

	printf("%.2x\n", -20);
	printf("%.2x\n", 20);
}

float_bits float_negate(float_bits f) {
	unsigned sign = f >> 31;
	unsigned exp = f >> 23 & 0xff;
	unsigned fraction = f & 0x7fffff;
	if (exp == 0xff && fraction != 0) return f;
	return fraction | exp << 23 | ((sign == 0? 1 : 0) << 31);
}

//negate a float number based on bit-level operations
float float_negate_v2(float f) {
	//interpret f as unsigned
	unsigned* uf = (unsigned*) &f;
	//get three fields of f
	unsigned sign = (*uf) >> 31;
	unsigned exp = (*uf) >> 23 & 0xff;
	unsigned fraction = (*uf) & 0x7fffff;
	//if NaN
	if (exp == 0xff && fraction != 0) return f;
	//interpret nf as float
	unsigned nf = fraction | exp << 23 | ((sign == 0? 1 : 0) << 31);
	float* nfp = (float*) &nf;
	return *nfp;
}

int main() {
	float_bits a = 10;
	show_int(a);
	show_int(float_negate(a));

	float c = 10.67;
	int* d = (int*) &c;
	show_int(*d);

	printf("test float negate v2 begin\n");
	float e = 10.67;
	show_float(e);
	show_float(float_negate_v2(e));
	printf("test float negate v2 end\n");

	return 0;
}
