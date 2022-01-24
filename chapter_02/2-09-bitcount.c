/*
 * Exercise 2-9.
 *
 * bitcount:    count 1 bits in x
 * int bitcount(unsigned x) {
 *   int b;
 *   for (int b = 0; x != 0; x >>= 1)
 *     if (x & 01)
 *       b++;
 *   return b;
 * }
 *
 * In a two's complement number system, x &= (x-1) deletes the
 * rightmost 1-bit in x. Explain why. Use this observation to
 * write a faster version of bitcount().
*/

// x    ==  0101 0110     (86)
// x--  ==  0101 0101  &  (85)
//          ------------
//          0101 0100

// x    ==  0101 0101     (85)
// x--  ==  0101 0100  &  (84)
//          ------------
//          0101 0100

// Subtracting 1 from x either removes the rightmost 1-bit, or
// "moves it" to the right. To then & the two numbers means the
// difference between them will not be kept so the "moved" bit
// will also be removed in those cases.

#include <stdio.h>
#include <limits.h>

int bitcount(unsigned long long x) {
  int i = 0;
  while (x) {
    ++i;
    x &= (x - 1);
  }
  return i;
}

int main() {
  printf("%u\n", bitcount(0));
  printf("%u\n", bitcount(UCHAR_MAX));
  printf("%u\n", bitcount(USHRT_MAX));
  printf("%u\n", bitcount(UINT_MAX));
  printf("%u\n", bitcount(ULLONG_MAX));
}
