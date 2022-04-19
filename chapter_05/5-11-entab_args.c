/*
 * Exercise 5-11.
 *
 * Modify the programs entab and detab (written as exercises
 * in Chapter 1) to accept a list of tab stops as arguments.
 * Use the default tab settings if there are no arguments.
*/

/*
 * Exercise 1-21.
 * Write a program `entab` that replaces strings of blanks by the
 * minimum number of tabs and blanks to achieve the same spacing.
 * Use the same tab stops as for `detab`. When either a tab or
 * single blank would suffice to reach a tab stop, which should be
 * given preference?
*/

/*
	just like chapter 1, this one confuses me. luckily someone else
	has already asked online for an explanation:

	> In the olden days, when K&R was written, typewriters had `tab
	> stops` which configured how far the paper would move left each
	> time that the `TAB` key was pressed. So a list of `tab stops`
	> is a list of monotonically increasing column numbers, such as
	> `6,16,32,40,72,80`
	https://stackoverflow.com/q/21174032

	> entab needs to work out when runs of spaces reach a topstop -
	> then that run of spaces can be replaced by a tab character.
	> For example, the following line of text (the ruller is there
	> for reference):
	>            1   1   2   2   2
	> 1      8   2   6   0   4   8
	> -------+---+---+---+---+---+
	>           this     is a line
	> should look like the following
	> \t  this \tis a line
	https://stackoverflow.com/a/2649817

	uhh i could be wrong but i think that one above should be
	> \t__this____is_a_line
	because i thought hitting tab took you to one past the
	tab stop? so for example
	> -------+---+---+---+---+---+
	>           this  is  a   line
	would be
	> \t__this\tis\ta\tline

	at least that's what i'm doing, fuck it

*/

// prints ruler and \t and _ literals for demo purposes

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
			if (spaces == 1)
				printf("_");
			else if (spaces)
				printf("\\t");
			spaces = 0;
		}
		if (c == ' ') {
			++spaces;
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
			if (spaces + i <= t)
				for (int j = spaces; j; --j)
					printf("_");
			spaces = 0;
			printf("%c", c);
		}
		++total;
	}
	if (c != '\n')
		printf("\n");
}
