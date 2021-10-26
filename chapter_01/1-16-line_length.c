/*
 * Exercise 1-16.
 * Revise the main routine of the longest-line program so it will
 * correctly print the length of arbitrarily long input lines, and
 * as much as possible of the text.
*/

#include <stdio.h>
#define MAXLINE 1000

/* getlinelength:  read a line into s, return length */
int getlinelength(char s[]) {
  int c, i;
  for (i=0; i<MAXLINE-1 && (c=getchar()) !=EOF && c!='\n'; ++i) {
    s[i] = c;
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
    printf("%d\n", len);
  }
}

