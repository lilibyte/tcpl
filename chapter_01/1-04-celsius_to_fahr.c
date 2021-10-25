/*
 * Exercise 1-3.
 * Modify the temperature conversion program to print a heading
 * above the table.
 *
 * Exercise 1-4.
 * Write a program to print the corresponding Celsius to Fahrenheit
 * table.
*/

#include <stdio.h>

int main() {
  float fahr, celsius;
  int lower, upper, step;

  lower = 0;    /* lower limit of temp table */
  upper = 300;  /* upper limit */
  step  = 20;   /* step size */

  printf("°C\t°F\n");
  printf("-----------\n");

  celsius = lower;
  while (celsius <= upper) {
    fahr = celsius * (9.0/5.0) + 32.0;
    printf("%3.0f\t%3.0f\n", celsius, fahr);
    celsius += step;
  }
}
