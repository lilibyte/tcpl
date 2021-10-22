#include <stdio.h>
#include <stdbool.h>

/*
 * Exercise 1-10.
 * Write a program to copy its input to its output, replacing each
 * tab by \t, each backspace by \b, and each backslash by \\.
 * This makes tabs and backspaces visible in an unambigious way.
 */

int main() {
  int c = 0;

  while ((c = getchar()) != EOF) {
    if (c == '\t')
      printf("\\t");
    else if (c == 127 || c == '\b')
      printf("\\b");
    else if (c == '\\')
      printf("\\\\");
    else
      putchar(c);
  }
}

/*
 * I had to research this one a bit, and based on the search results
 * a lot of people end up confused doing this exact exercise.
 *
 * From my current understanding, POSIX specifies two methods of processing
 * input: canonical mode (usually default) which works on lines delimited by
 * a \n, EOF, or EOL, and non-canonical mode which works on single bytes at a
 * time and does not process backspaces or kill signals.
 *
 * Thus, for `getchar()` to receive a backspace character from keyboard input,
 * the terminal environment would need to be changed to specify that input
 * should be unbuffered. More conveniently, I used `stty` to change the
 * character used for erasing to NULL, which causes the delete character from
 * my keyboard (ASCII 127) to be handled literally.
 *
 * `stty erase 0 ; ./01-10.c ; stty ek`
 *
 * Since the program could still receive a backspace character (\b) from some
 * other stdin source like piping: `echo -e 'hello\b' | ./1-10.c`, I handle
 * the ASCII code for it as well `c == 127 || c == '\b'` even if it doesn't
 * change anything in this context.
 *
 * https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap11.html#tag_11_01_06
 * https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html
 * https://www.linusakesson.net/programming/tty/
*/
