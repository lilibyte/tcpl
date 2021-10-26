/*
 * Exercise 1-18.
 * Write a program to remove trailing blanks and tabs from each line
 * of input, and to delete entirely blank lines.
*/

#include <stdio.h>
#define MAXLINE 1000

/* getinputline:  read a line into arr, return length */
int getinputline(char arr[]) {
  int c, len;

  // take input until MAXLINE is hit, or EOF or a newline is returned
  // (if len > 0 and c == '\n', len will end +1 longer than last index)
  for (len=0; len<MAXLINE-1 && (c=getchar()) !=EOF && c!='\n'; ++len) {
    arr[len] = c;
  }

  // decrement len until the first index that isn't whitespace is found
  while (arr[len-1] == '\t' || arr[len-1] == ' ') {
    --len;
  }

  // if the line length isn't 0 && if the last char input was a newline
  if (len && c == '\n') {
    arr[len] = c;
    ++len;
  }

  // set the last index to a null byte
  arr[len] = '\0';
  return len;
}

int main() {
  int len = 0;
  char line[MAXLINE];
  while ((len = getinputline(line)) > 0) {
    printf("%s", line);
  }
}
