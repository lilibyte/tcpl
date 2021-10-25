/*
 * Exercise 1-15.
 * Rewrite the temperature conversion program of Section 1.2 to
 * use a function for conversion.
*/

#include <stdio.h>

float convert_fahr(float fahr) {
  return (5.0 / 9.0) * (fahr - 32.0);
}

int main() {
  float fahr;
  int lower, upper, step;

  lower = 0;    /* lower limit of temp table */
  upper = 300;  /* upper limit */
  step  = 20;   /* step size */

  printf("°F\t°C\n");
  printf("--------------\n");

  fahr = lower;
  while (fahr <= upper) {
    printf("%3.0f\t%6.1f\n", fahr, convert_fahr(fahr));
    fahr += step;
  }
}
