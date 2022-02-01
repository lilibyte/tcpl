/*
 * Exercise 4-1.
 *
 * Write the function strrindex(s,t), which returns the position
 * of the rightmost occurence of t in s, or -1 if there is none.
*/

#include <stdio.h>
#include <string.h>

// alternatively could find every occurence of t in s from
// the left and return the rightmost afterwards to avoid
// linearly looping through s twice (strlen) and potentially
// three times depending on the length of t

int strrindex(const char s[], const char t[]) {
  int len = strlen(s) - 1;
  int mlen = strlen(t) - 1;
  for (int i = len; i >= 0; --i) {
    if (s[i] == t[mlen]) {
      int j = 0;
      for (; j < mlen; ++j)
        if (!(s[i-j] == t[mlen-j]))
          break;
      if (j == mlen)
        return i - mlen;
    }
  }
  return -1;
}

int main() {
  printf("%d\n", strrindex("t", "t"));
  printf("%d\n", strrindex("tt", "t"));
  printf("%d\n", strrindex("test", "test"));
  printf("%d\n", strrindex("testestestest", "test"));
  printf("%d\n", strrindex("teshhhtsettes", "test"));
  printf("%d\n", strrindex("hucrphurcphrucph", "test"));
}
