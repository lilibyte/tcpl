/*
 * Exercise 6-1.
 *
 * Our version of getword does not properly handle underscores,
 * string constants, comments, or preprocessor control lines.
 * Write a better version.
*/

// took me a bit to realize that binsearch needed pre-sorted input
// and thus to sort the initializers according to their ASCII values

// program assumes good input; it's not a C syntax parsing exercise

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof keytab[0])

struct key {
	char *word;
	int count;
} keytab[] = {
	{ "_Bool", 0 },
	{ "auto", 0 },
	{ "bool", 0 },
	{ "break", 0 },
	{ "case", 0 },
	{ "char", 0 },
	{ "const", 0 },
	{ "continue", 0 },
	{ "default", 0 },
	{ "unsigned", 0 },
	{ "void", 0 },
	{ "volatile", 0 },
	{ "while", 0 },
};

int getword(char *, int);
int binsearch(char *, struct key *, int);
int getch(void);
void ungetch(int);

char buf;

/* count C keywords */
int main() {
	int n;
	char word[MAXWORD];

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]) || word[0] == '_')
			if ((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
	for (n = 0; n < NKEYS; ++n)
		if (keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);
}

/* binsearch:  find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n) {
	int cond = 0, low = 0, mid = 0;
	int high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

/* getch:  get a (possibly pushed back) character */
int getch(void) {
	if (buf != '\0') {
		char tmp = buf;
		buf = '\0';
		return tmp;
	} else
		return getchar();
}

/* ungetch:  push character back on input */
void ungetch(int c) {
	buf = c;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim) {
	int c;
	char *w = word;

	while (isspace(c = getch()))
		;
	if (c != EOF)
		*w++ = c;
	if (c  == '/') {
		c = getch();
		if (c == '/')
			while ((c = getch()) != EOF && c != '\n')
				;
		else if (c == '*') {
			while ((c = getch()) != EOF)
				if (c == '*' && (c = getch()) == '/')
					break;
		} else {
			ungetch(c);
			c = '/';
		}
	}
	else if (c == '"') {
		int p = c;
		while ((c = getch()) != EOF) {
			if (c == '"' && p != '\\')
				break;
			p = c;
		}
	}
	else if (c == '\'') {
		c = getch();
		if (c == '\\')
			c = getch();
		c = getch();
	}
	else if (c == '#') {
		while ((c = getch()) != EOF && c != '\n')
			;
	}
	if (!isalpha(c) && c != '_') {
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; ++w) {
		*w = getch();
		if (!isalnum(*w) && *w != '_') {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return word[0];
}
