/*
 * Exercise 5-17.
 *
 * Add a field-handling capability, so sorting may
 * be done on fields within lines, each field sorted
 * according to an independent set of options. (The
 * index for this book was sorted with -df for the
 * index category and -n for the page numbers.)
*/

/*
  for clarity, here's a great description of what
  the goal of the exercise is:
  https://stackoverflow.com/a/1185870
  most importantly, this bit which explains the
  part that didn't make sense to me:

  > Note that when sorting, the primary criterion is the first key
  > field specified. When two rows are compared, if there is a
  > difference between the first key field in the two rows, then
  > the subsequent key fields are never considered. When two rows
  > are the same in the first key field, then the criterion for the
  > second key field determines the relative order; then, if the
  > second key fields are the same, the third key field is consulted,
  > and so on. If there are no more key fields specified, then the
  > usual default sort criterion is "the whole line of input in
  > ascending string order". A stable sort preserves the relative
  > order of two rows in the original data that are the same when
  > compared using the key field criteria (instead of using the
  > default, whole-line comparison).

  the input:
  apple      11
  banana     10
  pear       3
  pineapple  7
  pear       3
  apple      10

  would sort into this with the -1df -2n:
  apple      10
  apple      11
  banana     10
  pear       3
  pear       3
  pineapple  7

  but sort into this with -2n -1df:
  pear       3
  pear       3
  pineapple  7
  apple      10
  banana     10
  apple      11

*/

// i think things are working as intended... i think...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINES 5000
#define MAXFIELDS 12

#define USAGE "usage: %s [-\33[1mi\33[0m[n[r] | d[fr] | f[dr] | r ...] [-h]" \
	"\n\twhere \33[1mi\33[0m is a digit specifying the desired field.\n"

#define EXAMPLE "example: %s -2n -1df\n\tsorts the input lines first using " \
	"number comparison\n\ton the second column, and falls back to using " \
	"folded\n\tcase directory comparison on the first column.\n"

#define MUTEX "error: mutually exclusive arguments: -n %s\n"

// array of MAXLINES arrays of MAXFIELDS char pointers
// [["l0f0", "l0f1", "l0f2"], ["l1f0", "l1f1", "l1f2"]]
char *lineptr[MAXLINES][MAXFIELDS];

// lol there's gotta be a better way, but whatever
int fold;

typedef struct {
	int field;
	int numeric;
	int reverse;
	int fold;
	int directory;
	int string;
	int (*comp)(const char *, const char *);
} Options;

Options *options[MAXFIELDS + 1];
Options default_opt;

int readfields(char *lineptr[MAXLINES][MAXFIELDS], int nlines);
void writefields(char *lineptr[MAXLINES][MAXFIELDS], int nlines);
int numcmp(const char *, const char *);
int strcmpfold(const char *, const char *);
int dircmp(const char *, const char *);
void opt_init(Options*, int, int, int, int, int, int);
void qsort_(void *lineptr[MAXLINES][MAXFIELDS], int left, int right, Options opt);

/* sort input lines */
int main(int argc, char *argv[]) {
	int nlines;          // number of input lines read
	int i = 1;
	char *fn = *argv;

	if (argc > 1 && argc > MAXFIELDS) {
		printf("error: too many fields (> %d)\n", MAXFIELDS);
		return 1;
	}

	opt_init(&default_opt, 1, 0, 0, 0, 0, 1);
	options[0] = &default_opt;

	// handle any order of arguments
	while (*(++argv) != NULL) {
		if (**argv == '-') {
			++(*argv);
			int field = 1;
			if (isdigit(**argv)) {
				field = strtol(*argv, NULL, 10);
				++(*argv);
			}

			Options *new = malloc(sizeof(Options));
			opt_init(new,field,0,0,0,0,1);

			while (**argv != '\0') {
				switch (**argv) {
					case 'n':
						new->numeric = 1;
						new->string = 0;
						break;
					case 'r':
						new->reverse = 1;
						break;
					case 'f':
						new->fold = 1;
						break;
					case 'd':
						new->directory = 1;
						new->string = 0;
						break;
					case 'h':
						printf(USAGE "\n" EXAMPLE, fn, fn);
						return 0;
					default: {
						printf("error: unknown argument %c\n", **argv);
						printf(USAGE, fn);
						return 1;
					}
				}
				++(*argv);
			}

			if (new->numeric && (new->fold || new->directory)) {
				printf(MUTEX, new->directory && new->fold ? "-d -f" : new->fold ? "-f" : "-d");
				return 1;
			}

			if (new->numeric)
				new->comp = numcmp;
			else if (new->fold && !new->directory)
				new->comp = strcmpfold;
			else if (new->directory)
				new->comp = dircmp;
			else
				new->comp = strcmp;

			options[i++] = new;

		} else if (strcmp(*argv, "-h") == 0) {
			printf(USAGE "\n" EXAMPLE, fn, fn);
			return 0;
		} else {
			printf("error: unknown argument %s\n", *argv);
			return 1;
		}

	}

	int (*comp)(const char *, const char *) = NULL;

	Options opt = *(options[1] == NULL ? options[0] : options[1]);

	if ((nlines = readfields(lineptr, MAXLINES)) >= 0) {
		qsort_((void*)lineptr, 0, nlines-1, opt);
		writefields(lineptr, nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}

}

/* qsort_:  sort v[left]...v[right] into specified order */
void qsort_(void *v[MAXLINES][MAXFIELDS], int left, int right, Options opt) {
	int res;
	void swap(void *v[MAXLINES][MAXFIELDS], int, int, Options);
	if (left >= right)
		// do nothing if array contains fewer than two elements
		return;
	swap(v, left, (left + right) / 2, opt);
	int last = left;
	for (int i = left + 1; i <= right; ++i) {
		fold = opt.fold;
		res = (*opt.comp)(v[i][opt.field-1], v[left][opt.field-1]);
		int f = opt.field;  // index into options
		while (!res) {
			// if the elements are still equal after using default function
			if (!f) {
				break;
			}
			// no next field option specified, use default
			else if (options[f] == NULL) {
				f = 0;
				continue;
			}
			else {
				fold = options[f]->fold;
				int (*comp)(const char *, const char *) = options[f]->comp;
				res = (comp)(v[i][options[f]->field-1], v[left][options[f]->field-1]);
			}
			if (f)
				++f;
		}

		if ((res < 0 && !opt.reverse) || (res > 0 && opt.reverse))
			swap(v, ++last, i, opt);
	}
	swap(v, left, last, opt);
	qsort_(v, left, last-1, opt);
	qsort_(v, left+1, right, opt);
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

/* dircmp:  compare letters, numbers and blanks from s1 and s2 */
int dircmp(const char *s1, const char *s2) {
	char a, b;
	while (1) {
		a = *s1++;
		b = *s2++;
		while (a && !(isalnum(a) || a == ' '))
			a = *(s1++);
		while (b && !(isalnum(b) || b == ' '))
			b = *(s2++);
		// super lazy
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
void swap(void *v[MAXLINES][MAXFIELDS], int i, int j, Options opt) {
	char *temp[MAXFIELDS];
	for (int n = 0; n < MAXFIELDS; ++n) {
		temp[n] = v[i][n];
		v[i][n] = v[j][n];
		v[j][n] = temp[n];
	}
}

/* readfields:  read input lines as tab delimited fields */
int readfields(char *lineptr[MAXLINES][MAXFIELDS], int maxlines) {
	int nlines = 0;
	int i = 0;
	char c = 0;
	int field = 0;
	size_t len = 0;
	ssize_t read = 0;
	char *line = NULL;   // string allocated by getline
	char *sub = NULL;    // sub-string of line containing next field

	while ((read = getline(&line, &len, stdin)) != EOF) {
		if (nlines >= maxlines)
			return -1;
		else {
			if (line[read-1] == '\n')
				line[read-1] = '\0';
			sub = strtok(line, "\t");
			do {
				lineptr[nlines][field] = malloc(sizeof(strlen(sub) + 1));
				strcpy(lineptr[nlines][field], sub);
				field++;
			} while ((sub = strtok(NULL, "\t")) != NULL);
			nlines++;
			field = 0;
		}
	}
	return nlines;
}

/* writefields:  write outline lines as tab delimited fields */
void writefields(char *lineptr[MAXLINES][MAXFIELDS], int nlines) {
	int n = 0;
	while (n < nlines) {
		int i = 0;
		while (lineptr[n][i] != NULL)
			printf("%s\t", lineptr[n][i++]);
		printf("\n");
		++n;
	}
}

/* opt_init:  initialize new option structures */
void opt_init(Options *opt, int field, int num, int rev, int fold, int dir, int str) {
	opt->field = field;
	opt->numeric = num;
	opt->reverse = rev;
	opt->fold = fold;
	opt->directory = dir;
	opt->string = str;
	opt->comp = strcmp;
}
