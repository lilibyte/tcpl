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
   Couldn't resist but to rewrite part of it. Bit easier to
   read now.
*/

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

// [index, count]
int last_quote[2];
int last_single_quote[2];
// [opening_line, opening_index, opening_count,
//  closing_line, closing_index, closing_count]
int last_parentheses[6];
int last_brace[6];
int last_bracket[6];
int last_comment[6];

int linenum = 1;

/* report errors that can span multiple lines last */
int report_errors() {
  if (last_parentheses[2] != last_parentheses[5]) {
    printf("error: unmatched parentheses\n");
  } else if (last_brace[2] != last_brace[5]) {
    printf("error: unmatched brace\n");
  } else if (last_bracket[2] != last_bracket[5]) {
    printf("error: unmatched bracket\n");
  } else {
    return 0;
  }
  return 1;
}

int error_check(char line[]) {
  char c;
  int i = 0;
  int blanks = 0;
  int n;
  while ((c = line[i]) != '\n') {

    n = 0;

    // no unmatched comment block
    if (c == '"' && (last_comment[2] == last_comment[5])) {
      if ((line[i - 1] != '\\' || line[i - 2] == '\\')) {
        if (line[i - 1] == '\'') {
          ++i;
          continue;
        }
        last_quote[0] = i + 1;
        ++last_quote[1];
      }
    }

    // no unmatched comment block or double quotes
    if (c == '\'' && (last_comment[2] == last_comment[5]) &&
        (!(last_quote[1] % 2))) {
      if (line[i - 1] != '\\' || line[i - 2] == '\\') {
        last_single_quote[0] = i + 1;
        ++last_single_quote[1];
      }
    }

    // unmatched comment block
    if (c == '/' && last_comment[2] != last_comment[5]) {
      if (line[i + 1] == '*') {
        printf("error: nested comment on line %d,%d\n", linenum, i + 1);
        return 1;
      }
      // ensure this is not valid:  /*/
      if (line[i - 1] == '*' && ((last_comment[0] != linenum) || (last_comment[1] != i - 2))) {
        last_comment[3] = linenum;
        last_comment[4] = i + 1;
        ++last_comment[5];
      }
      ++i;
      continue;
    }

    // unmatched double or single quote
    if (c == '\\' && (last_quote[1] % 2 || last_single_quote[1] % 2)) {
      switch (line[i + 1]) {
        case '\\':
          ++i;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case 'a': case 'b':
        case 'e': case 'f': case 'r': case 'n': case 't':
        case 'u': case 'U': case 'v': case 'x': case '?':
        case '"': case '\'':
          break;
        default:
          printf("error: unknown escape sequence '%c' on line %d,%d\n",
                 line[i + 1], linenum, i + 1);
          return 1;
      }
    }

    // no unmatched quotes or comments
    if (!(last_single_quote[1] % 2) && !(last_quote[1] % 2) &&
        (last_comment[2] == last_comment[5])) {
      switch (c) {
        case ')':
          n = 3;
        case '(':
          last_parentheses[n] = linenum;
          last_parentheses[n + 1] = i + 1;
          ++last_parentheses[n + 2];
          break;
        case ']':
          n = 3;
        case '[':
          last_bracket[n] = linenum;
          last_bracket[n + 1] = i + 1;
          ++last_bracket[n + 2];
          break;
        case '}':
          n = 3;
        case '{':
          last_brace[n] = linenum;
          last_brace[n + 1] = i + 1;
          ++last_brace[n + 2];
          break;
        case '/':
          if (line[i + 1] == '/') {
            return 0;
          } else if (line[i + 1] == '*') {
            // start of a new comment
            last_comment[0] = linenum;
            last_comment[1] = i + 1;
            ++last_comment[2];
          } else if (line[i - 1] == '*') {
            printf("error: unmatched comment on line %d,%d\n",
                   linenum, i + 1);
            return 1;
          }
          break;
      }
    }
    ++i;
  }

  if ((last_comment[2] == last_comment[5]) && last_quote[1] % 2) {
    printf("error: unmatched double quote on line %d,%d\n",
           linenum, last_quote[0]);
  } else if ((last_comment[2] == last_comment[5]) && last_single_quote[1] % 2) {
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
    ++linenum;
    if (status) {
      printf("%s\n", line);
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
