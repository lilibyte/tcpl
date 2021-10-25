/*
 * Exercise 1-14.
 * Write a program to print a histogram of the frequencies
 * of different characters in its input.
*/

#include <stdio.h>

int main() {
  int c = 0;

  // the 96 printable ASCII characters between 32 and 127
  int wordlens[96] = {0};

  while ((c = getchar()) != '\n')
    // increment if not outside of printable range
    if (!(c > 127 || c < 32))
      ++wordlens[c-32];
  for (int i = 0; i < 96; ++i)
    // if the char has been given at least once
    if (wordlens[i]) {
      printf("%c: ", i + 32);
      for (int j = 0; j < wordlens[i]; ++j)
        printf("*");
      printf("\n");
    }
}
