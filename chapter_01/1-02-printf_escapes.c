/*
 * Exercise 1-2.
 * Experiment to find out what happens when `printf`'s argument
 * string contains `\c`, where `c` is some character not listed above.
 * (\n, \t, \b, \", \\)
*/

#include <stdio.h>

int main() {
  /* Warning: Unknown escape sequence '\g' */
  printf("\g");
}

/*
 * man://printf(1)
 *
 * \"          double quote
 * \\          backslash
 * \a          alert (BEL)
 * \b          backspace
 * \c          produce no further output
 * \e          escape
 * \f          form feed
 * \n          new line
 * \r          carriage return
 * \t          horizontal tab
 * \v          vertical tab
 * \NNN        byte with octal value NNN (1 to 3 digits)
 * \xHH        byte with hexadecimal value HH (1 to 2 digits)
 * \uHHHH      Unicode (ISO/IEC 10646) character with hex value HHHH
 *             (4 digits)
 * \UHHHHHHHH  Unicode character with hex value HHHHHHHH (8 digits)
 * %%          a single %
 * %b          ARGUMENT as a string with '\' escapes interpreted,
 *             except that octal escapes are of the form \0 or \0NNN
 * %q          ARGUMENT is printed in a format that can be reused as
 *             shell input, escaping non-printable characters with the
 *             proposed POSIX $'' syntax.
*/

