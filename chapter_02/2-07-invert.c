/*
 * Exercise 2-7.
 *
 * Write a function invert(x,p,n) that returns x with the n bits
 * that begin at position p inverted (i.e., 1 changed into 0 and
 * vice versa), leaving the others unchanged.
*/

#include <stdio.h>

int invert(unsigned x, int p, int n) {

  // p = 4, n = 3

  // 0000 0111
  //         p <<
  // ------------
  // 0111 0000
  // 0101 0101  |  (85)
  // ------------
  // 0111 0101     (117)

  unsigned mask_x = x | ~(~0U << n) << p;

  // 0101 0101
  //           >>
  // ------------
  // 0000 0101
  // 0000 0111  &
  // ------------
  // 0000 0101
  //           <<
  // ------------
  // 0101 0000  ~
  // ------------
  // 1010 1111
  // 0111 0101  &
  // ------------
  // 0010 0101

  return ~((~(~0U << n) & x) << p) & mask_x;
}

int main() {
  printf("%u\n", invert(85, 4, 3));
}
