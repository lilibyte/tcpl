/*
 * Exercise 5-5.
 *
 * Write versions of the library functions strncpy,
 * strncat, and strncmp, which operate on at most the
 * first n characters of their argument strings.
 * For example, strncpy(s,t,n) copies at most n characters
 * of t to s.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>

char *strncpy_(char *dest, char *src, size_t n);
char *strncat_(char *dest, char *src, size_t n);
int strncmp_(char *s1, char *s2, size_t n);

/* strncpy_:  copies at most n characters of the string
   pointed to by src, including its null byte, to the
   buffer pointed to by dest.

   Warning: If there is no null byte among the first n
   bytes of src, the string placed in dest will not be
   null-terminated. */
char *strncpy_(char *dest, char *src, size_t n) {
	while (n--) *dest++ = *src++;
	return dest;
}

/* strncat_:  concatenates at most n characters of the
   string pointed to by src to the buffer pointed to
   by dest. */
char *strncat_(char *dest, char *src, size_t n) {
	while (*dest) ++dest;
	while (n--) *dest++ = *src++;
	return dest;
}

/* strncmp_:  compare at most n characters of the strings
   pointed to by s1 and s2.

   Returns 0 if equal, -1 if s1 comes before s2 in lexicographical
   order, and 1 if s1 comes after s2 in lexicographical order */
int strncmp_(char *s1, char *s2, size_t n) {
	while (*s1 == *s2 && --n) {
		++s1;
		++s2;
	}
	if (*s1 < *s2)
		return -1;
	else if (*s1 > *s2)
		return 1;
	else
		return 0;
}

int main() {
	printf("strncpy_(t1, s1, 20);\n");
	printf("----------\n");
	char s1[] = "let's all love lain!";
	char t1[21];
	strncpy_(t1, s1, 21);
	printf("ASSERT\n");
	printf("^%s$\n", s1);
	printf("EQUAL TO\n");
	printf("^%s$\n", t1);
	printf(strncmp(s1, t1, 21) ? "FALSE\n" : "TRUE\n");

	printf("\nstrncpy_(t2, s1, 10);\n");
	printf("----------\n");
	char t2[21];
	strncpy_(t2, s1, 10);
	printf("ASSERT\n");
	printf("^%.10s$\n", s1);
	printf("EQUAL TO\n");
	printf("^%s$\n", t2);
	printf(strncmp("let's all ", t2, 21) ? "FALSE\n" : "TRUE\n");

	printf("\nstrncat_(t3, s2, 21);\n");
	printf("----------\n");
	char s2[21] = "let's all love ";
	char t3[] = "lain!";
	printf("ASSERT\n");
	printf("^%s$\n", s2);
	printf("PLUS UP TO 6 CHARACTERS OF\n");
	printf("^%s$\n", t3);
	printf("EQUAL TO\n");
	printf("^let's all love lain!$\n");
	strncat_(s2, t3, 6);
	printf(strncmp("let's all love lain!", s2, 21) ? "FALSE\n" : "TRUE\n");

	printf("\nstrncat_(t4, s3, 2);\n");
	printf("----------\n");
	char s3[12] = "THRILL";
	char t4[] = "HOUSE";
	printf("ASSERT\n");
	printf("^%s$\n", s3);
	printf("PLUS UP TO 2 CHARACTERS OF\n");
	printf("^%s$\n", t4);
	printf("EQUAL TO\n");
	printf("^THRILLHO$\n");
	strncat_(s3, t4, 2);
	printf(strncmp("THRILLHO", s3, 12) ? "FALSE\n" : "TRUE\n");

	printf("\nstrncmp_(\"__\", \"__\", 2);\n");
	printf("----------\n");
	printf("ASSERT \"ab\", \"ab\" == 0\n");
	printf(strncmp_("ab", "ab", 2) ? "FALSE\n" : "TRUE\n");
	printf("ASSERT \"aa\", \"ab\" == -1\n");
	printf(strncmp_("aa", "ab", 2) == -1 ? "TRUE\n" : "FALSE\n");
	printf("ASSERT \"az\", \"ab\" == 1\n");
	printf(strncmp_("az", "ab", 2) == 1 ? "TRUE\n" : "FALSE\n");
	printf("ASSERT \"112za\", \"113az\" == 0\n");
	printf(strncmp_("112za", "113az", 2) ? "FALSE\n" : "TRUE\n");
	printf("ASSERT \"1z\", \"2a\" == -1\n");
	printf(strncmp_("1z", "2a", 2) == -1 ? "TRUE\n" : "FALSE\n");
}
