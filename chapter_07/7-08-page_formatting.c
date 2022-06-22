/*
 * Exercise 7-8.
 *
 * Write a program to print a set of files, starting each new one on a
 * new page, with a title and a running page count for each file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <ctype.h>

#define WRAP 79

int fn2title(char *);

int main(int argc, char *argv[]) {
	char *fn = basename(*argv++);
	if (argc == 1) {
		printf("%s: error: expected at least 1 positional argument\n", fn);
		printf("usage: %s FILE[...]\n", fn);
		exit(1);
	}
	int c, pageno = 1;

	while (--argc) {
		char *title = *argv;
		FILE *fp = fopen(*argv, "r");
		++argv;
		if (fp == NULL) {
			printf("%s: error: failed to open file %s\n", fn, title);
			exit(1);
		}
		int len = fn2title(title);
		printf("%s%*d\n", title, WRAP - len, pageno);
		for (int i = 0; i < WRAP; ++i)
			putchar('-');
		printf("\n\n");
		while ((c = getc(fp)) != EOF)
			putchar(c);
		putchar('\n');
		if (argc - 1)
			putchar('\n');
		++pageno;
	}
	return 0;
}

/*
 * Function:  fn2title
 * -------------------
 * converts a system filename (fn) to a page title in place
 *   - strips filesystem path before the basename
 *   - replaces underscores with spaces
 *   - converts the start of words to uppercase
 *   - strips file extension (first occurance of '.' replaced with '\0')
 *
 * example: "this_is_a_page_title_example.txt" ->
 *          "This Is A Page Title Example"
 *
 * returns: the length of the new string (num of chars before '\0')
*/
int fn2title(char *fn) {
	if (fn == NULL)
		return 0;
	strcpy(fn, basename(fn));
	char *start = fn;
	int last_space = 0;
	while (*fn) {
		if (*fn == '_' || *fn == ' ') {
			*fn = ' ';
			last_space = 1;
		}
		else if (last_space || fn == start) {
			*fn = toupper(*fn);
			last_space = 0;
		}
		else if (*fn == '.') {
			*fn = '\0';
			break;
		}
		++fn;
	}
	return fn - start;
}
