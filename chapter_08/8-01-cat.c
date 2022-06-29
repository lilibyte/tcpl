/*
 * Exercise 8-1.
 *
 * Rewrite the program cat from Chapter 7 using read, write,
 * open and close instead of their standard library equivalents.
 * Perform experiments to determine the relative speeds of the
 * two versions.
*/

// cat is defined on page 163 in K&R2

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

int fdcopy(int);
void error(char *, ...);

int main(int argc, char *argv[]) {
	int fd, res = 0;
	char *prog = basename(*argv);

	if (argc == 1)
		res = fdcopy(fd);
	else
		while (--argc > 0 && res != -1)
			if ((fd = open(*++argv, O_RDONLY, 0)) == -1) {
				error("%s: can't open %s", prog, *argv);
				exit(1);
			} else {
				res = fdcopy(fd);
				close(fd);
			}
	if (res == -1)
		error("%s: writing stdout failed\n", prog);
	exit(0);
}

int fdcopy(int fd) {
	int c, n, total = 0;
	while ((n = read(fd, &c, 1)) > 0) {
		if (write(1, &c, 1) != n)
			return -1;
		++total;
	}
	return total;
}

void error(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}
