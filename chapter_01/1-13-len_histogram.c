/*
 * Exercise 1-13.
 * Write a program to print a histogram of the lengths of words
 * in its input. It is easy to draw the histogram with the bars
 * horizontal; a vertical orientation is more challenging.
*/

// I am not dealing with vertically printing these right now lol

#include <stdio.h>

int main() {
  int c = 0, wl = 0;
  // array of world lengths in the format of
  // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10+]
  int wordlens[10] = {0};

  while ((c = getchar()) != EOF) {
    if (c == '\n' || c == '\t' || c == ' ') {
      if (wl <= 9 && wl > 0)
        ++wordlens[wl-1];
      else if (wl > 9)
        ++wordlens[9];
      wl = 0;
    } else {
      ++wl;
    }
  }

  for (int i = 0; i < 10; ++i) {
    if (i == 9)
      printf("%d+: ", i+1);
    else
      printf("  %d: ", i+1);
    for (int j = 0; j < wordlens[i]; ++j)
      printf("*");
    printf("\n");
  }
}
