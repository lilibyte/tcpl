/*
 * Exercise 4-2.
 *
 * Extend atof to handle scientific notation of the form
 * 123.45e-6 where a floating-point number may be followed by
 * e or E and an optionally signed exponent.
*/

#include <stdio.h>
#include <ctype.h>
#include <math.h>

double atof(char s[]) {
  double val, expval, power;
  int i;
  for (i = 0; isspace(s[i]); ++i)
    ;
  int sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-')
    ++i;
  for (val = 0.0; isdigit(s[i]); ++i)
    val = 10.0 * val + (s[i] - '0');
  if (s[i] == '.')
    ++i;
  for (power = 1.0; isdigit(s[i]); ++i) {
    val = 10.0 * val + (s[i] - '0');
    power *= 10.0;
  }
  if (s[i] == 'e' || s[i] == 'E')
    ++i;
  int expsign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-')
    ++i;
  for (expval = 0.0; isdigit(s[i]); ++i)
    expval = 10.0 * expval + (s[i] - '0');
  return (sign * val / power) * pow(10, expval * expsign);
}

int main() {
  printf("%.2f\n", atof("123.45"));
  printf("%.1f\n", atof("123.45e6"));
  printf("%.8f\n", atof("123.45e-6"));
  printf("%.8f\n", atof("123.45e1"));
  printf("%.8f\n", atof("123.45e0"));
}
