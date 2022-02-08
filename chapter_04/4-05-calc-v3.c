/*
 * Exercise 4-5.
 *
 * Add access to library functions like sin, exp, and pow.
 * See <math.h> in Appendix B, Section 4.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXVAL 100      // maximum depth of val stack
#define MAXOP 100       // maximum size of operand or operator
#define NUMBER '0'      // signal that a number was found
#define COMMAND '1'     // signal that a command was found
#define BUFSIZE 100     // ungetch buffer size

int getop(char []);
void push(double);
void dump();
void clear();
double pop(void);
void cmdparse();
char *cmdpop(void);
double top(void);
void swap();
int getch(void);
void ungetch(int);
void prints();

int sp = 0;             // next free stack position
double val[MAXVAL];     // value stack
char cmdval[MAXOP];     // command value stack
int last_cmd = 0;       // whether last issued val was a cmd

char buf[BUFSIZE];      // buffer for ungetch
int bufp = 0;           // next free position in buf

/* reverse Polish calculator */
int main() {
  int type;
  double op2;
  double tmp;
  char s[MAXOP];

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
  } else {
    printf("error: unknown command %s\n", cmdval);
  }
}

/* push:  push f onto value stack */
void push(double f) {
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}

/* pop:  pop and return top value from stack*/
double pop(void) {
  if (sp > 0)
    return val[--sp];
  else if (sp == 0)
    return val[sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

/* swap:  swap top two values on the stack */
void swap() {
  double tmp = val[sp-1];
  double tmp2 = val[sp-2];
  val[sp-1] = tmp2;
  val[sp-2] = tmp;
  printf("\t%.8g <-> %.8g\n", tmp, tmp2);
}

/* top:  return top value from stack*/
double top(void) {
  return sp > 0 ? val[sp-1] : val[sp];
}

/* dump:  print contents of value stack */
void dump(void) {
  printf("\t%.8g\n", val[sp - 2]);
  printf("\t%.8g\n", val[sp - 1]);
}

/* clear:  clear contents of value stack */
void clear(void) {
  for (int i = MAXVAL; i >= 0; --i)
    val[i] = 0.0;
}

/* getop:  get next operator or numeric operand */
int getop(char s[]) {
  int c, i = 0;
  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  s[1] = '\0';
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

int getch(void) {
  // get a (possibly pushed back) character
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
  // push character back on input
  if (bufp >= BUFSIZE)
    printf("ungetch: too many charactern\n");
  else
    buf[bufp++] = c;
}
