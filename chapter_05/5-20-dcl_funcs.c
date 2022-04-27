/*
 * Exercise 5-20.
 *
 * Expand dcl to handle declarations with function
 * argument types, qualifiers like const, and so on.
*/

/*
  annoying but valid (based on my C linter anyway) inputs to test for
  demonstration purposes:

  void f(const volatile int [const volatile restrict])
  void f(const volatile int * const volatile restrict [const volatile restrict])
*/

// what a complete mess :D
// but at least it works i think

// should go without saying for all of these exercises, but
// do note on this one especially that they are not designed
// to be safe or hold up to input that extends past the
// bounds of any of the arrays.

// also for historical novelty sake:  https://cdecl.org
// is a website that does the same thing as this this program
// in your web browser.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define MAXTOKEN 100

#define ERR_INVALID_ARGS "error: invalid function argument list\n"
#define ERR_MISSING "error: missing %c\n"
#define ERR_DUPE "error: duplicate %s declaration specifier\n"
#define ERR_INVALID_TYPE "error: invalid type \"%s\"\n"
#define ERR_RESTRICT "error: restrict requires a pointer\n"
#define ERR_INVALID_QUAL "error: invalid qualifier in array declaration\n"
#define ERR_INVALID_ARR "error: invalid array declaration\n"
#define ERR_INVALID_PTR "error: invalid pointer\n"
#define ERR_EXPECTED_NAME "error: expected name or (dcl)\n"

enum { NAME, PARENS, BRACKETS };
enum { CONST=1, VOLATILE, RESTRICT };

int dcl(void);
int dirdcl(void);
int gettoken(void);
int getch(void);
void ungetch(int);
void flushline();
int validqualifier(char *);
int validtype(char *);
void resetbufs();
int printerrflush(char *, ...);

int tokentype;            // type of last token
int prevtokentype;        // type of one-past last token
int paramqualifier;       // type of last qualifier
int parsingfuncargs;      // 1 if function args are currently being parsed
char temp[MAXTOKEN];      // temp buffer for holding token
char token[MAXTOKEN];     // last token string
char name[MAXTOKEN];      // identifier name
char datatype[MAXTOKEN];  // data type = char, int, etc.
char out[1000];           // output string
char buf;

/* strnrchr:  locate pointer to the n-th to last occurence of a character */
char *strnrchr(char *str, char c, int n) {
	char *beg = str;
	int count = 0;
	while (*str)
		if (*str++ == c)
			++count;
	if (count < n)
		return NULL;
	while ((--str) != beg) {
		if (*str == c && n > 1)
			--n;
		else if (*str == c && n == 1)
			return str;
	}
	return NULL;  // ?
}

/* flushline:  clear stdin buffer up to and including newline
	https://stackoverflow.com/a/25998256 */
void flushline() {
	scanf("%*[^\n]%1*[\n]");
}

/* validfunctoken:  valid token to appear in function argument list */
int validfunctoken(int token) {
	switch (token) {
		case NAME:
		case ')':
		case ',':
		case '*':
		case '[':
		case ']':
			return 1;
		default:
			return 0;
	}
}

int validqualifier(char *token) {
	if (strcmp(token, "const") == 0)
		return paramqualifier = CONST;
	else if (strcmp(token, "volatile") == 0)
		return paramqualifier = VOLATILE;
	else if (strcmp(token, "restrict") == 0)
		return paramqualifier = RESTRICT;
	return paramqualifier = 0;
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
		temp[i] = '\0';
	}
	for (int i = 0; i < 1000; ++i) {
		out[i] = '\0';
	}
	buf = '\0';
	tokentype = 0;
	prevtokentype = 0;
}

/* printerrflush:  print, error, and flush. simple as. */
int printerrflush(char *msg, ...) {
	// learned something new
	// https://stackoverflow.com/a/804705
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	resetbufs();
	flushline();
	return 1;
}

/* dcl:  parse a declarator */
int dcl(void) {
	int ns;
	// count *'s
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
			printf(ERR_MISSING, ')');
			resetbufs();
			return 1;
		}
	// variable name
	} else if (tokentype == NAME)
		strcpy(name, token);
	else {
		printf(ERR_EXPECTED_NAME);
		resetbufs();
		return 1;
	}
	while ((type = gettoken()) == PARENS || type == BRACKETS || type == '(') {
		if (type == PARENS)
			strcat(out, " function returning");
		else if (type == '(') {
			parsingfuncargs = 1;
			int has_const = 0;
			int has_volatile = 0;
			int has_type = 0;
			int has_pointer = 0;
			int has_p_const = 0;
			int has_p_volatile = 0;
			int has_p_restrict = 0;
			int has_arr_open = 0;
			int has_arr_close = 0;
			strcat(out, " function takes");

			while (validfunctoken(type = gettoken())) {
				if (type == ',') {
					if (prevtokentype == ',')
						return printerrflush(ERR_INVALID_ARGS);
					strcat(out, ",");
					has_const = 0;
					has_volatile = 0;
					has_type = 0;
					has_pointer = 0;
					has_p_const = 0;
					has_p_volatile = 0;
					has_p_restrict = 0;
					has_arr_open = 0;
					has_arr_close = 0;
				}

				else if (type == '[') {
					if (has_arr_open)
						return printerrflush(ERR_INVALID_ARR);
					has_arr_open = 1;
					// these can be added again inside [] for some reason
					has_const = 0;
					has_volatile = 0;
					has_p_restrict = 0;
					strcat(out, " array[");
				}

				else if (type == ']') {
					if (!has_arr_open)
						return printerrflush(ERR_MISSING, '[');
					strcat(out, "]");
					has_arr_close = 1;
				}

				else if (type == '*') {
					if (has_pointer)  // been here before
						return printerrflush(ERR_INVALID_PTR);
					char c;
					has_pointer = 1;
					while ((c = getchar()) == ' ' || c == '\t' || c == '*') {
						if (c == ' ' || c == '\t')
							continue;
						++has_pointer;
					}
					ungetch(c);
					char slice[MAXTOKEN+1];
					int n = has_type + has_const + has_volatile;
					if (!n)
						return printerrflush(ERR_INVALID_PTR);
					char *tokenslice = strnrchr(out, ' ', n);
					strcpy(slice, tokenslice);
					strcpy(tokenslice, " pointer to");
					for (int i = 1; i < has_pointer; ++i)
						strcat(out, " pointer to");
					strcat(out, slice);
				}

				else if (type == NAME) {
					if (validqualifier(token)) {
						if (paramqualifier == CONST) {
							if (has_pointer && !has_p_const)
								has_p_const = 1;
							else if (!has_const)
								has_const = 1;
							else
								return printerrflush(ERR_DUPE, "const");
						}

						else if (paramqualifier == VOLATILE) {
							if (has_pointer && !has_p_volatile)
								has_p_volatile = 1;
							else if (!has_volatile)
								has_volatile = 1;
							else
								return printerrflush(ERR_DUPE, "volatile");
						}

						else if (paramqualifier == RESTRICT) {
							if (!has_p_restrict) {
								if (has_pointer)
									has_p_restrict = 1;
								else if (has_arr_open && !has_arr_close)
									has_p_restrict = 1;
								else if (!has_pointer)
									return printerrflush(ERR_RESTRICT);
							}
							else
								return printerrflush(ERR_DUPE, "restrict");
						}

						// "const pointer to int" not "pointer to int const"
						if (has_pointer && !has_arr_open) {
							char slice[MAXTOKEN+12];
							char *tokenslice = NULL;
							int n = 2 + has_type + has_const + has_volatile;
							tokenslice = strnrchr(out, ' ', n);
							strcpy(slice, tokenslice);
							strcpy(tokenslice+1, token);
							strcat(out, slice);
							goto passcat;
						}
					}

					else if (validtype(token)) {
						if (has_arr_open)
							return printerrflush(ERR_INVALID_QUAL);
						has_type = 1;
					}

					else {
						return printerrflush(ERR_INVALID_TYPE, token);
					}

					if (prevtokentype == '[')
						sprintf(temp, "%s", token);
					else
						sprintf(temp, " %s", token);
					strcat(out, temp);
					passcat:;

				} else {
					if (prevtokentype == ',')
						return printerrflush(ERR_INVALID_ARGS);
					if (has_arr_open && !has_arr_close)
						return printerrflush(ERR_MISSING, ']');
					strcat(out, " and returns");
					break;
				}
			}
			parsingfuncargs = 0;
		}
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
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
			prevtokentype = tokentype;
			return tokentype = PARENS;
		} else {
			ungetch(c);
			prevtokentype = tokentype;
			return tokentype = '(';
		}
	} else if (c == '[') {
		if (parsingfuncargs == 1)
			return tokentype = c;
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0';
		prevtokentype = tokentype;
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		do
			*p++ = c;
		while (isalnum(c = getch()));
		*p = '\0';
		ungetch(c);
		prevtokentype = tokentype;
		return tokentype = NAME;
	} else {
		prevtokentype = tokentype;
		return tokentype = c;
	}
}

/* convert declaration to words */
int main() {
	int has_const = 0;
	int has_volatile = 0;
	char temp[MAXTOKEN+1];
	while (gettoken() != EOF) {
		if (*token == '\0')
			continue;
		// parse qualifiers and type
		if (validqualifier(token)) {
			if (paramqualifier == CONST) {
				if (has_const)
					printerrflush(ERR_DUPE, "const");
				has_const = 1;
				if (has_volatile) {
					sprintf(temp, " %s", token);
					strcat(datatype, temp);
				}
				else
					strcat(datatype, token);
			}
			else if (paramqualifier == VOLATILE) {
				if (has_volatile)
					printerrflush(ERR_DUPE, "volatile");
				has_volatile = 1;
				if (has_const) {
					sprintf(temp, " %s", token);
					strcat(datatype, temp);
				}
				else
					strcat(datatype, token);
			}
			else
				printerrflush(ERR_RESTRICT);
			continue;
		}
		else if (validtype(token)) {
			if (has_const || has_volatile) {
				sprintf(temp, " %s", token);
				strcat(datatype, temp);
			}
			else
				strcat(datatype, token);
		}
		// parse rest of line
		int r = dcl();
		if (!r && tokentype != '\n')
			printf("syntax error\n");
		else if (!r) {
			printf("%s: %s %s\n", name, out, datatype);
		}
		resetbufs();
		has_const = 0;
		has_volatile = 0;
	}
}
