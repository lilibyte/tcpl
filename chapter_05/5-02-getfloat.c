/*
 * Exercise 5-2.
 *
 * Write getfloat, the floating-point analog of getint.
 * What type does getfloat return as its function value?
*/

// should return int just like getfloat because it's
// returning a status code not the float value

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

/* getfloat:  get next floating-point integer from input into *pn */
int getfloat(float *pn) {
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
	if (c == '.') {
		c = getch();
		for (float s = 0.1; isdigit(c) && s; c = getch()) {
			*pn += s * (c - '0');
			s /= 10;
		}
	}
	*pn *= sign;
	if (c != EOF)
		ungetch(c);
	return c;
}

int main() {
	float a = 10.5;
	getfloat(&a);
	printf("%.8g\n", a);
	// a in this example should be 10.5 if given an incorrect input e.g. -xyz
	// and otherwise be equal to the input given converted to int
}
