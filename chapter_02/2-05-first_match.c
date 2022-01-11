/*
 * Exercise 2-4.
 *
 * Write the function any(s1, s2), which returns
 * the first location in the string s1 where any
 * character from the string s2 occurs, or -1 if
 * s1 contains no characters from s2. (The standard
 * library function strpbrk does the same job but
 * returns a pointer to the location.)
*/

#include <stdio.h>
#include <stdbool.h>

int any(char s1[], char s2[]) {
  bool matches[256] = { false };
  for (int i = 0; s2[i] != '\0'; ++i)
    matches[s2[i]] = true;
  for (int i = 0; s1[i] != '\0'; ++i)
    if (matches[s1[i]])
      return i;
  return -1;
}

int main() {
  printf("%d\n", any("And you don't seem to understand", "A"));
  printf("%d\n", any("A shame you seemed an honest man", "zzzzz"));
  printf("%d\n", any("And all the fears you hold so dear", "zzzzfzzzzz"));
}
