/*
 * Exercise 5-11.
 *
 * Modify the programs entab and detab (written as exercises
 * in Chapter 1) to accept a list of tab stops as arguments.
 * Use the default tab settings if there are no arguments.
*/

/*
 * Exercise 1-20.
 * Write a program `detab` that replaces tabs in the input with the
 * proper number of blanks to space to the next tab stop. Assume a
 * fixed set of tab stops, say every n columns. Should n be a variable
 * or a symbolic parameter?
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char c;
	int tabs[argc+1];
	tabs[0] = 1;
	int spaces = 0;
	int tabpos = 1;
	int total = 0;
	int default_tab = 8;
	if (argc > 1) {
		for (int j = 1; j <= argc-1; ++j) {
			tabs[j] = atoi(*(++argv));
			if (tabs[j-1] >= tabs[j]) {
				printf("\x1B[0Gerror: tab stops must increase "
						"monotonically (%d >= %d)\n", tabs[j-1], tabs[j]);
				return 1;
			}
			if (j == 1)
				putchar('-');
			for (int i = tabs[j] - tabs[j-1]; i > 0; --i)
				printf((i == 1) ? "+" : "-");
		}
		printf("\n");
	}
	int i = 0;
	while ((c = getchar()) != EOF) {
		int t = (tabpos < argc) ? tabs[tabpos] : default_tab;
		if (total == t) {
			i = !!i;
			++tabpos;
			spaces = 0;
		}
		if (c == '\t') {
			for (int j = t - (i + spaces); j; --j)
				printf("_");
			i = 0;
			spaces = 0;
		}
		else if (c == ' ') {
			++spaces;
			printf("_");
		}
		else if (c == '\n') {
			tabpos = 1;
			spaces = 0;
			i = 0;
			total = 0;
			printf("\n");
			continue;
		}
		else {
			++i;
			printf("%c", c);
		}
		++total;
	}
	if (c != '\n')
		printf("\n");
}
