/*
 * Exercise 1-9.
 * Write a program to copy its input to its output, replacing each
 * string of one or more blanks by a sigle blank.
*/

#include <stdio.h>
#include <stdbool.h>

int main() {
  int c = 0;

  bool blank_last = false;
  while ((c = getchar()) != EOF) {
    if (c == ' ')
      blank_last = true;
    else {
      if (blank_last) {
        putchar(' ');
        blank_last = false;
      }
      putchar(c);
    }
  }
}
