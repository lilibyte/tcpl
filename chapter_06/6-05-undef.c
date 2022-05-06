/*
 * Exercise 6-5.
 *
 * Write a function undef that will remove a name and definition
 * from the table maintained by lookup and install.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

struct nlist {
	struct nlist *next;  // next table in chain
	char *name;          // defined name
	char *defn;          // replacement text
};

static struct nlist *hashtab[HASHSIZE];

struct nlist *lookup(char *);
struct nlist *install(char *, char *);
int undef(char *);
size_t djb2(char *);

int main() {
	struct nlist *first = install("test", "1");
	struct nlist *second = lookup("test");
	printf("%p\n", first);
	printf("%p\n", second);

	int r1 = undef("test");
	struct nlist *idk = lookup("test");
	printf("%d: %p\n", r1, idk);

	int r2 = undef("shouldntexit");
	printf("%d: %p\n", r2, lookup("shouldntexit"));

	first = install("first", "first val");
	// manually add another to test undef
	second = malloc(sizeof(struct nlist));
	second->name = strdup("second");
	second->defn = strdup("second val");
	second->next = hashtab[djb2("first")];
	hashtab[djb2("first")] = second;
	int r3 = undef("first");
	first = lookup("first");
	printf("%d: %p\n", r3, first);
	// check if "second" is still there
	// i can't use lookup() because the string is wrong
	printf("%p\n", hashtab[djb2("first")]);
	printf("%s\n", hashtab[djb2("first")]->name);
}

/* lookup:  look for s in hashtab */
struct nlist *lookup(char *s) {
	struct nlist *np;
	for (np = hashtab[djb2(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

/* undefn:  remove an element from hashtab */
int undef(char *s) {
	struct nlist *np;
	struct nlist *prev = NULL;
	for (np = hashtab[djb2(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0) {
			if (prev != NULL && np != prev)
				prev->next = np->next;
			else
				hashtab[djb2(s)] = NULL;
			free((void *) np->defn);
			free((void *) np->name);
			free((void *) np);
			return 1;
		}
		prev = np;
	}
	return 0;
}

/* install:  put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn) {
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {
		np = malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = djb2(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else
		free((void *) np->defn);
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

/* djb2:  hash function (k=33) by Dan Bernstein */
size_t djb2(char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash % HASHSIZE;
}
