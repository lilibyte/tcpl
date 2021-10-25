/*
 * Exercise 1-12.
 * Write a program that prints its input one word per line.
*/

#include <stdio.h>

int main() {
  int c = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\t')
      putchar('\n');
    else
      putchar(c);
  }
}
