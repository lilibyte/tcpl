/*
 * Exercise 6-2.
 *
 * Write a program that reads a C program and prints in alphabetical order
 * each group of variable names that are identical in the first 6 characters,
 * but different somewhere thereafter. Don't count words within strings and
 * comments. Make 6 a parameter that can be set from the command line.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define MAXWORDS 1000

int getword(char *, int);
int getch(void);
void ungetch(int);
size_t djb2(char *, int);

char buf;

struct node {
	char *str;
	struct node *next;
	struct node *prev;
};

/* count C keywords */
int main(int argc, char **argv) {
	int unq = 6;
	char word[MAXWORD];
	static struct node *words[1000];

	while (*++argv) {
		if (**argv == '-')
			unq = abs(atoi(*argv+1));
	}

	size_t hash = 0;
	int s;
	while ((s = getword(word, MAXWORD)) != EOF) {
		if (s >= unq) {
			hash = djb2(word, unq);
			if (words[hash] == NULL) {
				struct node *bucket = NULL;
				bucket = words[hash] = malloc(sizeof(struct node));
				bucket->str = NULL;
				bucket->str = malloc(sizeof(word) + 1);
				strncpy(bucket->str, word, s);
				bucket->next = NULL;
				bucket->prev = NULL;
			} else {
				struct node *prev = words[hash];
				int order = strncmp(word, prev->str, s);
				while (prev->next != NULL && order) {
					if ((order = strncmp(word, prev->str, s)) < 0)
						break;
					else if (order > 0)
						prev = prev->next;
				}
				if (order > 0) {
					struct node *new = malloc(sizeof(struct node));
					new->str = malloc(sizeof(word) + 1);
					strncpy(new->str, word, s);
					new->next = prev;
					new->prev = prev->prev;
					if (prev->prev == NULL)
						words[hash] = new;
					else
						prev->prev->next = new;
					prev->prev = new;
				}
				else if (order < 0) {
					struct node *new = malloc(sizeof(struct node));
					new->str = malloc(sizeof(word) + 1);
					strncpy(new->str, word, s);
					new->next = prev;
					new->prev = prev->prev;
					if (prev->prev == NULL)
						words[hash] = new;
					prev->prev = new;
				}
			}
		}
		force_continue:;
	}

	int i = 0;
	struct node *next = NULL;
	while (i < MAXWORDS) {
		if (words[i] != NULL) {
			next = words[i]->next;
			if (next != NULL)
				printf("%s\n", words[i]->str);
			while (next != NULL) {
				printf("%s\n", next->str);
				next = next->next;
			}
		}
		++i;
	}
}

/* getch:  get a (possibly pushed back) character */
int getch(void) {
	if (buf != '\0') {
		char tmp = buf;
		buf = '\0';
		return tmp;
	} else
		return getchar();
}

/* ungetch:  push character back on input */
void ungetch(int c) {
	buf = c;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim) {
	int c;
	char *w = word;
	char *beg = word;

	while (isspace(c = getch()))
		;
	if (c != EOF)
		*w++ = c;
	else
		return c;
	if (c  == '/') {
		c = getch();
		if (c == '/')
			while ((c = getch()) != EOF && c != '\n')
				;
		else if (c == '*') {
			while ((c = getch()) != EOF)
				if (c == '*' && (c = getch()) == '/')
					break;
		} else {
			ungetch(c);
			c = '/';
		}
	}
	else if (c == '"') {
		int p = c;
		while ((c = getch()) != EOF) {
			if (c == '"' && p != '\\')
				break;
			p = c;
		}
	}
	else if (c == '\'') {
		c = getch();
		if (c == '\\')
			c = getch();
		c = getch();
	}
	if (!isalpha(c) && c != '_') {
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; ++w) {
		*w = getch();
		if (!isalnum(*w) && *w != '_') {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return w - beg;
}

/* djb2:  hash function (k=33) by Dan Bernstein */
size_t djb2(char *str, int n) {
	unsigned long hash = 5381;
	int c;
	while (n-- && (c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash % MAXWORDS;
}
