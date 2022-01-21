/*
 * Exercise 2-6.
 *
 * Write a function setbits(x,p,n,y) that returns x
 * with the n bits that begin at position p set to the
 * rightmost n bits of y, leaving the other bits unchanged.
*/

#include <stdio.h>
#include <limits.h>

// "we assume that bit position 0 is the right end
// and that n and p are sensible positive values"

// n must be <= (p + 1)
int setbits(unsigned x, int p, int n, unsigned y) {

  // easier for me to do manually for some reason

  // o_x =  0101 0101           (85)
  //          (p+1-n) >>        (2)
  //        ------------
  //   x =  0001 0101
  //        1111 1000  &
  //        ------------
  //        0001 0000           (16)

  unsigned copy_x = x;
  unsigned mask_x = ~0U << n;

  x >>= (p+1-n);
  x &= mask_x;

  // o_y =  1010 0110           (166)
  //          (p+1-n) >>        (2)
  //        ------------
  //   y =  0010 1001           (41)
  //        0000 0111  &
  //        ------------
  //   y =  0000 0001           (1)
  //   x =  0001 0000  |        (16)
  //        ------------
  //        0001 0001           (17)
  //          (p+1-n) <<        (2)
  //        ------------
  //   x =  0100 0100           (68)

  unsigned mask_y = ~mask_x;

  y >>= (p+1-n);
  y &= mask_y;

  x |= y;
  x <<= (p+1-n);

  // o_x =  0101 0101           (85)
  //        0000 0111  &
  //        ------------
  //        0000 0101
  //   x =  0100 0100  |
  //        ------------
  //        0100 0101           (69)

  x |= (copy_x & mask_y);
  return x;
}

int main() {
  printf("%u\n", setbits(85, 4, 3, 166));
  // idk if this is correct, i'll come back to it if it's not
}
