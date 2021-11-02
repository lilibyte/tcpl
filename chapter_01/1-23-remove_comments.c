/*
 * Exercise 1-23.
 * Write a program to remove all comments from a C program.
 * Don't forget to handle quotes strings and character constants
 * properly. C comments do not nest.
*/

/*
   I'm tired of defining a function for getting input so I'm now
   switching to `getline()`. I don't know 100% exactly how these
   pointers and larger integer types work but it's not part of
   exercise so just ignoring them for now.

   https://dev-notes.eu/2019/07/Get-a-line-from-stdin-in-C/
   https://linux.die.net/man/3/getline
   https://stackoverflow.com/q/11198305/
*/

/*
   I'm not handling tabs cleanly, but cleaning up whitespace
   after removing comments isn't even part of the exercise, so
   I'm leaving it as it is.
*/

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool inside_quotes = false;
bool inside_block_comment = false;

void destroy_comments(char line[]) {
  char c;
  int i = 0;
  int blanks = 0;
  int wordlen = 0;
  while ((c = line[i]) != '\n') {
    if (c == '/' && !inside_quotes) {
      if (!inside_block_comment && line[i + 1] == '*') {
        inside_block_comment = true;
        ++i;
        continue;
      } else if (inside_block_comment && line[i - 1] == '*') {
        inside_block_comment = false;
        if (line[i + 1] == '\n') {
          break;
        }
        ++i;
        continue;
      } else if (line[i + 1] == '/') {
        if (wordlen) {
          printf("\n");
        }
        break;
      }
    } else if (!inside_block_comment && c == '"' && line[i - 1] != '\\') {
      inside_quotes = !inside_quotes;
    }
    if (!inside_block_comment) {
      if (c == ' ') {
        ++blanks;
      } else {
        while (blanks) {
          printf(" ");
          --blanks;
        }
        printf("%c", c);
        ++wordlen;
      }
    }
    ++i;
  }
  if (!inside_block_comment && c == '\n') {
    printf("\n");
  }
}

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, stdin)) != EOF) {
    destroy_comments(line);
  }
  free(line);
}
