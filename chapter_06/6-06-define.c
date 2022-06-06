/*
 * Exercise 6-6.
 *
 * Implement a simple version of the #define preprocessor (i.e., no
 * arguments) suitable for use with C programs, based on the routines
 * of this section. You may also find getch and ungetch helpful.
*/

// See Appendix A12. for a definition of the capabilities of the
// preprocessor.

// I didn't get to implementing nested macros (either macro names
// as function arguments, or existing macros in the definition of
// another macro) and my code is too messy at this point to care
// about fixing. I probably also missed places where I should have
// done bounds checking, or bounds checked incorrectly, but I
// don't consider this a big deal for random exercises.

// This should have been broken into smaller pieces, and a recursive
// descent parser should have been used to parse input.

// Some features should work as-is:
// * most valid #define syntax
// * #undef for deleting macros
// * (...) variable arguments
// * __VA_ARGS__ variable argument expansion
// * #abc stringification operator
// * abc##def concatenation operator

// There are plenty of things that probably don't work, or edge
// cases that will cause my code to fail, but I don't care
// anymore and need to move on because I'm losing motivation lol.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAXWORD 64
#define MAXSUB 128
// "For portability, you should not have more than 31
// parameters for a macro."
// https://www.ibm.com/docs/en/i/7.3?topic=directive-function-like-macros
#define MAXARGS 31
#define BUCKETS 3079

#define ERR_UNEXPECTED_EOF -1
#define ERR_MAX_ARGS -2
#define ERR_MAX_ARG -3
#define ERR_MALLOC_FAIL -4
#define ERR_STR_UNTERMINATED -5
#define ERR_EXPECTED_EXPRESSION -6
#define ERR_EXPECTED_N_ARGS -7

#define ERR_NAME_MISSING "error: macro name missing\n"
#define ERR_INVALID_PP_TOKEN "error: pasting formed '%.*s', an invalid preprocessing token\n"
#define ERR_INVALID_HASH "error: invalid hash for macro \"%s\"\n"
#define ERR_INVALID_PARAM_TOKEN "error: invalid token in macro parameter list\n"
#define ERR_CLOSE_EXPECTED "error: expected macro function argument list to close\n"
#define ERR_MAX_NAME "error: maximum characters for macro name exceeded\n"
#define ERR_MAX_TOKEN "error: tried to store too many characters in token\n"
#define ERR_MAX_SUB "error: macro substitution exceeds maximum allowed length %d %d\n"
#define ERR_END_CAT "error: '##' cannot appear at end of macro expansion\n"
#define ERR_START_CAT "error: '##' cannot appear at start of macro expansion\n"
#define ERR_INCOMPLETE_DEF "error: incomplete macro definition\n"
#define ERR_INCOMPLETE_FUN_DEF "error: incomplete macro function definition\n"
#define ERR_MAX_PARAM "error: maximum characters for parameter in macro function definition exceeded\n"
#define ERR_MAX_PARAMS "error: maximum parameters for function definition exceeded\n"
#define ERR_STRINGIFY_PARAM "error: '#' is not followed by a macro parameter\n"
#define ERR_EXPECTED_COMMA "error: expected comma in macro parameter list\n"
#define ERR_MACRO_NAME "error: macro name must be an identifier\n"
#define ERR_STR_UNTERMINATED_STR "error: missing terminating '\"' character\n"
#define ERR_MALLOC_FAIL_STR "error: failed to allocate memory\n"
#define ERR_UNEXPECTED_EOF_STR "error: incomplete macro function call\n"
#define ERR_MAX_ARGS_STR "error: maximum arguments for function call exceeded\n"
#define ERR_MAX_ARG_STR "error: maximum characters for argument in function call exceeded\n"
#define ERR_EXPECTED_EXPRESSION_STR "error: expected expression in argument list\n"
#define ERR_EXPECTED_N_ARGS_STR "error: expected %d arguments for %s)\n"

struct arg {
	// index in the substitution value the identifier appears at
	ptrdiff_t line_index;
	// position in parameter list the identifier was given at
	ptrdiff_t arg_pos;
	// if true, \ and " characters at this location should
	// be escaped by an additional \
	// https://en.cppreference.com/w/c/preprocessor/replace
	// https://en.cppreference.com/w/cpp/preprocessor/replace
	bool stringified;
	// #define test(a, b) a + b
	//                    ^   ^ line_index == 4; arg_pos == 1
	//                    ^ line_index == 0; arg_pos == 0
	// #define test2(a, b) a, #b
	//                     ^   ^ stringified == true
	//                     ^ stringified == false
};

struct node {
	// macro identifier key; includes terminating ( if macro is
	// function-like
	char *id;
	// macro's substitution value
	char *val;
	// if macro is function-like AND val contains named parameters
	// this has their data; otherwise, it will be NULL
	struct arg **argorder;
	// if macro is function-like this is the number of
	// required arguments the macro function takes; otherwise
	// it will be NULL
	int *argc;
	// true if macro takes variable arguments (...) argumenst
	bool variadic;
	// if variadic is false, it will be set to -1; otherwise
	// is the index of val where __VA_ARGS__ was provided
	int va_index;
	// true if __VA_ARGS__ is prefixed with a # stringification
	// operator
	bool va_stringified;
	// pointer to indexes of val that mark the end of the
	// left operator of a macro concatenation expression;
	// if the macro does not use this operator, this should
	// be NULL
	// (remember that a named argument is discarded from val)
	// literal ## right
	//       ^ catops == 6
	// named_arg ## right
	// ^ catops == 0
	int *catops;
	struct node *next;
};

struct node *subs[BUCKETS];

int getch(void);
void ungetch(int);
size_t djb2(char *);
char *getkey(void);
char *getsub(size_t *, struct node *);
size_t *getargs(struct node *);
int parseargs(char **);
void getlaststart(char *tmp, char **otp, char **ot, int co, ptrdiff_t *lts);
int gettoken(char *);
int discardblank(void);
int discardline(void);
void discardcomment();
int testcomment();
void printerr(int);
int verifyid(char *);
struct node *initnode();

char buf = '\0';  // buffer for getch/ungetch

/* #define macro substitution preprocessor for C */
int main() {
	char c;
	char *word = malloc(MAXWORD);
	size_t hash = 0;
	while ((gettoken(word)) >= 1) {
		if (strcmp("#define", word) == 0) {
			struct node *new_node = initnode();
			new_node->id = getkey();
			if (new_node->id == NULL)
				return 1;
			if (*new_node->id == '\0') {
				printf(ERR_NAME_MISSING);
				return 1;
			}
			size_t *args = NULL;
			if ((c = getch()) == '(') {
				args = getargs(new_node);
				if (args == NULL)
					return 1;
			}
			else
				ungetch(c);
			if (args != NULL) {
				new_node->argorder = malloc(sizeof(struct arg) * MAXARGS);
				if (new_node->argorder == NULL)
					return 1;
			}
			new_node->catops = calloc(MAXSUB, sizeof(int));
			new_node->val = getsub(args, new_node);
			if (new_node->argorder != NULL && *new_node->argorder == NULL) {
				free(new_node->argorder);
				new_node->argorder = NULL;
			}
			if (new_node->catops != NULL && *new_node->catops == 0)
				new_node->catops = NULL;
			free(args);
			if (new_node->val == NULL)
				return 1;
			size_t hash = djb2(new_node->id);
			if (subs[hash] != NULL && strcmp(new_node->id, subs[hash]->id))
				new_node->next = subs[hash];
			subs[hash] = new_node;
		}
		else if (strcmp("#undef", word) == 0) {
			char *key = getkey();
			size_t hash = djb2(key);
			if (subs[hash] != NULL)
				subs[hash] = subs[hash]->next;
			if (buf == '\n')
				buf = '\0';
		}
		// token is an existing macro key
		else if (subs[hash = djb2(word)] != NULL) {
			struct node *next = subs[hash];
			while (next->next != NULL) {
				if (strncmp(next->id, word, MAXWORD) == 0)
					break;
				else if (next->next == NULL) {
					printf(ERR_INVALID_HASH, next->id);
					return 1;
				}
				else
					next = next->next;
			}
			// macro is not function-like and does not contain a ## operator
			if (next->argc == NULL && next->catops == NULL) {
				printf("%s", next->val);
			}
			// macro is not function-like but does contain a ## operator
			else if (next->argc == NULL && next->catops != NULL) {
				char *valp = next->val;
				int *catops = next->catops;
				int index = 0;
				ptrdiff_t last_token_start = -1;
				bool unverified_cat = false;
				char *output_token = malloc(MAXSUB);
				char *otp = output_token;
				while (*valp) {
					if (index == (*catops) - 1) {
						unverified_cat = true;
						while (*valp == ' ' || *valp == '\t') {
							++valp;
							++index;
						}
						++catops;
					}
					if (last_token_start == -1 && !isspace(*valp))
						last_token_start = otp - output_token;
					else if (last_token_start > -1 && (isspace(*valp) || *valp == '\0'))
						last_token_start = -1;
					if (isspace(*valp) && unverified_cat) {
						int res = verifyid(output_token + last_token_start);
						if (!res) {
							printf(ERR_INVALID_PP_TOKEN, res, output_token + last_token_start);
							return 1;
						}
						last_token_start = -1;
						unverified_cat = false;
					}
					*otp++ = *valp;
					*otp = '\0';
					++valp;
					++index;
				}
				if (unverified_cat) {
					int res = verifyid(output_token + last_token_start);
					if (res) {
						printf(ERR_INVALID_PP_TOKEN, res, output_token + last_token_start);
						return 1;
					}
				}
				printf("%s", output_token);
			}
			// marco is function-like, but takes no arguments
			else if (*next->argc == 0 && !next->variadic) {
				while (isspace(c = getch()) || c == '/') {
					if (c == '/') {
						int res = testcomment();
						if (res == 0) {
							printf(ERR_INVALID_PARAM_TOKEN);
							return 1;
						}
					}
				}
				if (c != ')') {
					printf(ERR_CLOSE_EXPECTED);
					return 1;
				}
				printf("%s", next->val);
			}
			// macro is function-like and takes required arguments
			// (named, comma-separated) and/or variable arguments (...)
			else {
				char *args[MAXARGS];
				int argc = parseargs(args);
				if (argc == ERR_EXPECTED_N_ARGS) {
					printf(ERR_EXPECTED_N_ARGS_STR, *next->argc, next->id);
					return 1;
				}
				else if (argc < 0) {
					printerr(argc);
					return 1;
				}
				if (argc < *next->argc || argc > *next->argc && !next->variadic) {
					printf(ERR_EXPECTED_N_ARGS_STR, *next->argc, next->id);
					return 1;
				}
				char *valp = next->val;
				struct arg **argorders = next->argorder;
				struct arg *argorder = NULL;
				if (argorders != NULL)
					argorder = *argorders;
				int index = 0;
				int *catops = next->catops;
				char *output_token = malloc(MAXWORD * MAXARGS);
				char *otp = output_token;
				bool unverified_cat = false;
				ptrdiff_t last_token_start = -1;
				bool stropen = false;
				while (*valp || argorder != NULL || next->va_index == index) {
					if (index == next->va_index) {
						int max = *next->argc;
						int j = 0;
						while (max < argc || max == 0) {
							if (j) {
								*otp++ = ',';
								*otp++ = ' ';
							}
							if (next->va_stringified) {
								if (last_token_start == -1)
									last_token_start = (otp-1) - output_token;
								char *argp = args[max++];
								while (*argp) {
									if (*argp == '\\' || *argp == '"')
										*otp++ = '\\';
									*otp++ = *argp;
									++argp;
								}
							}
							else {
								char *tmp = args[max++];
								getlaststart(tmp, &otp, &output_token, *catops, &last_token_start);
							}
							++j;
						}
						if (catops && index == *catops) {
							unverified_cat = true;
							while (*valp == ' ' || *valp == '\t') {
								++valp;
								++index;
							}
							++catops;
						}
						*otp = '\0';
						++index;
					}
					else if ((argorder != NULL && index == argorder->line_index) || *valp == 0) {
						if (argorder->stringified == true) {
							if (last_token_start == -1)
								last_token_start = (otp-1) - output_token;
							char *argp = args[argorder->arg_pos];
							while (*argp) {
								if (*argp == '\\' || *argp == '"')
									*otp++ = '\\';
								*otp++ = *argp++;
							}
							*otp = '\0';
						}
						else {
							char *tmp = args[argorder->arg_pos];
							getlaststart(tmp, &otp, &output_token, *catops, &last_token_start);
						}
						if (catops && index == (*catops) - 1) {
							unverified_cat = true;
							while (*valp == ' ' || *valp == '\t') {
								++valp;
								++index;
							}
							++catops;
						}
						++argorders;
						argorder = *argorders;
					}
					else if (*valp) {
						if (last_token_start == -1 && (isalpha(*valp) || *valp == '_'))
							last_token_start = otp - output_token;
						if (unverified_cat && isspace(*valp)) {
							int res = verifyid(output_token + last_token_start);
							if (res) {
								printf(ERR_INVALID_PP_TOKEN, res, output_token + last_token_start);
								return 1;
							}
							unverified_cat = false;
							last_token_start = -1;
						}
						else if (catops && index == *catops) {
							while (isspace(*(otp - 1)))
								--otp;
							*(otp + 1) = '\0';
							unverified_cat = true;
							while (*valp == ' ' || *valp == '\t') {
								++valp;
								++index;
							}
							++catops;
							continue;
						}
						*otp++ = *valp;
						++valp;
						++index;
					}
				}
				*otp = '\0';
				if (unverified_cat) {
					int res = verifyid(output_token + last_token_start);
					if (res) {
						printf(ERR_INVALID_PP_TOKEN, res, output_token + last_token_start);
						return 1;
					}
				}
				printf("%s", output_token);
			}
		}
		else if (*word) {
			printf("%s", word);
		}
		if (buf) {
			printf("%c", buf);
			buf = '\0';
		}
		memset(word, '\0', MAXWORD);
	}
}

/* initnode:  initialize a new node with default values */
struct node *initnode() {
	struct node *new_node = malloc(sizeof(struct node));
	new_node->id = NULL;
	new_node->val = NULL;
	new_node->argorder = NULL;
	new_node->argc = NULL;
	new_node->variadic = false;
	new_node->va_index = -1;
	new_node->va_stringified = false;
	new_node->next = NULL;
	new_node->catops = NULL;
	return new_node;
}

/* getlaststart:  set the index of where an arg's last valid token starts */
void getlaststart(char *tmp, char **otp, char **ot, int co, ptrdiff_t *lts) {
	ptrdiff_t last_before_space = -1;
	bool discard_spaces = false;
	while (*tmp) {
		if (*tmp == ' ' || *tmp == '\t') {
			if (*lts != -1)
				last_before_space = *lts;
			*lts = -1;
		}
		else if (*lts == -1)
			*lts = *otp - *ot;
		// don't write space characters to otp if token
		// will be used as concatenation operand
		if (!discard_spaces && co == *otp - *ot)
			discard_spaces = true;
		if (!(discard_spaces && *tmp == ' ' || *tmp == '\t'))
			*(*otp)++ = *tmp;
		++tmp;
	}
	// trailing spaces
	if (*lts == -1 && last_before_space != -1)
		*lts = last_before_space;
}

/* verifyid:  check if s is a valid identifier; return num of parsed chars */
int verifyid(char *s) {
	char *p = s;
	int res = 0;
	bool stropen = false;
	if (!isalpha(*p)) {
		res = 1;
		if (*p == '"')
			stropen = true;
		++p;
	}
	while (isalnum(*p) || *p == '_')
		++p;
	if (!res && ((*p == ',' && *(p + 1) == '\0') || *p == '\0'))
		return 0;
	if (*p == '"') {
		++p;
		stropen = !stropen;
		while (*p) {
			if (*p == '"')
				stropen = !stropen;
			else if (!stropen && !isalnum(*p))
				break;
			++p;
		}
		res = 1;
	}
	else if (ispunct(*p)) {
		res = 1;
		++p;
	}
	if (res)
		res = p - s;
	return res;
}

/* getkey:  get a valid #define macro name from stdin */
char *getkey(void) {
	char *word = malloc(MAXWORD);
	char *wordp = word;
	discardblank();
	for (int i = 0, c = 0; (c = getch()) != EOF; ++i) {
		if (i >= MAXWORD - 1) {
			printf(ERR_MAX_NAME);
			free(word);
			return NULL;
		}
		if (isspace(c) || i > 0 && c == '(') {
			ungetch(c);
			if (i + 1 <= MAXWORD - 2 && c == '(')
				*wordp++ = '(';
			else if (c == '(')
				// counter will increment and trigger the above error
				continue;
			*wordp = '\0';
			return word;
		}
		if (i == 0 && !isalpha(c) && c != '_') {
			printf(ERR_MACRO_NAME);
			free(word);
			return NULL;
		}
		if (i > 0 && !(isalnum(c) || c == '_')) {
			printf(ERR_MACRO_NAME);
			free(word);
			return NULL;
		}
		*wordp++ = c;
	}
	free(word);
	return NULL;
}

/* getargs:  get a #define macro function's argument list */
size_t *getargs(struct node *macro) {
	// note the last char in input before getting here was (
	discardblank();
	// allocate a "2D array":
	// https://stackoverflow.com/a/29977424
	size_t *args = calloc(MAXARGS, sizeof(size_t));
	char *arg = malloc(MAXWORD);
	macro->argc = malloc(sizeof(int));
	if (args == NULL || arg == NULL || macro->argc == NULL)
		return NULL;
	*macro->argc = 0;
	size_t *argsp = args;
	char *argp = arg;
	int i = 0;
	char c = getch();
	if (c == ')')
		return args;
	int argc = 0;
	ungetch(c);
	while ((c = getch()) != EOF) {
		// if this is hit, the character is inherently not ), and
		// would need to increment at least one more time after this
		if (argsp - args >= MAXARGS) {
			printf(ERR_MAX_PARAMS);
			return NULL;
		}
		// ensure counter is one less than MAX
		if (i >= MAXWORD - 1 && !(c == ')' || c == ',')) {
			printf(ERR_MAX_PARAM);
			free(args);
			return NULL;
		}
		if (c == '\\') {
			discardblank();
			if ((c = getch()) == '\n') {
				*argp = ' ';
				++i;
				continue;
			} else {
				printf(ERR_INVALID_PARAM_TOKEN);
				free(args);
				return NULL;
			}
		}
		if (c == '\n') {
			printf(ERR_INCOMPLETE_FUN_DEF);
			free(args);
			return NULL;
		}
		if (c == '.') {
			if ((c = getch()) == '.' && (c = getch()) == '.') {
				discardblank();
				if ((c = getch()) != ')') {
					printf(ERR_CLOSE_EXPECTED);
					free(args);
					return NULL;
				}
				macro->variadic = true;
				break;
			}
			else {
				ungetch(c);
				continue;
			}
		}
		if (i == 0 && !isalpha(c) && c != '_') {
			printf(ERR_INVALID_PARAM_TOKEN);
			free(args);
			return NULL;
		}
		if (c == ')') {
			++argc;
			break;
		}
		if (c == ' ' || c == '\t') {
			discardblank();
			char new = getch();
			if (new == '\\') {
				ungetch(new);
				continue;
			}
			if (new != ',' && new != ')') {
				printf(ERR_EXPECTED_COMMA);
				free(args);
				return NULL;
			}
			ungetch(new);
			continue;
		}
		if (c == ',') {
			*argp = '\0';
			*argsp++ = djb2(arg);
			argp = arg;
			++argc;
			discardblank();
			i = 0;
			continue;
		}
		if (!isalnum(c) && c != '_') {
			printf(ERR_INVALID_PARAM_TOKEN);
			free(args);
			return NULL;
		}
		*argp++ = c;
		++i;
	}
	if (c == EOF) {
		printf(ERR_INCOMPLETE_DEF);
		free(args);
		return NULL;
	}
	*argp = '\0';
	*argsp = djb2(arg);
	*macro->argc = argc;
	return args;
}

/* getsub:  get a #define macro's substitute value */
char *getsub(size_t *args, struct node *macro) {
	char c;
	discardblank();
	char *val = malloc(MAXSUB);
	int word = 0;
	if (val == NULL)
		return val;
	char *valp = val;
	int *catops = macro->catops;
	int i = 0;
	int argc = 0;
	// currently parsing a string literal
	bool stropen = false;
	// currently parsing a stringified token
	bool strwrap = false;
	// i wish there was a better way to do this
	// https://stackoverflow.com/a/34450520
	static size_t variadic_hash;
	static bool variadic_hash_computed = false;
	if (variadic_hash_computed == false) {
		variadic_hash = djb2("__VA_ARGS__");
		variadic_hash_computed = true;
	}
	bool incomplete_concat = false;
	while ((c = getch()) != EOF) {
		bool invalid_c = isspace(c) || (!isalnum(c) && c != '_');
		if (i >= (MAXSUB-1) + strwrap * 2) {
			printf(ERR_MAX_SUB, i, strwrap);
			return NULL;
		}

		// check if last word was __VA_ARGS__ and handle accordingly
		if (macro->variadic && macro->va_index == -1 && !stropen && invalid_c) {
			*valp = '\0';
			size_t hash = djb2(valp - word);
			if (hash == variadic_hash) {
				valp -= word;
				*valp = '\0';
				i -= word;
				if (incomplete_concat)
					incomplete_concat = false;
				macro->va_index = i;
				if (strwrap) {
					*valp++ = '"';
					++i;
					macro->va_stringified = true;
				}
				strwrap = false;
				ungetch(c);
				word = 0;
				continue;
			}
		}

		// check if last word was in args and handle accordingly
		if (invalid_c && args != NULL && macro->argorder != NULL
				&& stropen == false && word > 0) {
			*valp = '\0';
			size_t hash = djb2(valp - word);
			size_t *argsp = args;
			while (argsp != NULL && *argsp != 0) {
				if (*argsp == hash) {
					valp -= word;
					i -= word;
					if (incomplete_concat)
						incomplete_concat = false;
					if (strwrap) {
						*valp++ = '"';
						++i;
					}
					*valp = '\0';
					struct arg *new_arg = malloc(sizeof(struct arg));
					new_arg->arg_pos = argsp - args;
					new_arg->line_index = (valp - val) - strwrap;
					new_arg->stringified = strwrap;
					macro->argorder[argc++] = new_arg;
					strwrap = false;
					break;
				}
				++argsp;
			}
			if (c == '\n' || c == '#')
				ungetch(c);
			else {
				do {
					*valp++ = c;
					++i;
				} while ((c = getch()) == ' ' || c == '\t');
				ungetch(c);
				*valp = '\0';
			}
			word = 0;
			continue;
		}

		// if ## was given and the current character is a valid
		// identifier character, the concatenation operands are complete
		if (!invalid_c && incomplete_concat)
			incomplete_concat = false;

		if (c == '#' && stropen == false && strwrap == false) {
			char new = getch();
			// concatenation operator found
			if (new == '#') {
				if (!i && !argc) {
					printf(ERR_START_CAT);
					return NULL;
				}
				incomplete_concat = true;
				int j = 1;
				while (isspace(*(valp - j)))
					++j;
				// remove 1 from j to subtract its default value
				// add one to catops so 0 can always mean false
				*catops++ = (i - (j - 1)) + 1;
				// add a space after concat operator to handle the case
				// where `arg##__VA_ARGS__` is given because va_index
				// shouldn't have the same index as `arg`s argorder index.
				// the space will be disregarded by main().
				if (argc && macro->argorder != NULL
						&& macro->argorder[argc-1] != NULL
						&& macro->argorder[argc-1]->line_index == *(catops-1)) {
					*valp++ = ' ';
					++i;
				}
				continue;
			}
			// macro is function-like
			else if (args != NULL) {
				// and next character is not a valid first identifier character,
				// or if the function does not take any arguments, it is
				// inherently invalid and parsing can stop
				if ((!isalpha(new) && new != '_') || *args == 0) {
					printf(ERR_STRINGIFY_PARAM);
					return NULL;
				}
				ungetch(new);
				discardblank();
				strwrap = true;
				*valp++ = '"';
				++i;
				continue;
			}
			// # is a literal character if macro not function-like
			else if (args == NULL) {
				ungetch(new);
			}
		}

		if (c == '\n') {
			int j = 1;
			while (isspace(*(valp-j)))
				++j;
			if (*(valp-j) == '\\') {
				*(valp-j) = ' ';
				valp -= j;
				discardblank();
				++i;
				continue;
			}
			else
				break;
		} else if (c == '"' && strwrap == false) {
			stropen = !stropen;
		} else if (c == '/' && stropen == false) {
			int res = testcomment();
			if (res == 0) {
				++i;
				*valp++ = '/';
			} else if (res == -1)
				buf = '\0';
			continue;
		}
		*valp++ = c;
		++i;
		if (!invalid_c)
			++word;
	}
	*valp = '\0';
	if (incomplete_concat) {
		printf(ERR_END_CAT);
		return NULL;
	}
	return val;
}

/* gettoken:  get single token from standard input */
int gettoken(char *buffer) {
	char c;
	int i = 0;
	bool stropen = false;
	while ((c = getch()) != EOF) {
		if (i >= MAXWORD) {
			printf(ERR_MAX_TOKEN);
			return -1;
		}
		if ((c == ' ' || c == '\t') && stropen == false) {
			if (i)
				break;
			else {
				ungetch(c);
				return 1;
			}
		}
		else if (c == '#' && i == 0)
			;
		else if (c == '_')
			;
		else if (stropen == false
				&& c == '('
				&& i > 0
				&& isalnum(*(buffer-1)) || *(buffer-1) == '_') {
			*buffer = '(';
			return ++i;
		}
		else if (c == '"' || c == '\'')
			stropen = !stropen;
		else if (c == '/') {
			int res = testcomment();
			if (res == 0) {
				++i;
				*buffer++ = '/';
			} else if (res == -1)
				buf = '\0';
			continue;
		}
		else if (c == '\n' || (ispunct(c) && stropen == false)) {
			i = 1;
			break;
		}
		++i;
		*buffer++ = c;
	}
	*buffer = '\0';
	ungetch(c);
	return i;
}

/* parseargs:  read and validate arguments given to a macro function */
int parseargs(char **buf) {
	int argc = 0;
	char *arg = malloc(MAXWORD);
	char *argp = arg;
	if (arg == NULL)
		return ERR_MALLOC_FAIL;
	int index = 0;
	char c;
	char prev = '\0';
	bool stropen = false;
	bool terminate = false;
	int scope = 0;
	while (!terminate) {
		if (argc > MAXARGS) {
			return ERR_MAX_ARGS;
		}
		while ((c = getch()) != EOF) {
			if (index >= MAXWORD)
				return ERR_MAX_ARG;
			if (c == '/' && stropen == false) {
				if (testcomment() != 0)
					continue;
			}
			else if (c == '"' && prev != '\\') {
				stropen = !stropen;
			}
			else if (c == ',' && stropen == false) {
				if (index == 0)
					return ERR_EXPECTED_EXPRESSION;
				*argp = '\0';
				*buf++ = arg;
				++argc;
				if (argc < MAXARGS)
					*buf = NULL;
				argp = malloc(MAXWORD);
				if (argp == NULL)
					return ERR_MALLOC_FAIL;
				arg = argp;
				index = 0;
				continue;
			}
			else if (c == ')' && stropen == false) {
				if (scope == 0) {
					if (index == 0 && argc > 0)
						return ERR_EXPECTED_EXPRESSION;
					argc = (argc || index) ? argc + 1 : 0;
					*argp = '\0';
					*buf++ = arg;
					if (argc < MAXARGS)
						*buf = NULL;
					terminate = true;
					break;
				}
				else
					--scope;
			}
			else if ((c == ' ' || c == '\t') && index == 0 && stropen == false) {
				continue;
			}
			else if (c == '\n') {
				if (stropen)
					return ERR_STR_UNTERMINATED;
				else
					continue;
			}
			else if (c == '(' && stropen == false)
				++scope;
			prev = c;
			*argp++ = c;
			++index;
		}
		index = 0;
		prev = '\0';
		if (c == EOF) {
			return ERR_UNEXPECTED_EOF;
		}
	}
	return argc;
}

/* testcomment:  check if a comment is present in the character stream */
int testcomment() {
	char c = getch();
	switch (c) {
		case '*':
			discardcomment();
			return -1;
		case '/':
			discardline();
			return 1;
		default:
			ungetch(c);
			return 0;
	}
}

/* discardcomment:  discard block comment from stdin */
void discardcomment() {
	char c;
	bool star = false;
	char prev = '\0';
	while ((c = getch()) != EOF) {
		if (c == '/' && prev == '*')
			break;
		prev = c;
	}
	if (c == EOF)
		ungetch(c);
	else
		ungetch(getch());
}

/* discardblank:  discard all tabs and spaces from stdin */
int discardblank(void) {
	char c;
	while ((c = getch()) == ' ' || c == '\t')
		;
	ungetch(c);
	return c;
}

/* discardline:  discard all characters until the next newline */
int discardline(void) {
	char c;
	while ((c = getch()) != '\n' && c != EOF)
		;
	ungetch(c);
	return c;
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

void printerr(int c) {
	switch (c) {
		case ERR_STR_UNTERMINATED:
			printf(ERR_STR_UNTERMINATED_STR);
			break;
		case ERR_MALLOC_FAIL:
			printf(ERR_MALLOC_FAIL_STR);
			break;
		case ERR_UNEXPECTED_EOF:
			printf(ERR_UNEXPECTED_EOF_STR);
			break;
		case ERR_MAX_ARGS:
			printf(ERR_MAX_ARGS_STR);
			break;
		case ERR_MAX_ARG:
			printf(ERR_MAX_ARG_STR);
			break;
		case ERR_EXPECTED_EXPRESSION:
			printf(ERR_EXPECTED_EXPRESSION_STR);
			break;
	}
}

/* djb2:  hash function (k=33) by Dan Bernstein */
size_t djb2(char *str) {
	// ignore invalid characters for identifier start
	while (*str != '\0' && !isalnum(*str) && *str != '_')
		++str;
	unsigned long hash = 5381;
	int c;
	// hash until identifier ends
	while ((c = *str++) && isalnum(c) || c == '_')
		hash = ((hash << 5) + hash) + c;
	if (c == '(')
		hash = ((hash << 5) + hash) + c;
	return hash % BUCKETS;
}
