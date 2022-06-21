/*
 * Exercise 7-5.
 *
 * Rewrite the postfix calculator of Chapter 4 to use scanf
 * and/or sscanf to do the input and number conversion.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXVAL 100          // maximum depth of val stack
#define MAXOP 100           // maximum size of operand or operator
#define MAXLINE MAXOP * 10  // maximum size of input line

int getch(void);
void ungetch(int);
int getop(char []);
void push(double);
double pop(void);
double top(void);

int sp = 0;                 // next free stack position
double val[MAXVAL];         // value stack

int main() {
	char line[MAXLINE];
	int res = 0;
	while (res != EOF) {
		double op2;
		double f = 0.0;
		char c = '\0';
		int offset;
		fgets(line, MAXOP * 10, stdin);
		char *ptr = line;
		while ((res = sscanf(ptr, "%lf%n", &f, &offset)) == 1) {
			ptr += offset;
			push(f);
		}
		int fin = 0;
		while (!fin) {
			if ((res = sscanf(ptr, "%c", &c)) == 1) {
				++ptr;
				switch (c) {
				case ' ':
				case '\t':
					continue;
				case '+':
					push(pop() + pop());
					break;
				case '-':
					op2 = pop();
					push(pop() - op2);
					break;
				case '*':
					push(pop() * pop());
					break;
				case '/':
					op2 = pop();
					if (op2 != 0.0)
						push(pop() / op2);
					else
						printf("error: zero divisor\n");
					break;
				case '\n':
					printf("\t%.8g\n", top());
					fin = 1;
					break;
				default:
					printf("error: unknown command %c\n", c);
					break;
				}
			}
			else
				fin = 1;
		}
	}
}

void push(double f) {
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

double pop(void) {
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

double top(void) {
	return sp > 0 ? val[sp-1] : val[sp];
}
