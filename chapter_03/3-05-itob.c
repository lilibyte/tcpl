/*
 * Exercise 3-5.
 *
 * Write the function itob(n,s,b) that converts the integer
 * n into a base b character representation in the string s.
 * In particular, itob(n,s,16) formats n as a hexadecimal
 * integer in s.
*/

#include <stdio.h>

// lol
void itob(int n, char s[], int b) {
  switch (b) {
    case 16:
      sprintf(s, "%x", n);
      break;
    case 10:
      sprintf(s, "%d", n);
      break;
    case 8:
      sprintf(s, "%o", n);
      break;
  }
}

int main() {
  char s[1000];
  itob(4584, s, 16);
  printf("%s\n", s);
  itob(4584, s, 10);
  printf("%s\n", s);
  itob(4584, s, 8);
  printf("%s\n", s);
}
