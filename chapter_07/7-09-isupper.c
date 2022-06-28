/*
 * Exercise 7-9.
 *
 * Functions like isupper can be implemented to save
 * space or to save time. Explore both possibilities.
*/

#include <stdio.h>

#define isupper_min(c) c >= 'A' && c <= 'Z'

// it seems like other people's ideas of fast are pretty
// conflicting and unsatisfactory.
// https://clc-wiki.net/wiki/K%26R2_solutions:Chapter_7:Exercise_9
// the C Answer Book more-or-less just says the macro version
// saves both time and space, but others disagree.

// found an interesting take on the saves time variation:
// https://github.com/msambinelli/KnR_The_C_Programming_Language/blob/master/cap7/exercise7.9.c

int upper_table[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0
};

int isupper_stolen(int c) {
	return upper_table[c];
}

// the "correct" answer for saves space might actually just be the
// same expression as my isupper_min() but inside of a function.

int isupper_min_better(int c) {
	return c >= 'A' && c <= 'Z';
}

int main() {
	printf("a/A is upper? %d/%d\n", isupper_min('a'), isupper_min('A'));
	printf("z/Z is upper? %d/%d\n", isupper_min('z'), isupper_min('Z'));

	printf("a/A is upper? %d/%d\n", isupper_stolen('a'), isupper_stolen('A'));
	printf("z/Z is upper? %d/%d\n", isupper_stolen('z'), isupper_stolen('Z'));

	printf("a/A is upper? %d/%d\n", isupper_min_better('a'), isupper_min_better('A'));
	printf("z/Z is upper? %d/%d\n", isupper_min_better('z'), isupper_min_better('Z'));
}
