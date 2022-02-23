/*
 * Exercise 4-10.
 *
 * An alternate organization uses getline to read an entire
 * input line; this makes getch and ungetch unnecessary.
 * Revise the calculator to use this approach.
*/

// refactored everything, exhausting exercise but
// satisfying to have finished it

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define _GNU_SOURCE
#define MAXVAL 100      // maximum depth of valbuf stack
#define MAXVAR 16       // maximum length of variable name
#define BUCKETS 24593   // maximum number of variables
#define DEBUG 0

void push(double);
void dump();
void clear();
double pop();
void alphaparse(int*, bool);
void assignparse();
void cmdparse();
void mathparse(char);
void numparse(int*, bool, bool, bool);
void varparse();
bool ismathop(char);
double top();
void swap();
void echo();
void clearvarbuf();
unsigned long djb2(char*, size_t);

typedef struct node {
  double val;
} node;

int sp = 0;                // next free stack position
double valbuf[MAXVAL];     // value stack
char varbuf[MAXVAR];       // buffer for validating new variable
node *vartable[BUCKETS];   // hash table for variables

// whether last issued val was a variable
//  1 if valid, known variable
// -1 if valid, unknown variable
//  0 if not valid
int last_var = 0;

char *line = NULL;         // input line for getline
char *strstart;            // beginning of operand/operator in input line
size_t strend;             // end of operand/operator in input line

/* reverse Polish calculator */
int main() {
  double op2;
  node *new_var;
  size_t len = 0;
  ssize_t read;
  bool dont_echo = false;

  while ((read = getline(&line, &len, stdin)) != EOF) {
    int i = 0;
    while (i < read-1) {
      while (line[i] == ' ' || line[i] == '\t')
        ++i;
      if (last_var == -1 && line[i] != '=') {
        printf("error: unknown variable $%s\n", varbuf);
        clearvarbuf();
      }
      bool is_signed = (line[i] == '-' || line[i] == '+') && isdigit(line[i+1]);
      bool is_hex = (i < len-1-is_signed)
        && (line[i+is_signed] == '0' && tolower(line[i+1+is_signed]) == 'x');
      bool is_var = line[i] == '$' && isalpha(line[i+1]);
      bool is_decimal = line[i] == '.' && isdigit(line[i+1]);
      if ((dont_echo = (isalpha(line[i]) || is_var))) {
        alphaparse(&i, is_var);
      } else if (is_signed || isdigit(line[i]) || is_decimal) {
        numparse(&i, is_hex, is_decimal, is_signed);
      } else if (line[i] == '=') {
        assignparse();
        ++i;
      } else if (ismathop(line[i])) {
        mathparse(line[i++]);
      } else if (isspace(line[i])) {
        dont_echo = true;
      } else {
        printf("error: unknown operator %c\n", line[i++]);
      }
    }
    if (!dont_echo)
      printf("\t%.8g\n", top());
  }
}

/* cmdparse:  parse most recent command */
void cmdparse() {
  if (strncmp(strstart, "top", strend) == 0) {
    printf("\t%.8g\n", top());
  } else if (strncmp(strstart, "swap", 4) == 0) {
    swap();
  } else if (strncmp(strstart, "dump", 4) == 0) {
    dump();
  } else if (strncmp(strstart, "clear", 4) == 0) {
    clear();
  } else if (strncmp(strstart, "pow", 3) == 0) {
    double op2 = pop();
    push(pow(pop(), op2));
    printf("\t%.8g\n", top());
  } else if (strncmp(strstart, "sin", 3) == 0) {
    push(sin(pop()));
    printf("\t%.8g\n", top());
  } else if (strncmp(strstart, "exp", 3) == 0) {
    push(exp(pop()));
    printf("\t%.8g\n", top());
  } else if (strncmp(strstart, "echo", 4) == 0) {
    echo();
  } else {
    printf("error: unknown command %.*s\n", (int)strend, strstart);
  }
}

/* varparse:  validate known variable and add to value stack */
void varparse() {
  unsigned long hash = djb2(strstart, strend);
  if (DEBUG) {
    printf("-----------------------------------\n");
    printf("starting variable validation\n");
    printf("-----------------------------------\n");
    printf("hash of strstart-strend: %lu\n", hash);
    printf("strstart-strend contents: .%.*s.\n", (int)strend, strstart);
    printf("hashtable address: %p\n", vartable[hash]);
    printf("adding variable to temp buffer\n");
  }
  strncpy(varbuf, strstart, strend);
  if (DEBUG)
    printf("temp buffer contents: .%s.\n", varbuf);
  varbuf[strend+1] = '\0';
  if (vartable[hash] != NULL) {
    if (DEBUG)
      printf("variable exists, adding to value stack\n");
    push(vartable[hash]->val);
    last_var = 1;
    if (DEBUG)
      printf("last_var set to %d (return)\n\n", last_var);
  } else {
    if (DEBUG)
      printf("variable does not exist\n");
    last_var = -1;
    if (DEBUG)
      printf("last_var set to %d (return)\n\n", last_var);
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
    printf("\t0\t1\n");
    return;
  }
  int lastval = sp - 1;
  int pos = 1;
  while (lastval >= 0) {
    printf("\t%.8g\t%d\n", valbuf[lastval], pos);
    --lastval;
    ++pos;
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
  if (last_var == 1) {
    printf("\t%.8g\n", vartable[djb2(varbuf, strlen(varbuf))]->val);
    pop();  // don't keep echo'd value on stack
  }
  else if (last_var == 0)
    printf("error: incorrect operand for echo; variable expected\n");
  clearvarbuf();
}

/* clearvarbuf:  clear the new variable buffer */
void clearvarbuf() {
  if (DEBUG) {
    printf("-----------------------------------\n");
    printf("clearing temp varbuf\n");
    printf("-----------------------------------\n");
    printf("null'd out varbuf\n");
  }
  for (int i = 0; i < MAXVAR; ++i)
    varbuf[i] = '\0';
  last_var = 0;
  if (DEBUG)
    printf("last_var set to %d (return)\n\n", last_var);
}

/* assignparse:  assignment operator handler */
void assignparse() {
  node *new_var;
  if (!last_var) {
    printf("error: incorrect operand for =; variable expected\n");
  } else {
    if (DEBUG) {
      printf("-----------------------------------\n");
      printf("creating new variable\n");
      printf("-----------------------------------\n");
      printf("temp varbuf contents: .%s.\n", varbuf);
    }
    // need unique memory space for new vars
    new_var = malloc(sizeof(double));
    unsigned long hash = djb2(varbuf, strlen(varbuf));
    if (DEBUG) {
      printf("address of new var: %p\n", new_var);
      printf("hash of temp varbuf: %p\n", new_var);
    }
    // throw out old val if re-assignment
    if (vartable[hash] != NULL)
      pop();
    new_var->val = pop();
    if (DEBUG) {
      printf("new var value: %g\n", new_var->val);
      printf("adding new var to hashtable\n");
      printf("adding new var's value to stack (return)\n\n");
    }
    vartable[hash] = new_var;
    // assignment returns new val to stack
    push(new_var->val);
    clearvarbuf();
  }
}

/* alphaparse:  alphanumeric operand handler */
void alphaparse(int *line_index, bool is_var) {
  int word_count = 0;
  int word_start = *line_index;
  strstart = line + (*line_index + is_var);
  ++(*line_index);
  while (isalnum(line[*line_index]) && word_count < MAXVAR)
    ++(*line_index) && ++word_count;
  strend = *line_index - is_var - word_start;
  if (is_var)
    varparse();
  else
    cmdparse();
}

/* numparse:  numeric operand handler */
void numparse(int *line_index, bool is_hex, bool is_decimal, bool is_signed) {
  strstart = line + *line_index;
  line_index += is_signed + (is_hex * 2) + is_decimal;
  while (isdigit(line[++(*line_index)]) || (is_hex && isxdigit(line[*line_index])))
    ;
  if (line[*line_index] == '.' && !is_hex) {
    ++(*line_index);
    while (isdigit(line[++(*line_index)]))
      ;
  } else if (tolower(line[*line_index]) == 'e' && !is_hex) {
    ++(*line_index);
    if (line[*line_index] == '+' || line[*line_index] == '-')
      ++(*line_index);
    if (isdigit(line[(*line_index)++]))
      while (isdigit(line[*line_index]))
        ++(*line_index);
  }
  char tmp = line[*line_index];
  line[*line_index] = '\0';
  push(strtod(strstart, NULL));
  line[*line_index] = tmp;
}

/* mathparse:  mathematical operator handler */
void mathparse(char c) {
  double op2;
  switch (c) {
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
    case '%':
      op2 = pop();
      if (op2 != 0.0)
        push(fmod(pop(), op2));
      else
        printf("error: zero divisor\n");
      break;
  }
}

/* ismathop:  whether a character is an existing mathematical operator */
bool ismathop(char c) {
  switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      return true;
    default:
      return false;
  }
}

/* djb2:  hash function (k=33) by dan bernstein */
unsigned long djb2(char *str, size_t count) {
  unsigned long hash = 5381;
  int c;
  int i = 0;
  while ((c = *str++) && i < count) {
    hash = ((hash << 5) + hash) + c;
    ++i;
  }
  return hash % BUCKETS;
}
