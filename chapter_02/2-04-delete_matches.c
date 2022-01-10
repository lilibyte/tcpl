/*
 * Exercise 2-4.
 *
 * Write an alternate version of squeeze(s1, s2)
 * that deletes each character in s1 that matches
 * any character in the string s2.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void squeeze(unsigned char s1[], unsigned char s2[]) {
  bool chars[256] = { false };
  for (int i = 0; s2[i] != '\0'; ++i)
    chars[s2[i]] = true;

  int j;
  for (int i = j = 0; s1[i] != '\0'; ++i)
    if (!(chars[s1[i]]))
      s1[j++] = s1[i];

  s1[j] = '\0';
}

int main() {
  unsigned char s1[] = "reiayanami";
  unsigned char s2[] = "borgerqueen";
  squeeze(s1, s2);
  printf("reiayanami - borgerqueen == iayaami (");
  puts((strcmp("iayaami", (char*)s1)) ? "false)\n" : "true)");

  unsigned char s3[] = "asuka";
  unsigned char s4[] = "noborger";
  squeeze(s3, s4);
  printf("asuka - noborger == asuka (");
  puts((strcmp("asuka", (char*)s3)) ? "false)\n" : "true)");
}
