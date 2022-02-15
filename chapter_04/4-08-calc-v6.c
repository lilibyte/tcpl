/*
 * Exercise 4-8.
 *
 * Suppose that there will never be more than one character
 * of pushback. Modify getch() and ungetch() accordingly.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXVAL 100      // maximum depth of valbuf stack
#define MAXOP 100       // maximum size of operand or operator
#define NUMBER '0'      // signal that a number was found
#define COMMAND '1'     // signal that a command was found
#define MAXVAR 16       // maximum length of variable name
#define BUCKETS 1543    // maximum number of variables

int getop(char []);
void push(double);
void dump();
void clear();
double pop(void);
void cmdparse();
int varparse();
void varvalid();
double top(void);
void swap();
void echo();
int getch(void);
void ungetch(int);
void ungets();
void clearvarbuf();
unsigned long djb2(char*);

typedef struct node {
  double val;
} node;

int sp = 0;             // next free stack position
double valbuf[MAXVAL];  // value stack
char cmdval[MAXOP];     // command value stack
int last_cmd = 0;       // whether last issued val was a cmd
char varbuf[MAXVAR];    // buffer for validating new variable

// whether last issued val was a variable
//  1 if valid, existing variable
// -1 if valid, unused variable
//  0 if not valid
int last_var = 0;

node *vartable[BUCKETS];   // hash table for variables

char buf;                  // buffer for ungetch

/* reverse Polish calculator */
int main() {
  int type;
  double op2;
  double tmp;
  char s[MAXOP];
  node *new_var;

  while ((type = getop(s)) != EOF) {
    switch (type) {
      case NUMBER:
        push(atof(s));
        break;
      case COMMAND:
        strcpy(cmdval, s);
        break;
      case '+':
        push(pop() + pop());
        break;
      case '*':
        push(pop() * pop());
        break;
      case '-':
        op2 = pop();
        push(pop() - op2);
        break;
      case '/':
        op2 = pop();
        if (op2 != 0.0)
          push(pop() / op2);
        else
          printf("error: zero divisor\n");
        break;
      case '%':
        op2 = pop();
        if (op2 != 0.0)
          push(fmod(pop(), op2));
        else
          printf("error: zero divisor\n");
        break;
      case '$':
        varvalid();
        break;
      case '=':
        if (last_var == 0) {
          printf("error: incorrect operand for =; variable expected\n");
          break;
        }
        // need unique memory space for new vars
        new_var = malloc(sizeof(double));
        // throw out old val if re-assignment
        if (vartable[djb2(varbuf)] != NULL)
          pop();
        new_var->val = pop();
        vartable[djb2(varbuf)] = new_var;
        // assignment returns new val to stack
        valbuf[sp++] = new_var->val;
        clearvarbuf();
        break;
      case '\n':
        if (last_cmd) {
          cmdparse();
          last_cmd = 0;
        } else
          printf("\t%.8g\n", top());
        break;
      default:
        printf("error: unknown command %s\n", s);
        break;
    }
  }
}

/* cmdparse:  parse most recent command */
void cmdparse() {
  if (strcmp(cmdval, "top") == 0) {
    printf("\t%.8g\n", top());
  } else if (strcmp(cmdval, "swap") == 0) {
    swap();
  } else if (strcmp(cmdval, "dump") == 0) {
    dump();
  } else if (strcmp(cmdval, "clear") == 0) {
    clear();
  } else if (strcmp(cmdval, "pow") == 0) {
    double op2 = pop();
    push(pow(pop(), op2));
    printf("\t%.8g\n", top());
  } else if (strcmp(cmdval, "sin") == 0) {
    push(sin(pop()));
    printf("\t%.8g\n", top());
  } else if (strcmp(cmdval, "exp") == 0) {
    push(exp(pop()));
    printf("\t%.8g\n", top());
  } else if (strcmp(cmdval, "echo") == 0) {
    echo();
  } else {
    printf("error: unknown command %s\n", cmdval);
  }
}

/* varparse:  validate and parse variable */
int varparse() {
  char s[MAXVAR];
  char c;
  int i = 0;
  if (isalpha(c = getch())) {
    s[i] = c;
    while (isalnum(c = getch()) && i < MAXVAR-1)
      s[++i] = c;
    if (c == EOF || isspace(c)) {
      s[i+1] = '\0';
      strcpy(varbuf, s);
      return 0;
    }
  }
  return 1;
}

/* varvalid:  validate known variable */
void varvalid() {
  if (varparse() == 0) {
    int hash = djb2(varbuf);
    if (vartable[hash] != NULL) {
      last_var = 1;
      valbuf[sp++] = vartable[djb2(varbuf)]->val;
    } else {
      last_var = -1;
    }
  } else {
    printf("error: invalid variable\n");
    clearvarbuf();
  }
}

/* push:  push f onto value stack */
void push(double f) {
  if (sp < MAXVAL)
    valbuf[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}

/* pop:  pop and return top value from stack*/
double pop(void) {
  if (sp > 0)
    return valbuf[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

/* swap:  swap top two values on the stack */
void swap() {
  double tmp = valbuf[sp-1];
  double tmp2 = valbuf[sp-2];
  valbuf[sp-1] = tmp2;
  valbuf[sp-2] = tmp;
  printf("\t%.8g <-> %.8g\n", tmp, tmp2);
}

/* top:  return top value from stack*/
double top(void) {
  return sp > 0 ? valbuf[sp-1] : valbuf[sp];
}

/* dump:  print contents of value stack */
void dump(void) {
  if (sp == 0) {
    printf("\t0\n");
    return;
  }
  int i;
  for (i = MAXVAL - 1; valbuf[i] == 0; --i)
    ;
  int pos = 1;
  while (i >= 0) {
    if (i < sp) {
      printf("\t%.8g\t%d\n", valbuf[i], pos);
      ++pos;
    }
    --i;
  }
}

/* clear:  clear contents of value stack */
void clear(void) {
  for (int i = MAXVAL; i >= 0; --i)
    valbuf[i] = 0.0;
  sp = 0;
}

/* echo:  output value of a variable*/
void echo(void) {
  if (last_var == 0)
    ;
  else if (last_var == 0)
    printf("error: incorrect operand for echo; variable expected\n");
  else if (last_var == 1) {
    printf("\t%.8g\n", vartable[djb2(varbuf)]->val);
    pop();  // don't keep echo'd value on stack
  }
  clearvarbuf();
}

/* clearvarbuf:  clear the new variable buffer */
void clearvarbuf() {
  for (int i = 0; i < MAXVAR; ++i)
    varbuf[i] = '\0';
  last_var = 0;
}

/* getop:  get next operator or numeric operand */
int getop(char s[]) {
  char c;
  int i = 0;
  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  s[1] = '\0';
  if (last_var == -1 && c != '=') {
    printf("error: unknown variable $%s\n", varbuf);
    clearvarbuf();
  }
  if (isalpha(c)) {
    while (isalnum(s[++i] = c = getch()))
      ;
    s[i] = '\0';
    if (c != EOF)
      ungetch(c);
    last_cmd = 1;
    return COMMAND;
  }
  if (c == '-') {
    s[++i] = c = getch();
    if (!isdigit(c) && c != '.') {
      ungetch(c);
      return '-';
    }
  }
  if (!isdigit(c) && c != '.')
    return c;
  if (isdigit(c))
    while (isdigit(s[++i] = c = getch()))
      ;
  if (c == '.')
    while (isdigit(s[++i] = c = getch()))
      ;
  s[i] = '\0';
  if (c != EOF)
    ungetch(c);
  return NUMBER;
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

/* ungetch:  push charater back on input stack */
void ungetch(int c) {
  buf = c;
}

/* djb2:  hash function (k=33) by dan bernstein */
unsigned long djb2(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
  return hash % BUCKETS;
}
