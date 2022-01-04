/*
 * Exercise 2-2.
 *
 * for (i=0; i<lim-1 && (c=getchar()) != '\n' && c != EOF; ++i)
 *
 * Write a loop equivalent to the for loop above without
 * using && or ||.
*/

#include <stdio.h>
#define MAXLINE 1000

/* getlinelength:  read a line into s, return length */
int getlinelength(char s[]) {
  int c, i = 0;
  while ((c=getchar()) != EOF) {
    if (c == '\n') {
      break;
    } else if (i < MAXLINE - 1) {
      s[i] = c;
      ++i;
    } else {
      break;
    }
  }
  if (c == '\n') {
    s[i] = c;
    ++i;
  }
  s[i] = '\0';
  return i;
}

int main() {
  int len = 0;
  char line[MAXLINE];
  while ((len = getlinelength(line)) > 0) {
    printf("%s", line);
  }
}
