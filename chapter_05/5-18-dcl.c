/*
 * Exercise 5-18.
 *
 * Make dcl recover from input errors.
*/

// not sure this is everything i should be recovering from,
// but i handled a few new errors and now reset the buffers

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

int dcl(void);
int dirdcl(void);

int gettoken(void);
int tokentype;            // type of last token
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

int validtype(char *token) {
	if (strcmp(token, "void") == 0)
		return 1;
	else if (strcmp(token, "char") == 0)
		return 1;
	else if (strcmp(token, "short") == 0)
		return 1;
	else if (strcmp(token, "int") == 0)
		return 1;
	else if (strcmp(token, "long") == 0)
		return 1;
	else if (strcmp(token, "float") == 0)
		return 1;
	else if (strcmp(token, "double") == 0)
		return 1;
	else
		return 0;
}

// inefficient, but that's irrelevant for this
void resetbufs() {
	for (int i = 0; i < MAXTOKEN; ++i) {
		token[i] = '\0';
		name[i] = '\0';
		datatype[i] = '\0';
	}
	for (int i = 0; i < 1000; ++i) {
		out[i] = '\0';
	}
	buf = '\0';
}

/* dcl:  parse a declarator */
int dcl(void) {
	int ns;
	// count *'s'
	for (ns = 0; gettoken() == '*'; )
		++ns;
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

/* convert declaration to words */
int main() {
	while (gettoken() != EOF) {
		// first token on line is the datatype
		strcpy(datatype, token);
		if (!validtype(datatype)) {
			printf("error: invalid type \"%s\"\n", datatype);
			flushline();
			resetbufs();
			continue;
		}
		// parse rest of line
		int r = dcl();
		if (tokentype != '\n')
			printf("syntax error\n");
		else if (!r)
			printf("%s: %s %s\n", name, out, datatype);
		resetbufs();
	}
}
