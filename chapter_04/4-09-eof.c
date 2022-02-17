/*
 * Exercise 4-9.
 *
 * Our getch and ungetch do not handle a pushed-back EOF
 * correctly. Decide what their properties ought to be if
 * an EOF is pushed back, then implement your design.
*/

// buf needs to be an int because an unsigned char array
// cant hold -1 (EOF) through 255 (CHAR_MAX)

#define BUFSIZE 100

int buf[BUFSIZE];
