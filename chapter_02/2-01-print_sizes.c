/*
 * Exercise 2-1. Write a program to determine the ranges of
 * char, short, int, and long variables, both signed and unsigned,
 * by printing appropriate values from standard headers and by
 * direct computation. Harder if you compute them: determine the
 * ranges of the various floating-point types.
*/

#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>

// using %d for unsigned ints results in -1 for this reason:
// https://stackoverflow.com/q/66117245/

int main() {
  long double i;
  printf("char:\n");
  printf("--------------------------------------------------\n");
  printf("CHAR_BIT\t%d\tbits in a char\n", CHAR_BIT);
  printf("CHAR_MAX\t%d\tmaximum value of a char\n", CHAR_MAX);
  printf("UCHAR_MAX\t%d\tmaximum value of an unsigned char\n", UCHAR_MAX);
  printf("SCHAR_MAX\t%d\tmaximum value of an signed char\n", SCHAR_MAX);
  printf("CHAR_MIN\t%d\tminimum value of a char\n", CHAR_MIN);
  printf("SCHAR_MIN\t%d\tminimum value of a signed char\n", SCHAR_MIN);

  printf("\nint:\n");
  printf("--------------------------------------------------\n");
  printf("INT_MAX\t\t%d\tmaximum value of int\n", INT_MAX);
  printf("INT_MIN\t\t%d\tminimum value of int\n", INT_MIN);
  printf("UINT_MAX\t%u\tmaximum value of an unsigned int\n", UINT_MAX);

  printf("\nshort:\n");
  printf("--------------------------------------------------\n");
  printf("SHRT_MAX\t%d\t\tmaximum value of short\n", SHRT_MAX);
  printf("SHRT_MIN\t%d\t\tminimum value of short\n", SHRT_MIN);
  printf("USHRT_MAX\t%u\t\tmaximum value of an unsigned short\n", USHRT_MAX);

  printf("\nlong:\n");
  printf("--------------------------------------------------\n");
  printf("LONG_MAX\t%ld\t\tmaximum value of long\n", LONG_MAX);
  printf("LONG_MIN\t%ld\t\tminimum value of long\n", LONG_MIN);
  printf("ULONG_MAX\t%lu\t\tmaximum value of an unsigned long\n", ULONG_MAX);

  printf("\nlong long:\n");
  printf("--------------------------------------------------\n");
  printf("LLONG_MAX\t%lld\t\tmaximum value of long long\n", LLONG_MAX);
  printf("LLONG_MIN\t%lld\t\tminimum value of long long\n", LLONG_MIN);
  printf("ULLONG_MAX\t%llu\t\tmaximum value of an unsigned long long\n", ULLONG_MAX);

  printf("\nfloat:\n");
  printf("--------------------------------------------------\n");
  printf("FLT_MAX\t\t%e\tmaximum floating-point number\n", FLT_MAX);
  printf("FLT_MIN\t\t%f\tminimum normalized floating-point number\n", FLT_MIN);

  printf("\ndouble:\n");
  printf("--------------------------------------------------\n");
  printf("DBL_MAX\t\t%e\tmaximum double floating-point number\n", DBL_MAX);
  printf("DBL_MIN\t\t%f\tminimum normalized double floating-point number\n", DBL_MIN);

  /*
     What I've already learned online prior to this is that the number
     of values a type can hold is 2^n where n is the number of bits
     the type holds
     https://stackoverflow.com/a/5789914

     When I first wrote this I had the following function:

     ```
     unsigned long long max_value(int n) {
       unsigned long long result = 1;
       for (int i = 1; i <= n; ++i) {
         result *= 2;
       }
       return result - 1;
     }
     ```

     But apparently you can usually just use bit shifting to do
     what I didn't anyway
     https://stackoverflow.com/a/213043
     oh well

     In doing that I got this warning:
     Shift count >= width of type
     solved with this:
     https://stackoverflow.com/a/4201315
  */

  printf("\ncomputed i guess:\n");
  printf("--------------------------------------------------\n");
  printf("unsigned char:      %d\n", (1 << CHAR_BIT) - 1);
  printf("unsigned short:     %d\n", (1 << sizeof(short) * CHAR_BIT) - 1);
  printf("unsigned int:       %lu\n", (1UL << sizeof(int) * CHAR_BIT) - 1);
  printf("unsigned long:      %lu\n", (1UL << sizeof(long) * CHAR_BIT) - 1);
  printf("unsigned long long: %lu\n", (1UL << sizeof(long long) * CHAR_BIT) - 1);

  printf("%f\n", powf(2.0, 64.0));
}
