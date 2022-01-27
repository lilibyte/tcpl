/*
 * Exercise 3-3.
 *
 * Write a function expand(s1,s2) that expands shorthand
 * notations like a-z in the string s1 into the equivalent
 * complete list abc...xyz in s2. Allow for letters of
 * either case and digits, and be prepared to handle cases
 * like a-b-c and a-z0-9 and -a-z. Arrange that a leading
 * or trailing - is taken literally.
*/

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

char *expand(const char s1[], char s2[]) {
  char c;
  int i = 0;
  int diff = 0;
  int last_valid = -1;
  bool valid_shorthand_found = false;
  char next_char;
  char prev_char;

  while ((c = s1[i]) != '\0') {
    bool within_bounds = i && s1[i+1] != '\0';
    if (c == '-' && within_bounds) {
      next_char = s1[i+1];
      prev_char = s1[i-1];
      if ((prev_char < next_char) && (i - 1 != last_valid)) {
        bool both_lower = islower(prev_char) && islower(next_char);
        bool both_upper = isupper(prev_char) && isupper(next_char);
        bool both_digit = isdigit(prev_char) && isdigit(next_char);
        if (both_lower || both_upper || both_digit)
          valid_shorthand_found = true;
      }
    }
    if (valid_shorthand_found) {
      last_valid = i + 1;
      valid_shorthand_found = false;
      for (int j = prev_char + 1; j <= next_char; ++j) {
        s2[diff] = j;
        ++diff;
      }
      i += 2;
    } else {
      s2[diff] = c;
      ++i;
      ++diff;
    }
  }
  s2[diff] = '\0';
  return s2;
}

int main() {
  char s2[100];
  printf("%s\n", expand("a-z0-3A-C", s2));
  printf("%s\n", expand("A-z4-2z-xp-G", s2));
  printf("%s\n", expand("-a-z a-b-c", s2));
  printf("%s\n", expand("0-9a-zA-Z", s2));
}
