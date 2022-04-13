/*
 * Exercise 5-10.
 *
 * Write the program expr, which evaluates a reverse Polish
 * expression from the command line, where each operator or
 * operand is a separate argument. For example,
 *
 *     expr   2   3   4   +   *
 *
 * evaluates 2 x (3+4).
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* isdigit:  return 1 if str is made up of digits and 0 otherwise */
int isdigitstr(char *str) {
	while (*str)
		if (!isdigit(*(str++)))
			return 0;
	return 1;

}

/* Reverse Polish Expression Calculator */
int main(int argc, char **argv) {
	static int stack[100];
	size_t stackp = 0;
	char **arg = ++argv;

	while (--argc > 0) {
		arg = argv++;
		if (isdigitstr(*arg)) {
			stack[stackp++] = atoi(*arg);
			continue;
		}
		switch (**arg) {
			case '+':
				stack[stackp-2] = stack[stackp-2] + stack[stackp-1];
				--stackp;
				break;
			case '-':
				stack[stackp-2] = stack[stackp-2] - stack[stackp-1];
				--stackp;
				break;
			case '*':
				stack[stackp-2] = stack[stackp-2] * stack[stackp-1];
				--stackp;
				break;
			case '/':
				if (!stack[stackp-2] || !stack[stackp-1]) {
					printf("error: cannot divide by zero\n");
					return 1;
				}
				stack[stackp-2] = stack[stackp-2] / stack[stackp-1];
				--stackp;
				break;
			default:
				printf("error: invalid operator: %c\n", **arg);
				return 1;
		}
		if ((*(*arg)+1) != ' ' && *((*arg)+1) != '\0') {
			printf("error: invalid operator: %s\n", *arg);
			return 1;
		}
	}
	printf("%d\n", stack[stackp-1]);
	return 0;
}
