/*
 * Exercise 5-7.
 *
 * Rewrite readlines to store lines in an array supplied
 * by main, rather than calling alloc to maintain storage.
 * How much faster is the program?
*/

// "If *lineptr is set to NULL before the call, then
// getline() will allocate a buffer for storing the line."

// idk if this is actually doing anything different,
// the GNU getline() might just be doing the malloc
// instead i guess. might come back to this one later.

// doing anything else would feel like an anti-pattern,
// and this still avoids the extra strcpy() call that's
// in the book's example

#include <stdio.h>
#include <string.h>
#include <string.h>

#define MAXLINES 5000

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);
void swap(char *v[], int i, int j);

/* swap:  interchange v[i] and v[j] */
void swap(char *v[], int i, int j) {
	char *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

/* qsort:  sort v[left]...v[right] into increasing order */
void qsort(char *v[], int left, int right) {
	if (left >= right)  // do nothing if array contains < 2 elements
		return;
	swap(v, left, (left + right) / 2);
	int last = left;
	for (int i = left + 1; i <= right; ++i) {
		if (strcmp(v[i], v[left]) < 0)
			swap(v, ++last, i);
	}
	swap(v, left, last);
	qsort(v, left, last-1);
	qsort(v, last+1, right);
}

/* readlines:  read input lines */
int readlines(char *lineptr[], int maxlines) {
	int nlines = 0;
	size_t len = 0;
	ssize_t read = 0;

	while ((read = getline(&lineptr[nlines], &len, stdin)) != EOF) {
		if (nlines >= maxlines)
			return -1;
		else {
			if (lineptr[nlines][read-1] == '\n')
				lineptr[nlines][read-1] = '\0';
			nlines++;
		}
	}
	return nlines;
}

/* writelines:  write output lines */
void writelines(char *lineptr[], int nlines) {
	for (int i = 0; i < nlines; ++i)
		printf("%s\n", lineptr[i]);
}

/* sort input lines */
int main() {
	char *lines[MAXLINES];
	int nlines;
	if ((nlines = readlines(lines, MAXLINES)) >= 0) {
		qsort(lines, 0, nlines-1);
		writelines(lines, nlines);
		return 0;
	} else {
		printf("error: input too big");
		return 1;
	}
}
