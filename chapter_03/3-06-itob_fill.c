/*
 * Exercise 3-6.
 *
 * Write a version of itoa that accepts three arguments
 * instead of two. The third argument is a minimum field
 * width; the converted width must be padded with blanks
 * on the left if necessary to make it wide enough.
*/

#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void reverse(char s[]) {
  for (int i = 0, j = strlen(s)-1; i < j; i++, j--) {
    int c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[], int m) {
  int sign;
  bool overflow = n == INT_MIN;
  if (overflow)
    ++n;
  if ((sign = n) < 0)
    n = -n;
  int i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  if (overflow)
    ++s[0];
  for (int j = 0; j < m - i; ++j)
    s[i + j] = ' ';
  s[i + (m - i >= 0 ? m - i : 0)] = '\0';
  reverse(s);
}

int main() {
  printf("1234567890\n");
  char s1[1000];
  itoa(1000, s1, 10);
  printf("%s\n", s1);
  itoa(-1000, s1, 10);
  printf("%s\n", s1);
  itoa(9999, s1, 3);
  printf("%s\n", s1);
}
