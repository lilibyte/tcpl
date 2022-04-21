/*
 * Exercise 5-14.
 *
 * Modify the sort program to handle a -r flag, which
 * indicates sorting in reverse (decreasing) order. Be
 * sure that -r works with -n.
*/

/*
    K&R book code generates this warning:

    Pointer type mismatch ('int (*)(char *, char *)' and
    'int (*)(const char *, const char *)')

    on the line:

    qsort_((void **) lineptr, 0, nlines-1,
        (int (*)(void*,void*))(numeric ? numcmp : strcmp));

    because numcmp and strcmp have different pointer types.

    To get around this I just changed the numcmp prototype to

    int numcmp(const char *, const char *);

    and changed the remaining references to the function to
    char* instead of void*

    https://cboard.cprogramming.com/c-programming/168913-pointers-functions-k-r-warning-iso-c-forbids-conversion-object-pointer.html
    https://stackoverflow.com/q/70665273/
    https://stackoverflow.com/q/35653392/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];

int reverse = 0;     // 1 if reverse sort

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int numcmp(const char *, const char *);

// int (*comp)(char *, char *)
// comp is a pointer to a function that has two char * arguments
// and returns an int.
void qsort_(void *lineptr[], int left, int right, int (*comp)(char *, char *));

/* sort input lines */
int main(int argc, char *argv[]) {
	int nlines;          // number of input lines read
	int numeric = 0;     // 1 if numeric sort

	// handle any order of arguments
	if (argc > 1) {
		while (*++argv != NULL) {
			if (strcmp(*argv, "-n") == 0)
				numeric = 1;
			else if (strcmp(*argv, "-r") == 0)
				reverse = 1;
		}
	}

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort_((void **) lineptr, 0, nlines-1,
			(int (*)(char*,char*))(numeric ? numcmp : strcmp));
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}

/* qsort_:  sort v[left]...v[right] into specified order */
void qsort_(void *v[], int left, int right, int (*comp)(char *, char *)) {
	int res;
	void swap(void *v[], int, int);
	if (left >= right)
		// do nothing if array contains fewer than two elements
		return;
	swap(v, left, (left + right) / 2);
	int last = left;

	// exercise 5-14 code here
	for (int i = left + 1; i <= right; ++i) {
		res = (*comp)(v[i], v[left]);
		if ((res < 0 && !reverse) || (res > 0 && reverse))
			swap(v, ++last, i);
	}

	swap(v, left, last);
	qsort_(v, left, last-1, comp);
	qsort_(v, left+1, right, comp);
}

/* numcmp:  compare s1 and s2 numerically */
int numcmp(const char *s1, const char *s2) {
	double v1 = atof(s1);
	double v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

/* swap:  exchange two pointers */
void swap(void *v[], int i, int j) {
	void *temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

/* readlines:  read input lines */
int readlines(char *lineptr[], int maxlines) {
	int nlines = 0;
	size_t len = 0;
	ssize_t read = 0;

	while ((read = getline(&lineptr[nlines], &len, stdin)) != EOF)
		if (nlines >= maxlines)
			return -1;
		else {
			if (lineptr[nlines][read-1] == '\n')
				lineptr[nlines][read-1] = '\0';
			nlines++;
		}
	return nlines;
}

/* writelines:  write outline lines */
void writelines(char *lineptr[], int nlines) {
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}
