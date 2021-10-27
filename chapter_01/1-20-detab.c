/*
 * Exercise 1-20.
 * Write a program `detab` that replaces tabs in the input with the
 * proper number of blanks to space to the next tab stop. Assume a
 * fixed set of tab stops, say every n columns. Should n be a variable
 * or a symbolic parameter?
*/

/*
  I can't understand this wording at all, so I had to search for an
  explanation of what the goal of the exercise is:
  https://stackoverflow.com/q/23583252/

  "You have to replace each tab by spaces until the next tab stop.
  That depends exactly on where the cursor is.
  So, if there's a tab stop every 5 columns:
  -----|-----|-----|-----|-----|-----|-----|-----|-----|
  Where | is a tab stop and - is a character. Hitting a tab takes you
  to the next tab stop."

  "You don't want to replace every tab by the same number of spaces:
  some will be replaced by less than that."
*/

/*
   Runs successfully assuming I understand the exercise correctly:

   echo -e "XXXXX\tXXX\tX\tXX\tXXXX\tXXXXX\tXX\tX" | ./chapter_01/1-20-detab
   XXXXX XXX   X     XX    XXXX  XXXXX XX    X
*/

#include <stdio.h>

#define  MAXLINE  79

void print_aligned_columns(char input_line[], int column_len) {
  int current_word_len = 0;
  int index = 0;
  while (input_line[index] != '\0') {
    if (input_line[index] == '\t') {
      for(int i = 0; i <= (column_len - current_word_len); ++i) {
        printf(" ");
      }
      current_word_len = 0;
    } else if (current_word_len >= column_len) {
      printf("\nerror: incorrect formatting; "
             "expecting <=%d character columns\n", column_len);
      break;
    } else {
      printf("%c", input_line[index]);
      ++current_word_len;
    }
    ++index;
  }
}

int getinput(char input_line[]) {
  char c;
  int i = 0;
  while (i < MAXLINE-1 && (c=getchar()) != EOF && c != '\n') {
    input_line[i] = c;
    ++i;
  }
  if (i && c == '\n') {
    input_line[i] = c;
    ++i;
  }
  input_line[i] = '\0';
  return i;
}

int main() {
  int len;
  char line[MAXLINE];
  while ((len=getinput(line)) > 0) {
    print_aligned_columns(line, 5);
  }
}
