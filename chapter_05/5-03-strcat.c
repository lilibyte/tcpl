/*
 * Exercise 5-3.
 *
 * Write a pointer version of the function strcat that we
 * showed in Chapter 2: strcat(s,t) copies the string t
 * to the end of s.
*/

#include <stdio.h>

/* strcat_:  copy the string t to the end of s;
   return number of characters copied */
int strcat_(char *s, char *t) {
	while (*s++);
	--s;
	char *tmp = t;
	while (*s++ = *t++);
	return t - tmp;
}

int main() {
	char s[24] = "let's all love ";
	char t[] = "lain!";
	int r = strcat_(s, t);
	printf("%s\n", s);
	printf("%d\n", r);
}
