/*
 * Exercise 7-6.
 *
 * Write a program to compare two files, printing
 * the first line where they differ.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("error: expected 2 positional arguments\n");
		exit(1);
	}

	FILE *fp1 = fopen(*++argv, "r");
	FILE *fp2 = fopen(*++argv, "r");

	if (fp1 == NULL || fp2 == NULL) {
		printf("error: failed to read input files\n");
		exit(1);
	}

	char line1[MAXLINE];
	char line2[MAXLINE];
	char *s1, *s2;

	do {
		s1 = fgets(line1, MAXLINE, fp1);
		s2 = fgets(line2, MAXLINE, fp2);
		if (strncmp(line1, line2, MAXLINE) != 0) {
			printf("%s", line2);
			break;
		}
	} while (s1 != NULL && s2 != NULL);
	exit(0);
}
