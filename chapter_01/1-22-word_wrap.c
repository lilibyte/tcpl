/*
 * Exercise 1-22.
 * Write a program to "fold" long input lines into two or more shorter
 * lines after the last non-blank character that occurs before the
 * n-th column of input. Make sure your program does something
 * intelligent with very long lines, and if there are no blanks or
 * tabs before the specified column.
*/

/*
   What a mess! I don't know algorithms but I tried to make it
   slightly more efficient than pure brute forcing. Pseudo code:

   if the current char == ' ' and the remaining chars haven't been checked:
       for i in range(index, index + (wrap - cursor)):
           if line[i] == ' ':
               last_space that would fit on the line = i
           elif line[i] == '\n':
               remaining chars necessarily fit on line
       already_checked the remaining chars = 1
       if no space to wrap on and more than ~2/3 of the line is filled:
           print a newline and start over
   elif cursor at wrap edge and no spaces within the last ~1/3 of the line:
       print a "-" and a newline and continue
   elif line was already checked and the current char is the last_space:
       print a newline and continue
   if current char is not a leftover space:
       print the char and continue
*/

#include <stdio.h>

void foldline(char line[], int wrap, int linelen) {
  --wrap;
  char c = 0;
  int i = 0;
  int cursor = 0;           // location of cursor 0-wrap
  int last_space = 0;       // index of last space in line or 0
  int already_checked = 0;  // already found last space in line
  if (linelen - 1 < wrap) {
    printf("%s", line);
  } else {
    while ((c = line[i]) != '\0') {
      if ((c == ' ' || c == '\t') && cursor <= wrap && !already_checked) {
        last_space = 0;
        for (int j = i + 1; j <= i + (wrap - cursor); ++j) {
          if (line[j] == ' ' || line[j] == '\t') {
            last_space = j;
            already_checked = 1;
          } else if (line[j] == '\n') {
            last_space = 0;
            already_checked = 1;
            break;
          }
        }
        if (!last_space && (wrap - cursor) <= wrap / 3) {
          printf("\n");
          already_checked = 0;
          cursor = 0;
        }
      } else if (cursor == wrap && !already_checked) {
        printf("-\n");
        already_checked = 0;
        cursor = 0;
      } else if (already_checked && i == last_space) {
        printf("\n");
        cursor = 0;
        already_checked = 0;
      }
      if (!(i > 0 && cursor == 0)) {
        printf("%c", c);
      }
      ++i;
      ++cursor;
    }
  }
}

int getinput(char line[]) {
  char c;
  int i = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    line[i] = c;
    ++i;
  }
  if (i && c == '\n') {
    line[i] = c;
    ++i;
  }
  line[i] = '\0';
  return i;
}

int main() {
  char line[1000];
  int len = 0;
  while ((len = getinput(line)) > 0) {
    foldline(line, 40, len);
  }
}
