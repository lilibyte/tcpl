/*
 * Exercise 7-3.
 *
 * Revise minprintf to handle more of the other facilities of printf.
*/

// not going to add much because I already know there's no reason
// to ever do this instead of using sprintf.

// see Appendix B1.2 for a more in depth look at the printf format

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUF_MAX 128
#define iter(i) (i < BUF_MAX) ? i++ : BUF_MAX-1

int minprintf(char *fmt, ...);

int main(int argc, char *argv[]) {
	// interesting find:
	// none of these will print
	// minprintf("%18446744073709551615ld?\n", 123);
	// minprintf("%4294967295ld?\n", 123);
	// minprintf("%2147483648ld?\n", 123);
	// except this one (LONG_MAX, (2^31) - 1):
	// minprintf("%2147483647ld?\n", 123);
	minprintf("%045Ld\n", 123);
	minprintf("%45.6f\n", 123.45);
	minprintf("%45s\n", "Let's all love Lain!");
}

/* minprintf:  minimal printf with variable argument list */
int minprintf(char *fmt, ...) {
	va_list ap;  // points to each unnamed arg in turn
	char *p, *sval;
	int ival;
	double dval;
	char tmp[BUF_MAX];

	va_start(ap, fmt);  // make ap point to 1st unnamed arg
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		int i = 0;
		tmp[iter(i)] = *p++;
		// flags, which modify the specificiation
		int fin = 0;
		while (!fin) {
			switch(*p) {
			case '+':
			case '-':
			case ' ':
			case '#':
			case '0':
				tmp[iter(i)] = *p++;
				continue;
			default:
				fin = 1;
			}
		}
		// number specifying a minimum field width
		if (*p == '*')
			tmp[iter(i)] = *p++;
		else
			while (isdigit(*p))
				tmp[iter(i)] = *p++;

		// period, which separates the field width from the precision
		if (*p == '.') {
			tmp[iter(i)] = *p++;
			// number, the precision
			if (*p == '*')
				tmp[iter(i)] = *p++;
			else
				while (isdigit(*p))
					tmp[iter(i)] = *p++;
		}
		// length modifier
		switch (*p) {
		case 'h':
		case 'l':
		case 'L':
			tmp[iter(i)] = *p++;
		default:
			break;
		}
		// check if i is too big
		if (i + 1 >= BUF_MAX - 1)
			return 1;
		// defensively set next index to null
		tmp[i++] = '\0';
		// conversion characters
		switch(*p) {
		case 'd':
			ival = va_arg(ap, int);
			strcat(tmp, "d");
			printf(tmp, ival);
			break;
		case 'f':
			dval = va_arg(ap, double);
			strcat(tmp, "f");
			printf(tmp, dval);
			break;
		case 's':
			sval = va_arg(ap, char *);
			strcat(tmp, "s");
			printf(tmp, sval);
			break;
		default:
			putchar(*p);
			break;
		}
		memset(tmp, 0, sizeof(tmp));
	}
	va_end(ap);  // clean up when done
	return 0;
}
