/*
 * Exercise 4-13.
 *
 * Write a recursive version of the function reverse(s), which
 * reverses the string s in place.
*/

#include <stdio.h>
#include <string.h>

void reverse(char[], int);

int main() {
	char str[] = "let's all love lain!";
	printf("%s\n", str);
	reverse(str, strlen(str));
	printf("%s\n", str);
}

/* revers:  reverse a given string in place */
void reverse(char str[], int len) {
	static int i = 0;
	char tmp = str[len - 1 - i];
	str[len - 1 - i] = str[i];
	str[i++] = tmp;
	if (i <= (len - 1) / 2)
		reverse(str, len);
}
