/*
 * Exercise 5-12.
 *
 * Extend entab and detab to accept the shorthand
 *
 *     entab -m +n
 *
 * to mean tab stops every n columns, starting at column m.
 * Choose convenient (for the user) default behavior.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int m = 8;
	int n = 4;

	while (*++argv) {
		if (**argv == '-')
			m = abs(atoi(*(argv)+1));
		else if (**argv == '+')
			n = abs(atoi(*(argv)+1));
	}

	for (int j = 0; j < m; ++j)
		printf((j == m - 1) ? "+" : "-");
	for (int j = 1; j < 80-m; ++j)
		printf(!(j % n) ? "+" : "-");
	printf("\n");

	char c;
	int i = 0;
	int t = m;
	int spaces = 0;
	int total = 0;
	while ((c = getchar()) != EOF) {
		if (total == t) {
			t += n;
			i = !!i;
			if (spaces)
				printf("\\t");
			spaces = 0;
		}
		if (c == ' ') {
			++spaces;
		}
		else if (c == '\n') {
			spaces = 0;
			i = 0;
			total = 0;
			t = m;
			printf("\n");
			continue;
		}
		else {
			for (; spaces; --spaces)
				printf("_");
			++i;
			printf("%c", c);
		}
		++total;
	}
	if (c != '\n')
		printf("\n");
}
