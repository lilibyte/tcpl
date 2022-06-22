/*
 * Exercise 7-7.
 *
 * Modify the pattern finding program of Chapter 5 to take
 * its input from a set of named files or, if no files are
 * named as arguments, from the standard-input. Should the
 * file name be printed when a matching line is found?
*/

// See K&R2 page 117 for the original `find` example program.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdbool.h>

#define MAXLINE 1000
#define MAXFILES 10

#define USAGE "usage: %s [-x -n] pattern [files ...]\n"

int main(int argc, char *argv[]) {
	char *fn = basename(argv++[0]);

	if (argc < 3) {
		printf("%s: error: expected at least 3 arguments\n", fn);
		printf(USAGE, fn);
		return 1;
	}

	int c, found = 0, f = 0;
	bool except = false, number = false;
	char line[MAXLINE];
	FILE *files[MAXFILES];
	char *filenames[MAXFILES];
	char *pattern = NULL;

	while (--argc > 0) {
		if ((*argv)[0] == '-') {
			while ((c = *++(argv[0])))
				switch (c) {
				case 'x':
					except = true;
					break;
				case 'n':
					number = true;
					break;
				default:
					printf("%s: error: illegal option %c\n", fn, c);
					printf(USAGE, fn);
					exit(1);
				}
		}
		else if (pattern == NULL)
			pattern = *argv;
		else if (f < MAXFILES) {
			files[f] = fopen(*argv, "r");
			filenames[f] = *argv;
			if (files[f] == NULL) {
				printf("%s: error: failed to open file %s\n", fn, filenames[f]);
				exit(1);
			}
			++f;
		}
		++argv;
	}

	if (pattern == NULL) {
		printf("%s: error: expected a pattern positional argument\n", fn);
		printf(USAGE, fn);
		exit(1);
	}

	if (f == 0) {
		files[f] = stdin;
		filenames[f] = "stdin";
		++f;
	}

	for (int i = 0; i < f; ++i) {
		long lineno = 0;
		while (fgets(line, MAXLINE, files[i]) != NULL) {
			++lineno;
			if ((strstr(line, pattern) != NULL) != except) {
				if (number)
					printf("%ld:", lineno);
				printf("%s:%s", filenames[i], line);
				++found;
			}
		}
	}
	return found;
}
