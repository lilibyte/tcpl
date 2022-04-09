/*
 * Exercise 5-6.
 *
 * Rewrite appropriate programs from earlier chapters and exercises
 * with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and
 * getop (Chapter 4).
*/

// i'll redo the listed ones but i don't want to go through more
// than that.

#include <stdio.h>
#include <limits.h>
#include <string.h>

void getline_(char *line) {
	char c;
	while ((c = getchar()) != EOF && c != '\n')
		*(line++) = c;
	if (c == '\n')
		*(line++) = c;
	*line = '\0';
}

void reverse(char *s) {
	char *z = s;
	char tmp = *s;
	while (*z != '\0')
		z++;
	--z;
	while (z - s) {
		tmp = *s;
		*(s++) = *z;
		*(z--) = tmp;
	}
}

void itoa(int n, char *s) {
	int sign;
	int overflow = n == INT_MIN;
	char *tmp = s;
	if (overflow)
		++n;
	if ((sign = n) < 0)
		n = -n;
	do {
		*(s++) = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		*(s++) = '-';
	*s = '\0';
	if (overflow)
		++(*tmp);
	reverse(s - (s-tmp));
}

int atoi_(char *s) {
	int r = 0;
	int mul = 1;
	char *tmp = s;
	while (*(s+1)) s++;
	while (s - tmp >= 0) {
		r += (*(s--) - '0') * mul;
		mul *= 10;
	}
	return r;
}

size_t strrindex(char *s, char *t) {
	size_t i = 0;
	size_t potential = -1;
	size_t found = -1;
	char *tmp = t;
	while (*s) {
		if (*(s++) == *(t++)) {
			if (potential == -1)
				potential = i;
			if (!(*t)) {
				found = potential;
				t = tmp;
			}
		} else {
			t = tmp;
			potential = -1;
		}
		++i;
	}
	return found;
}

int main() {
	char line[1000] = "hello";
	getline_(line+5);
	printf("%s", line);

	char c[15];
	itoa(INT_MIN+1, c);
	printf("%s\n", c);
	itoa(INT_MIN, c);
	printf("%s\n", c);

	char a[] = "123456789";
	int r = atoi_(a);
	printf("%d\n", r);

	char str[100] = "hello hello world";
	printf("%zd\n", strrindex(str, "hello"));
}
