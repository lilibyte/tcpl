/*
 * Exercise 5-13.
 *
 * Write the program tail, which prints the last n lines of
 * its input. By default, n is 10, let us say, but it can be
 * changed by an optional argument, so that
 *
 *     tail -n
 *
 * prints the last n lines. The program should behave rationally
 * no matter how unreasonable the input or the value of n. Write
 * the program so it makes the best use of available storage;
 * lines should be stored as in the sorting program of Section
 * 5.6, not in a two-dimensional array of fixed size.
*/

#include <stdio.h>
#include <stdlib.h>

#define N_LINES 10
#define MAX_LINES 5000

int main(int argc, char **argv) {
	int n = 0;
	if (*++argv && **argv == '-')
		n = abs(atoi(*(argv)+1));
	if (!n)
		n = N_LINES;
	char *lines[MAX_LINES];
	char **lineptr = lines;
	size_t len = 0;
	int i = 0;
	while (getline(lineptr, &len, stdin) != EOF) {
		if (i >= n)
			free(lines[i-n]);
		++lineptr;
		++i;
	}
	lineptr -= (i > n) ? n : i;
	while (*(lineptr+1))
		printf("%s", *(lineptr++));
}
