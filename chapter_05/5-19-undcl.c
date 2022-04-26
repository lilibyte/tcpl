/*
 * Exercise 5-19.
 *
 * Modify undecl so that it does not add redundant parentheses
 * to declarations.
*/

/*
	before input:  daytab [13] * int
	before output: int (*daytab[13])

	after input:   daytab [13] * int
	after output:  int *daytab[13]

	before input:  x () * [] * () char
	before output: char (*(*x())[])()

	after input:   x () * [] * () char
	after output:  char (*(*x())[])()
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int dcl(void);
int dirdcl(void);
int gettoken(void);

int tokentype;            // type of last token
int prevtokentype;        // type of one-past last token
char token[MAXTOKEN];     // last token string
char name[MAXTOKEN];      // identifier name
char datatype[MAXTOKEN];  // data type = char, int, etc.
char out[1000];           // output string
char buf;

/* flushline:  clear stdin buffer up to and including newline
	https://stackoverflow.com/a/25998256 */
void flushline() {
	scanf("%*[^\n]%1*[\n]");
}

// inefficient, but that's irrelevant for this
void resetbufs() {
	for (int i = 0; i < MAXTOKEN; ++i) {
		token[i] = '\0';
		name[i] = '\0';
		datatype[i] = '\0';
	}
	out[0] = '\0';
	buf = '\0';
}

/* dcl:  parse a declarator */
int dcl(void) {
	int ns;
	// count *'s'
	for (ns = 0; gettoken() == '*'; )
		++ns;
	dirdcl();
	if (dirdcl())
		return 1;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return 0;
}

/* dirdcl:  parse a direct declarator */
int dirdcl(void) {
	int type;
	// ( dcl )
	if (tokentype == '(') {
		dcl();
		if (tokentype != ')') {
			printf("error: missing )\n");
			resetbufs();
			return 1;
		}
	// variable name
	} else if (tokentype == NAME)
		strcpy(name, token);
	else {
		printf("error: expected name or (dcl)\n");
		resetbufs();
		return 1;
	}
	while ((type = gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
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

/* gettoken:  return next token */
int gettoken(void) {
	int c;
	char *p = token;
	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		do
			*p++ = c;
		while (isalnum(c = getch()));
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else
		return tokentype = c;
}

/* convert words to declarations */
int main() {
	int type;
	char temp[1000];
	while (gettoken() != EOF) {
		strcpy(out, token);
		while ((type = gettoken()) != '\n') {
			if (type == PARENS || type == BRACKETS) {
				if (type == PARENS && prevtokentype == '*') {
					sprintf(temp, "(%s)%s", out, token);
					strcpy(out, temp);
				} else
					strcat(out, token);
			} else if (type == '*') {
				if (prevtokentype == PARENS)
					sprintf(temp, "(*%s)", out);
				else
					sprintf(temp, "*%s", out);
				strcpy(out, temp);
			} else if (type == NAME) {
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
			} else {
				printf("invalid input at %s\n", token);
				resetbufs();
				flushline();
				break;
			}
			prevtokentype = tokentype;
		}
		if (*out != '\0')
			printf("%s\n", out);
	}
}
