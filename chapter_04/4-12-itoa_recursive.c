/*
 * Exercise 4-12.
 *
 * Adapt the ideas of printd to write a recursive version of itoa;
 * that is, convert an integer into a string by calling a recursive
 * routine.
*/

#include <stdio.h>

void itoa(int, char[]);

int main() {
	char arr[100];
	itoa(45084839, arr);
	printf("%s\n", arr);
}

/* itoa:  recursively convert n to character string */
void itoa(int n, char r[]) {
	static int i = 0;
	if (n < 0) {
		r[i++] = '-';
		n = -n;
	}
	if (n / 10)
		itoa(n / 10, r);
	r[i++] = n % 10 + '0';
	r[i] = '\0';
}
