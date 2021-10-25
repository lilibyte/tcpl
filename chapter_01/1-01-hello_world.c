/*
 * Exercise 1-1.
 * Run the "hello, world" program on your system.
 * Experiment with leaving out parts of the program, to see
 * what error messages you get.
*/

#include <stdio.h>

int main() {
  printf("hello, world\n");

  /* some errors mostly from clangd: */

  /*
     printf"hello, world\n");

     Expected ';' after expression
  */


  /*
     prinf("hello, world\n");

     Implicit declaration of function 'prinf' is invalid in C99
  */

  /*
     printf("hello, world\n")

     Expected ';' after expression
  */

  /*
     printf();

     Too few arguments to function call, at least argument
         '__format' must be specified
  */

  /*
     after removing the #include <stdio.h> line:

     Warning: Implicitly declaring library function 'printf' with
         type 'int (const char *, ...)'

     from gcc:

     warning: implicit declaration of function 'printf'
     note: include '<stdio.h>' or provide a declaration of 'printf'
     warning: incompatible implicit declaration of built-in function 'printf'

     apperently because the standard library is linked during
     compilation, so the function is found. but its use without a
     prototype may result in undefined behavior?
  */

  /*
     after removing the main() function but keeping the printf call:

     also from gcc:

     error: expected declaration specifiers or '...' before string constant
  */
}
