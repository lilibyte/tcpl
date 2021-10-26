/*
 * Exercise 1-19.
 * Write a function `reverse(s)` that reverses the character
 * string `s`. Use it to write a program that reverses its input a
 * line at a time.
*/

#include <stdio.h>
#define MAXLINE 1000

/* getinputline:  read a line into arr, return length */
int getinputline(char arr[]) {
  int c, len;
  for (len=0; len<MAXLINE-1 && (c=getchar()) !=EOF && c!='\n'; ++len) {
    arr[len] = c;
  }
  while (arr[len-1] == '\t' || arr[len-1] == ' ') {
    --len;
  }
  if (len && c == '\n') {
    arr[len] = c;
    ++len;
  }
  arr[len] = '\0';
  return len;
}

/* reverse:  reverse character string */
void reverse(char arr[], int len) {
  char reversed[len];
  int newlines = 0;
  int last = len-1;
  int first = 0;
  while (last >= 0) {
    if (arr[last] != '\n') {
      reversed[first] = arr[last];
      ++first;
    } else {
      ++newlines;
    }
    --last;
  }
  while (newlines) {
    reversed[len - newlines] = '\n';
    --newlines;
  }
  reversed[len] = '\0';
  first = 0;
  while ((arr[first] = reversed[first]) != '\0') {
    ++first;
  }
  reversed[0] = '\0';
}

int main() {
  int len = 0;
  char line[MAXLINE];
  while ((len = getinputline(line)) > 0) {
    reverse(line, len);
    printf("%s", line);
  }
}
