/*
 * Exercise 5-16.
 *
 * Add the -d ("directory order") option, which makes
 * comparisons only on letters, numbers and blanks.
 * Make sure it works in conjunction with -f.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];

int reverse = 0;     // 1 if reverse sort
int fold = 0;        // 1 if fold alpha
int directory = 0;   // 1 if directory sort

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int numcmp(const char *, const char *);
int strcmpfold(const char *, const char *);
int dircmp(const char *, const char *);

// int (*comp)(char *, char *)
// comp is a pointer to a function that has two char * arguments
// and returns an int.
void qsort_(void *lineptr[], int left, int right, int (*comp)(const char *, const char *));

/* sort input lines */
int main(int argc, char *argv[]) {
	int nlines;          // number of input lines read
	int numeric = 0;     // 1 if numeric sort
	char c;

	// handle any order of arguments
	while (*(++argv) != NULL) {
		if (**argv == '-') {
			while (*(++(*argv)) != '\0') {
				switch (**argv) {
					case 'n':
						numeric = 1;
						break;
					case 'r':
						reverse = 1;
						break;
					case 'f':
						fold = 1;
						break;
					case 'd':
						directory = 1;
						break;
					default: {
						printf("error: unknown argument %c\n", **argv);
						return 1;
					}
				}
			}
		} else {
			printf("error: unknown argument %s\n", *argv);
			return 1;
		}
	}

	if ((numeric && fold) || (numeric && directory)) {
		printf("error: mutually exclusive arguments: -n %s\n",
				directory && fold ? "-d -f" : directory ? "-d" : "-f");
		return 1;
	}

	int (*comp)(const char *, const char *) = NULL;

	if (numeric)
		comp = numcmp;
	else if (fold && !directory)
		comp = strcmpfold;
	else if (directory)
		comp = dircmp;
	else
		comp = strcmp;

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort_((void **) lineptr, 0, nlines-1, comp);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}

/* qsort_:  sort v[left]...v[right] into specified order */
void qsort_(void *v[], int left, int right, int (*comp)(const char *, const char *)) {
	int res;
	void swap(void *v[], int, int);
	if (left >= right)
		// do nothing if array contains fewer than two elements
		return;
	swap(v, left, (left + right) / 2);
	int last = left;
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

/* strcmpfold:  compare s1 and s2, ignoring case */
int strcmpfold(const char *s1, const char *s2) {
	char a, b;
	while ((a = tolower(*s1++)) == (b = tolower(*s2++)) && a)
		;
	if (a > b)
		return 1;
	else if (a < b)
		return -1;
	else
		return 0;
}

/* dircmp:  compare letters, numbers and blanks from s1 and s2*/
int dircmp(const char *s1, const char *s2) {
	char a, b;
	while (1) {
		a = *s1++;
		b = *s2++;
		while (a && !(isalnum(a) || a == ' '))
			a = *(s1++);
		while (b && !(isalnum(b) || b == ' '))
			b = *(s2++);
		if (fold) {
			a = tolower(a);
			b = tolower(b);
		}
		if (!a && !b)
			return 0;
		else if (a == b)
			continue;
		else if (a > b)
			return 1;
		else if (a < b)
			return -1;
	}
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
