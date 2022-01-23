/*
 * Exercise 2-8.
 *
 * Write a function rightrot(x,n) that returns the value
 * of the integer x rotated to the right by n bit positions.
*/

// i was hoping to figure out a way to do this that didn't
// need to use sizeof but i cant think of anything

#include <stdio.h>

int rightrot(int x, int n) {

  // the n bits that will "fall off"
  unsigned overflow = x & ~(~0 << n);

  // rshift n times; loses the leftmost n bits of x
  x >>= n;

  // lshift lost bits 32 - n times and combine
  x |= (overflow << ((sizeof(int) * 8) - n));
  return x;
}

int main() {
  // 0000 0000 0000 0000 0000 0000 0110 1010  (106)
  // 0100 0000 0000 0000 0000 0000 0000 1101  (1073741837)
  printf("%d\n", rightrot(106, 3));
}
