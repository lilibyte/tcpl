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
   Runs successfully assuming I understand the exercise correctly
   (second attempt as a result of confusion from exercise 1-21):

   echo -e "1\t12\t123\t1234\t12345" | ./chapter_01/1-20-detab
   1               12              123             1234            12345
*/

#include <stdio.h>

#define  MAXLINE  79
#define  TABSTOP  8

void print_aligned_columns(char input_line[], int column_len) {
  int current_word_len = 0;
  int index = 0;
  while (input_line[index] != '\0') {
    if (input_line[index] == '\t') {
      for(int i = 0; i < (TABSTOP - current_word_len); ++i) {
        printf(" ");
      }
      current_word_len = 0;
    } else {
      ++current_word_len;
    }
    printf("%c", input_line[index]);
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
