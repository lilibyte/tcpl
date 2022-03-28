/*
 * Exercise 5-1.
 *
 * As written, getint treats a + or - not followed by a
 * digit as a valid representation of zero. Fix it to push
 * such a character back on the input.
*/

#include <ctype.h>
#include <stdio.h>

#define BUFSIZE 100     // ungetch buffer size

/* taken from exercise 4-04 */
char buf[BUFSIZE];      // buffer for ungetch
int bufp = 0;           // next free position in buf
int getch(void);
void ungetch(int);

/* getch:  get a (possibly pushed back) character */
int getch(void) {
	return (bufp > 0) ? buf[--bufp] : getchar();
}

/* ungetch:  push charater back on input stack */
void ungetch(int c) {
	if (bufp >= BUFSIZE)
		printf("ungetch: too many charactern\n");
	else
		buf[bufp++] = c;
}

/* getint:  get next integer from input into *pn */
int getint(int *pn) {
	int c = 0, sign = 1;
	while (isspace(c = getch()))
		;
	if (c == '-' || c == '+') {
		if (c == '-')
			sign = -1;
		c = getch();
	}
	if (!isdigit(c) && c != EOF) {
		ungetch(c);
		return 0;
	}
	for (*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');
	*pn *= sign;
	if (c != EOF)
		ungetch(c);
	return c;
}

int main() {
	int a = 10;
	getint(&a);
	printf("%d\n", a);
	// a in this example should be 10 if given an incorrect input e.g. -xyz
	// and otherwise be equal to the input given converted to int
}
