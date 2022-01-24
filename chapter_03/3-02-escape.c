/*
 * Exercise 3-2.
 *
 * Write a function escape(s,t) that converts characters
 * like newline and tab into visible escape sequences like
 * \n and \t as it copies the string t to s. Use a switch.
 * Write a function for the other direction as well,
 * converting escape sequences into the real characters.
*/

#include <stdio.h>

// i know there are more escape sequences than i handle,
// but i think this still captures the spirit of the exercise

void escape(char s[], char t[]) {
  int i = 0;
  int diff = 0;
  char c;
  while ((c = t[i]) != '\0') {
    if (c == '\\') {
      switch(t[i+1]) {
        case 'n': {
          s[i-diff] = '\n';
          break;
        }
        case 't': {
          s[i-diff] = '\t';
          break;
        }
      }
    i += 2;
    ++diff;
    } else {
      s[i-diff] = c;
      ++i;
    }
  }
  s[i-diff] = '\0';
}

void unescape(char s[], char t[]) {
  int i = 0;
  int diff = 0;
  char c;
  while ((c = t[i]) != '\0') {
    switch (c) {
      case '\n': {
        s[i+diff] = '\\';
        s[i+diff+1] = 'n';
        ++diff;
        break;
      }
      case '\t': {
        s[i+diff] = '\\';
        s[i+diff+1] = 't';
        ++diff;
        break;
      }
      default: {
        s[i+diff] = c;
        break;
      }
    }
    ++i;
  }
  s[i+diff] = '\0';
}

int main() {
  char t1[] = "11\t12\t13\n21\t22\t23\n31\t32\t33";
  char s1[52];

  unescape(s1, t1);
  printf("%s\n", s1);

  char t2[] = "11\\t12\\t13\\n21\\t22\\t23\\n31\\t32\\t33";
  char s2[52];

  escape(s2, t2);
  printf("%s\n", s2);
}
