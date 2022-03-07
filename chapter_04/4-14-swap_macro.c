/*
 * Exercise 4-14.
 *
 * Define a macro swap(t,x,y) that interchanges two arguments of
 * type t. (Block structure will help.)
*/

/*
<lilibyte> ,kr4-14 can anyone explain what the point of this exercise is?
           is `swap(t,x,y) t y, t x` too simple? i feel like i'm missing
           what the exercise is asking

  <fizzie> lilibyte: It's (probably) looking for a macro definition
           which makes it so that
           int x = 123, y = 456;
           swap(int, x, y);
           printf("x=%d, y=%d\n", x, y);`
           will print "x=456, y=123".

<lilibyte> `t __TEMP_FOR_SHITTY_MACRO = y; y = x;
            x = __TEMP_FOR_SHITTY_MACRO;` works then? seems to

  <fizzie> An `do { ... } while (0)` wrapper is often recommended.
           (Consider a context like `if (x) swap(int, y, z);`.)

   <sham1> Also then you wouldn't need a __TEMP_FOR_SHITTY_MACRO
           and one can give it an actual name
*/

// thanks senpais

#include <stdio.h>

#define swap(t,x,y) do { t __mtmp = y; y = x; x = __mtmp; } while (0)

int main() {
	int x = 123, y = 456;
	printf("x=%d, y=%d\n", x, y);
	swap(int, x, y);
	printf("x=%d, y=%d\n", x, y);
}
