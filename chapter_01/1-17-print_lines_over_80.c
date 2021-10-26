/*
 * Exercise 1-17.
 * Write a program to print all input lines that are longer
 * than 80 characters.
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
    if (len > 80) {
      printf("%s", line);
    }
  }
}
