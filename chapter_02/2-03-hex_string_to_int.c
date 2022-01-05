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
    if ((c == '0' && i == 0) || ((c == 'x' || c == 'X') && i == 1)) {
      ++i;
      continue;
    }
    if (c >= '0' && c <= '9') {
      c = c - '0';
    } else if (c >= 'a' && c <= 'f') {
      c = c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      c = c - 'A' + 10;
    } else {
      return 0;
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
