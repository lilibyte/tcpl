/*
 * Exercise 6-4.
 *
 * Write a program that prints the distinct words in its input sorted
 * into decreasing order of frequency of occurence. Precede each word
 * by its count.
*/

// pretty much what i did for 6-3, just have to sort them now

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define BUCKETS 3079

struct node {
	char *str;
	int count;
};

int getword(char *, int);
int getch(void);
void ungetch(int);
size_t djb2(char *);
void initnoise(struct node *[]);
void printwords(struct node *[]);
int nodecmp(const void*, const void*);

char buf;

/* count words */
int main() {
	char word[MAXWORD];
	static struct node *words[BUCKETS];
	size_t hash;
	initnoise(words);
	while (getword(word, MAXWORD) != EOF) {
		hash = djb2(word);
		if (words[hash] != NULL) {
			if (words[hash]->count > -1) {
				words[hash]->count++;
			}
		} else {
			struct node *new = malloc(sizeof(struct node));
			new->count = 1;
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
	qsort(table, BUCKETS, sizeof(struct node*), nodecmp);
	for (int i = 0; i < BUCKETS; ++i) {
		if (table[i] != NULL) {
			if (table[i]->count != -1)
				printf("%4d %s\n", table[i]->count, table[i]->str);
			free(table[i]->str);
			free(table[i]);
		}
	}
}

/* nodecmp:  compare two table nodes for sorting */
int nodecmp(const void *n1, const void *n2) {
	// table is a pointer to nodes, so qsort iterators are
	// pointers to pointers to nodes
	// https://stackoverflow.com/a/70051345
	const struct node *const *e1 = n1;
	const struct node *const *e2 = n2;
	const struct node *p1 = *e1;
	const struct node *p2 = *e2;
	if (!p1 && !p2)
		return 0;
	else if (!p1)
		return 1;
	else if (!p2)
		return -1;
	if (p1->count > p2->count)
		return -1;
	else if (p1->count < p2->count)
		return 1;
	else
		return 0;
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
		;
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
