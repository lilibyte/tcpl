/*
 * Exercise 7-4.
 *
 * Write a private version of scanf analogous to
 * minprintf from the previous section.
*/

// should be good enough

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

#define BUF_MAX 1024

int minscanf(char *fmt, ...);
long getchartol(char *unget);
double getchartod(char *unget);

int main() {
	char arr[24];
	int i;
	double f;
	// working input:
	// hello_world 123, 123.45
	minscanf("%s %d, %f", arr, &i, &f);
	printf("%s\n", arr);
	printf("%d\n", i);
	printf("%f\n", f);
}

long getchartol(char *unget) {
	char tmp[BUF_MAX];  // NOTE: unsafe; never bounds checked
	int i = 0;
	char c;
	while (isspace(c = getchar()))
		;
	if (isdigit(c)) {
		tmp[i++] = c;
		while (isdigit(c = getchar()))
			tmp[i++] = c;
	}
	*unget = c;
	tmp[i++] = '\0';
	return strtol(tmp, NULL, 10);
}

double getchartod(char *unget) {
	char tmp[BUF_MAX];  // NOTE: unsafe; never bounds checked
	int i = 0;
	char c;
	while (isspace(c = getchar()))
		;
	if (isdigit(c)) {
		tmp[i++] = c;
		while (isdigit(c = getchar()))
			tmp[i++] = c;
		if (c == '.') {
			tmp[i++] = c;
			while (isdigit(c = getchar()))
				tmp[i++] = c;
		}
	}
	*unget = c;
	tmp[i++] = '\0';
	return strtod(tmp, NULL);
}

int minscanf(char *fmt, ...) {
	va_list ap;
	char *p = fmt;
	int total = 0;
	char buf = '\0';
	long *iarg = NULL;
	double *darg = NULL;
	char *sarg = NULL;
	va_start(ap, fmt);
	while (*p) {
		if (buf == EOF)
			return EOF;
		while (isspace(*p))
			++p;
		if (*p == '%' || buf == '%') {
			if (buf)
				buf = '\0';
			while (isspace(*++p))
				++p;
			switch (*p++) {
			case 'd':
				iarg = va_arg(ap, long *);
				*iarg = getchartol(&buf);
				++total;
				break;
			case 'f':
				darg = va_arg(ap, double *);
				*darg = getchartod(&buf);
				++total;
				break;
			case 's':
				sarg = va_arg(ap, char *);
				char c;
				while (!isspace(c = getchar()) && c != EOF)
					*sarg++ = c;
				if (c == EOF)
					return EOF;
				break;
			}
		}
		else {
			char c;
			while (*p != '%') {
				if (buf && *p == buf) {
					++p;
					buf = '\0';
				}
				else if (isspace(*p) || *p == (c = getchar()))
					++p;
				else
					return (c == EOF) ? c : total;
			}
		}
	}
	va_end(ap);
	return total;
}
