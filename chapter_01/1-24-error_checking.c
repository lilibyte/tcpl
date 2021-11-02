/*
 * Exercise 1-24.
 * Write a program to check a C program for rudimentary syntax
 * errors like unbalanced parentheses, brackets, and braces.
 * Don't forget about quotes, both single and double, escape
 * sequences, and comments. (This program is hard if you do it
 * in full generality.)
*/

/*
   th-thanks senpai!

   ~qorg > just count how many parentheses... are in the file
           if it is even it werks
           if it's odd does not
*/

/*
   My attempt to print the location of the offending character
   does not fully work, but I don't feel like putting more
   time into it when it wasn't even part of the exercise.

   Not happy with this in general, and I might redo it. I think
   I've still overcomplicated the problem, and it's very
   confusing to read and follow.
*/

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// [index, count]
int last_quote[2];
int last_single_quote[2];
// [opening_line, opening_index, opening_count,
//  closing_line, closing_index, closing_count]
int last_parentheses[6];
int last_brace[6];
int last_bracket[6];
int last_comment[6];

int linenum = 0;

/* report errors that can span multiple lines last */
int report_errors() {
  if (last_parentheses[2] != last_parentheses[5]) {
    int n = last_parentheses[2] > last_parentheses[5] ? 0 : 3;
    printf("error: unmatched parentheses on line %d,%d\n",
           last_parentheses[n], last_parentheses[n + 1]);
  } else if (last_brace[2] != last_brace[5]) {
    int n = last_brace[2] > last_brace[5] ? 0 : 3;
    printf("error: unmatched brace on line %d,%d\n",
           last_brace[n], last_brace[n + 1]);
  } else if (last_bracket[2] != last_bracket[5]) {
    int n = last_bracket[2] > last_bracket[5] ? 0 : 3;
    printf("error: unmatched bracket on line %d,%d\n",
           last_bracket[n], last_bracket[n + 1]);
  } else if (last_comment[2] != last_comment[5]) {
    int n = last_comment[2] > last_comment[5] ? 0 : 3;
    printf("error: unmatched block comment on line %d,%d\n",
           last_comment[n], last_comment[n + 1]);
  } else {
    return 0;
  }
  return 1;
}

int error_check(char line[]) {
  ++linenum;
  char c;
  int i = 0;
  int blanks = 0;
  while ((c = line[i]) != '\n') {
    switch (c) {
      case '"':
        if ((line[i - 1] != '\\' || line[i - 2] == '\\')) {
          last_quote[0] = i + 1;
          ++last_quote[1];
        }
        break;
      case '\'':
        if (!(last_quote[1] % 2) && (line[i - 1] != '\\' ||
            line[i - 2] == '\\')) {
          last_single_quote[0] = i + 1;
          ++last_single_quote[1];
        }
        break;
      case '(':
        if (!(last_quote[1] % 2)) {
          if (last_parentheses[3] > last_parentheses[0] ||
             (last_parentheses[4] > last_parentheses[1])) {
            last_parentheses[0] = linenum;
            last_parentheses[1] = i + 1;
          }
          ++last_parentheses[2];
        }
        break;
      case ')':
        if (!(last_quote[1] % 2)) {
          last_parentheses[3] = linenum;
          last_parentheses[4] = i + 1;
          ++last_parentheses[5];
        }
        break;
      case '[':
        if (!(last_quote[1] % 2)) {
          if (last_bracket[3] > last_bracket[0] ||
             (last_bracket[4] > last_bracket[1])) {
            last_bracket[0] = linenum;
            last_bracket[1] = i + 1;
          }
          ++last_bracket[2];
        }
        break;
      case ']':
        if (!(last_quote[1] % 2)) {
          last_bracket[3] = linenum;
          last_bracket[4] = i + 1;
          ++last_bracket[5];
        }
        break;
      case '{':
        if (!(last_quote[1] % 2)) {
          if (last_brace[3] > last_brace[0] ||
             (last_brace[4] > last_brace[1])) {
            last_brace[0] = linenum;
            last_brace[1] = i + 1;
          } else {
            last_brace[3] = linenum;
            last_brace[4] = i + 1;
          }
          ++last_brace[2];
        }
        break;
      case '}':
          if (!(last_quote[1] % 2)) {
          last_brace[3] = linenum;
          last_brace[4] = i + 1;
          ++last_brace[5];
        }
        break;
      case '/':
        if (line[i + 1] == '/') {
          return 0;
        } else if (line[i + 1] == '*') {
          if (last_comment[2] != last_comment[5]) {
            printf("error: nested comment on line %d,%d\n", linenum, i + 1);
            return 1;
          }
          last_comment[0] = linenum;
          last_comment[1] = i + 1;
          ++last_comment[2];
        } else if (line[i - 1] == '*' && last_comment[1] != i - 1) {
          last_comment[3] = linenum;
          last_comment[4] = i + 1;
          ++last_comment[5];
        }
        break;
      case '\\':
        if ((last_comment[2] != last_comment[5]) ||
           ((last_quote[1] && last_quote[1] % 2) ||
           (last_single_quote[1] && last_single_quote[1] % 2))) {
          if (line[i + 1] == '\\') {
            // additional increment so i + 1 won't error next iteration
            ++i;
          } else if (line[i + 1] == 'x' &&
                    ((line[i + 2] >= '0' && line[i + 2] <= '9') ||
                     (line[i + 2] >= 'A' && line[i + 2] <= 'F') ||
                     (line[i + 2] >= 'a' && line[i + 2] <= 'f'))) {
            // good
          } else if (line[i + 1] >= '0' && line[i + 1] <= '7') {
            // good
          } else if (line[i + 1] == 'a' || line[i + 1] == 'b' ||
                     line[i + 1] == 'f' || line[i + 1] == 'n' ||
                     line[i + 1] == 'r' || line[i + 1] == 't' ||
                     line[i + 1] == 'e' || line[i + 1] == 'u' ||
                     line[i + 1] == 'U' || line[i + 1] == 'v' ||
                     line[i + 1] == '"' || line[i + 1] == '\'' ||
                     line[i + 1] == '?') {
            // good
          } else {
            printf("error: unknown escape sequence '%c' on line %d,%d\n",
                   line[i + 1], linenum, i + 1);
            return 1;
          }
        } else if (last_comment[2] == last_comment[5]) {
          printf("error: invalid character '%c' on line %d,%d\n",
                 c, linenum, i + 1);
          return 1;
        }
        break;
      default:
        break;
    }
    ++i;
  }
  if (last_quote[1] % 2) {
    printf("%d\n", last_quote[1]);
    printf("error: unmatched double quote on line %d,%d\n",
           linenum, last_quote[0]);
  } else if (last_single_quote[1] % 2) {
    printf("error: unmatched single quote on line %d,%d\n",
           linenum, last_single_quote[0]);
  } else {
    return 0;
  }
  return 1;
}

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int status = 0;
  while ((read = getline(&line, &len, stdin)) != EOF) {
    status = error_check(line);
    if (status) {
      return 1;
    }
  }
  status = report_errors();
  if (!status) {
    printf("no errors found\n");
  }
  free(line);
  return status;
}
