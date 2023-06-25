#include <stdio.h>

int bitXor(int x, int y) {
  return (~x & y) & (~(x & ~y));
};
int isAsciiDigit(int x) {
  int l = x - 0x30;
  int r = 0x39 - x;
  return !((l >> 31) | (r >> 31));
};

int main() {
  printf("%d\n", isAsciiDigit(0x35));
  printf("%d\n", isAsciiDigit(0x3a));
  printf("%d\n", isAsciiDigit(0x05));
  return 0;
}
