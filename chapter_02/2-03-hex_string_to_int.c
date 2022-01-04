/*
 * Exercise 2-3.
 *
 * Write the function htoi(s), which converts a string
 * of hexadecimal digits (inucluding an optional 0x or 0X) into
 * its equivalent integer value. The allowable digits are
 * 0 through 9, a through f, and A through F.
*/

#include <stdio.h>

/* https://stackoverflow.com/a/213897 */
long powi(int p, unsigned n) {
    long r = 1;
    while (n > 0) {
      if (n % 2 == 1)
          r *= p;
      p *= p;
      n /= 2;
    }
    return r;
}

unsigned long htoi(char s[]) {
  char c;
  int p = 0, i = 0;
  long n = 0;
  int converted[100];

  while ((c = s[i]) != '\0') {
    if (c == '0' && i == 0) {
      ++i;
      continue;
    } else if ((c == 'x' || c == 'X') && i == 1) {
      ++i;
      continue;
    }

    if (c >= '0' && c <= '9') {
      c = c - '0';
    } else {
      switch (c) {
        case 'a':
        case 'A':
          c = 10;
          break;
        case 'b':
        case 'B':
          c = 11;
          break;
        case 'c':
        case 'C':
          c = 12;
          break;
        case 'd':
        case 'D':
          c = 13;
          break;
        case 'e':
        case 'E':
          c = 14;
          break;
        case 'f':
        case 'F':
          c = 15;
          break;
        default:
          return 0;
      }
    }
    converted[p] = c;
    ++i;
    ++p;
  }

  int j = 0;
  while (p) {
    --p;
    n += converted[j] * powi(16, p);
    ++j;
  }
  return n;
}

int main() {
  printf("%lu\n", htoi("0x1"));
  printf("%lu\n", htoi("0X2"));
  printf("%lu\n", htoi("deadbeef"));
  printf("%lu\n", htoi("0xdeadbeef"));
  printf("%lu\n", htoi("ffff"));
  printf("%lu\n", htoi("0Xffff"));
  printf("%lu\n", htoi("this in not hexadecimal"));
}
