/*
 * Exercise 6-3.
 *
 * Write a cross-referencer that prints a list of all words in a document,
 * and, for each word, a list of the line numbers on which it occurs.
 * Remove noise words like "the", "and", and so on.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define BUCKETS 3079

struct node {
	char *str;
	int count;
	int *lines;
	size_t line_sz;
};

int getword(char *, int);
int getch(void);
void ungetch(int);
size_t djb2(char *);
void initnoise(struct node *[]);
void printwords(struct node *[]);

char buf;
int line = 1;

/* count words */
int main() {
	char word[MAXWORD];
	static struct node *words[BUCKETS];
	size_t hash;
	initnoise(words);
	while (getword(word, MAXWORD) != EOF) {
		hash = djb2(word);
		if (words[hash] != NULL) {
			if (words[hash]->count != -1) {
				words[hash]->count++;
				if (words[hash]->lines[words[hash]->line_sz-1] != line)
					words[hash]->line_sz++;
				words[hash]->lines = realloc(words[hash]->lines,
						sizeof(int) * words[hash]->line_sz);
				words[hash]->lines[words[hash]->line_sz-1] = line;
			}
		} else {
			struct node *new = malloc(sizeof(struct node));
			new->count = 1;
			new->line_sz = 1;
			new->lines = malloc(sizeof(int) * 2);
			new->lines[0] = line;
			new->str = malloc(sizeof(word));
			strcpy(new->str, word);
			words[hash] = new;
		}
	}
	printwords(words);
}

/* initnoise:  initialize some noisy words into the table */
void initnoise(struct node *table[]) {
	// found list online
	char *noisy[] = {
		"a", "about", "after", "all", "also", "an", "another", "any", "are",
		"as", "and", "at",
		"be", "because", "been", "before", "being", "between", "but", "both", "by",
		"came", "can", "come", "could",
		"did", "do",
		"each", "even",
		"for", "from", "further", "furthermore",
		"get", "got",
		"has", "had", "he", "have", "her", "here", "him", "himself", "his",
		"how", "hi", "however",
		"i", "if", "in", "into", "is", "it", "its", "indeed",
		"just",
		"like",
		"made", "many", "me", "might", "more", "moreover", "most", "much",
		"must", "my",
		"never", "not", "now",
		"of", "on", "only", "other", "our", "out", "or", "over",
		"said", "same", "see", "should", "since", "she", "some", "still", "such",
		"take", "than", "that", "the", "their", "them", "then", "there", "these",
		"therefore", "they", "this", "those", "through", "to", "too", "thus",
		"under", "up",
		"very",
		"was", "way", "we", "well", "were", "what", "when", "where", "which",
		"while", "who", "will", "with", "would",
		"you", "your"
	};
	int len = sizeof noisy / sizeof noisy[0];
	int i = 0;
	while (i < len) {
		struct node *new = malloc(sizeof(struct node));
		new->count = -1;
		new->str = malloc(sizeof(noisy[i]));
		strcpy(new->str, noisy[i]);
		table[djb2(noisy[i])] = new;
		++i;
	}
}

/* printwords:  output word counts after input */
void printwords(struct node *table[]) {
	for (int i = 0; i < BUCKETS; ++i) {
		if (table[i] != NULL && table[i]->count != -1) {
			printf("%4d %s\t\t", table[i]->count, table[i]->str);
			for (int j = 0; j < table[i]->line_sz; ++j) {
				printf("%d", table[i]->lines[j]);
				if (j + 1 != table[i]->line_sz)
					printf(", ");
			}
			printf("\n");
			free(table[i]->lines);
			free(table[i]->str);
			free(table[i]);
		}
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
	while (isspace(c = getch()))
		if (c == '\n')
			++line;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; ++w) {
		*w = getch();
		if (!isalnum(*w)) {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return word[0];
}

/* djb2:  hash function (k=33) by Dan Bernstein */
size_t djb2(char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash % BUCKETS;
}
