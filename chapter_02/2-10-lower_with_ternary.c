/*
 * Exercise 2-10.
 *
 * int lower(int c) {
 *   if (c >= 'A' && c <= 'Z')
 *     return c + 'a' - 'A';
 *   else
 *     return c;
 * }
 *
 * Rewrite the function lower, which converts upper case
 * letters to lower case, with a conditional expression
 * instead of if-else.
*/

int lower(int c) {
  return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c;
}

int main() {
  printf("%c\n", lower('L'));
  printf("%c\n", lower('a'));
  printf("%c\n", lower('I'));
  printf("%c\n", lower('n'));
}
