/*
 * Exercise 7-1.
 *
 * Write a program that converts upper case to lower or
 * lower case to upper, depending on the name it is invoked with,
 * as found in argv[0].
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

// POSIX.1-2001
#include <libgen.h>

#define ERR "error: expected program name to be \"upper\" " \
	"or \"lower\"; received \"%s\"\n"

int main(int argc, char *argv[]) {
	char c;
	int (*togglecase)();
	char *base = basename(argv[0]);
	if (strcmp(base, "lower") == 0)
		togglecase = tolower;
	else if (strcmp(base, "upper") == 0)
		togglecase = toupper;
	else {
		printf(ERR, base);
		return 1;
	}
	while ((c = getchar()) != EOF)
		putchar(togglecase(c));
	return 0;
}
