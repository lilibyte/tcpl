/*
 * Exercise 1-11.
 * How would you test the word count program? What kinds of input
 * are most likely to uncover bugs if there are any?
*/

/*
 * I don't know what this exercise is asking :')
 *
 * If I were writing unit tests as I would in Python I'd write
 * assertions for sample inputs that have known correct results
 * and check against those. e.g. (pseudo-code):
 * assertLineCount("hello\nworld\n", 2)
 * assertWordCount("hello\nworld\n", 2)
 * assertCharCount("hello\nworld\n", 12)
 *
 * I don't know what kinds of input would uncover bugs as long as
 * they could be given to `getchar()`.
*/

#include <stdio.h>

#define IN  1   /* inside a word */
#define OUT 0   /* outside a word */

int main() {

  int c = 0, nl = 0, nw = 0, nc = 0;
  int state = OUT;   // whether inside or outside a word

  while ((c = getchar()) != EOF) {
    ++nc;
    if (c == '\n')
      ++nl;
    if (c == ' ' || c == '\n' || c == '\t')
      state = OUT;
    else if (state == OUT) {
      state = IN;
      ++nw;
    }
  }
  printf("%d %d %d\n", nl, nw, nc);
}
