/*
 * Exercise 5-4.
 *
 * Write the function strend(s,t), which returns 1 if
 * the string t occurs at the end of the string s, and
 * zero otherwise.
*/

// did a few different versions for fun, not sure which
// is the best (or the least bad...)

#include <stdio.h>

int recmp(char *s, char *t, char *f, int i) {
	if (!(*s))
		return i;
	if (*s == *t)
		recmp(s+1, t+1, f, i+1);
	else
		recmp(s+1, f, f, 0);
}

/* strend3:  check if string s ends with string t
   by comparing the whole string recursively */
int strend3(char *s, char *t) {
	char *tmp = t;
	return recmp(s, t, tmp, 0);
}

/* strend2:  check if string s ends with string t
   by comparing the whole string */
int strend2(char *s, char *t) {
	int i = 0;
	char *tmp = t;
	while (*s) {
		if (*s == *t) {
			++i;
			++t;
		} else {
			i = 0;
			t = tmp;
		}
		++s;
	}
	return i;
}

/* strend:  check if string s ends with string t
   by comparing the ends directly */
int strend(char *s, char *t) {
	int i = 0;
	char *tmp = t;
	while (*s) s++;
	while (*t) t++;
	int d = (t - tmp) + 1;
	while (*s-- == *t--) ++i;
	return i == d;
}

int main() {
	char s[] = "let's all love lain!";
	char t1[] = "lain";
	char t2[] = "lain!";
	printf(strend(s, t1) ? "true\n" : "false\n");
	printf(strend(s, t2) ? "true\n" : "false\n");
	printf(strend2(s, t1) ? "true\n" : "false\n");
	printf(strend2(s, t2) ? "true\n" : "false\n");
	printf(strend3(s, t1) ? "true\n" : "false\n");
	printf(strend3(s, t2) ? "true\n" : "false\n");
}
