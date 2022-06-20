/*
 * Exercise 7-2.
 *
 * Write a program that will print arbitrary input in a
 * sensible way. As a minimum, it should print non-graphic
 * characters in octal or hexadecimal according to local
 * custom, and break long text lines.
*/

// see below for table with char ranges for ctype.h functions
// https://en.cppreference.com/w/c/string/byte/isprint

#include <stdio.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#define ERR_INVALID_LEN "error: invalid max line length provided to -n: \"%ld\"\n"
#define ERR_INVALID_BASE "error: invalid base provided to -b: \"%s\"\n" \
	"\texpected x (hex), o (oct), d (dec), or i (dec)\n"

void printhelp(char *s);
char validatebase(char *s);

int main(int argc, char *argv[]) {
	int opt;
	long max = 79;
	char *endstr = NULL;
	char *fmt = "\\%c%%%c";
	char buf[5];
	char res = 0;

	while ((opt = getopt(argc, argv, "n:b:h")) != -1) {
		switch (opt) {
			case 'n':
				max = strtol(optarg, &endstr, 10);
				break;
			case 'b':
				res = validatebase(optarg);
				if (res == -1)
					return 1;
				sprintf(buf, fmt, res, res);
				break;
			case 'h':
				printhelp(basename(argv[0]));
				return 0;
		}
	}

	if (res == 0)
		sprintf(buf, fmt, 'x', 'x');

	if ((endstr && *endstr != 0) || max == 0) {
		printf(ERR_INVALID_LEN, max);
		return 1;
	}

	long len = 0;
	char c;
	while ((c = getchar()) != EOF) {
		if (len >= max && c != '\n') {
			putchar('\n');
			len = 0;
		}
		if (isspace(c) || isprint(c))
			putchar(c);
		else
			printf(buf, c);
		if (c == '\n')
			len = 0;
		else
			++len;
	}
}

/* printhelp:  print argument usage information */
void printhelp(char *s) {
	printf("usage: %s [-n LEN -b BASE -h]\n", s);
	printf("\t-h         prints this help message\n");
	printf("\t-n LEN     max line length before newline\n");
	printf("\t-b BASE    base to represent non-printable characters (o | x | d | i)\n");
}

/* validatebase:  validate the parameter given to -b */
char validatebase(char *s) {
	char c = tolower(s[0]);
	if (s[1] != '\0') {
		printf(ERR_INVALID_BASE, s);
		return -1;
	}
	switch (c) {
		case 'x':
		case 'o':
		case 'd':
		case 'i':
			break;
		default:
			printf(ERR_INVALID_BASE, s);
			return -1;
	}
	return c;
}
