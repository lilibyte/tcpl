/*
 * Exercise 3-4.
 *
 * In a two's complement number representation, our version of itoa
 * does not handle the largest negative number, that is, the value
 * of n equal to -(2^(wordsize-1)). Explain why not. Modify it to
 * print that value correctly, regardless of the machine on which
 * it runs.
*/

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void reverse(char s[]) {
  for (int i = 0, j = strlen(s)-1; i < j; i++, j--) {
    int c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[]) {
  int sign;
  bool overflow = n == INT_MIN;
  // The unary minus invokes undefined behavior due to signed integer
  // overflow when applied to INT_MIN on 2's complement platforms.
  // https://en.cppreference.com/w/c/language/operator_arithmetic
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
  s[i] = '\0';
  // probably not capturing the spirit of the exercise but whatever
  if (overflow)
    ++s[0];
  reverse(s);
}

int main() {
  char s1[1000];
  itoa(INT_MIN+1, s1);
  printf("%s\n", s1);
  itoa(INT_MIN, s1);
  printf("%s\n", s1);
}
