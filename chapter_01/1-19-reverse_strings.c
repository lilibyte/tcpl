/*
 * Exercise 1-19.
 * Write a function `reverse(s)` that reverses the character
 * string `s`. Use it to write a program that reverses its input a
 * line at a time.
*/

#include <stdio.h>
#define MAXLINE 1000

/* reverse:  read a line into arr, return length */
int reverse(char arr[]) {
  int c, len;
  for (len=0; len<MAXLINE-1 && (c=getchar()) !=EOF && c!='\n'; ++len) {
    arr[len] = c;
  }
  char reversed[len+1];
  int j = 0;
  for (int i = len-1; i >= 0; --i) {
    reversed[j] = arr[i];
    ++j;
  }
  for (int i = 0; i < len; ++i) {
    arr[i] = reversed[i];
  }
  if (len && c == '\n') {
    arr[len] = c;
    ++len;
  }
  arr[len] = '\0';
  return len;
}

int main() {
  int len = 0;
  char line[MAXLINE];
  while ((len = reverse(line)) > 0) {
    printf("%s", line);
  }
}
