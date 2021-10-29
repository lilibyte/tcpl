/*
 * Exercise 1-21.
 * Write a program `entab` that replaces strings of blanks by the
 * minimum number of tabs and blanks to achieve the same spacing.
 * Use the same tab stops as for `detab`. When either a tab or
 * single blank would suffice to reach a tab stop, which should be
 * given preference?
*/

/*
   Once again, I cannot comprehend these exercise questions which
   is starting to concern me. Luckily other people have asked for help:
   https://stackoverflow.com/q/3020031/

   "Write a program that takes input text with spaces and produces
   output visually equivalent text using tabs to the maximum extent
   possible.

   For example, with tabstops every 8 characters, and showing spaces
   as '_' and tabs as '\t';

   input;
   "_foo:___bar;______#comment"
   output;
   "_foo:\tbar;\t__#comment"

   input;
   "________foo:_____bar;______#comment"
   output;
   "\tfoo:\t_bar;\t___#comment"

   Write the program so that tabstop parameter n can be varied, i.e.
   allow values of n other than 8." (reworded slightly)
*/

/*
   Okay to hell with this question :D

   I ended up just working through the logic in Python and created
   this script:

```
line = "        foo:     bar;      #comment"
# line = "i i i i i i"
# line = "hello foo:   barr;        #comment"
# line = " foo:   bar;        #comment"
# line = "h                X"
print(line)

tabstop = 8
blanks = 0
current_word_len = 0

for index, c in enumerate(line):
    if (c == " " and (index or not current_word_len)
                 and index < len(line)-1
                 and (line[index+1]==" " or line[index-1] == " " )):
        blanks += 1
    else:
        tab = tabstop - (current_word_len % tabstop)
        while blanks:
            print(blanks, end="")
            if tab <= blanks:
                print("\t", end="")
                current_word_len = 0
                blanks -= tab
                tab = tabstop
            else:
                print(" ", end="")
                current_word_len += 1
                blanks -= 1
        print(c, end="")
        current_word_len += 1
        blanks = 0

print()
```

   And translating the logic to C. This one was very difficult for
   me and I don't really know why. I'm also not sure if this fully
   works but it's good enough for me for now.

   The biggest help for me was realizing that blanks being added
   for margin affect the size of tabs (I was previously handling
   "words" and consequtive blanks independently), and that tab
   sizes are not just based on the preceding word being >= tabstop
   but instead based on the preceding word being within multiples
   of the tabstop. For example, assuming `tabs 8`:

   echo -e "123456789012345678901234\tX"  # tab size of 8
   echo -e "12345678901234567890123\tX"   # tab size of 1
   echo -e "1234567890123456\tX"          # tab size of 8
   echo -e "123456789012345\tX"           # tab size of 1
*/

#include <stdio.h>

#define  MAXLINE  79

void print_aligned_columns(char line[], int tabstop) {
  int current_word_len = 0;
  int blanks = 0;
  int index = 0;
  char c;
  while ((c = line[index]) != '\0') {
    if (c == ' ' && (index || !current_word_len) && (line[index+1] != '\0') &&
        (line[index+1] == ' ' || line[index-1] == ' ')) {
      ++blanks;
    } else {
      int tab = tabstop - (current_word_len % tabstop);
      while (blanks) {
        if (tab <= blanks) {
          printf("\t");
          current_word_len = 0;
          blanks -= tab;
          tab = tabstop;
        } else {
          printf(" ");
          ++current_word_len;
          --blanks;
        }
      }
      printf("%c", c);
      ++current_word_len;
      blanks = 0;
    }
    ++index;
  }
}

int getinput(char line[]) {
  char c;
  int i = 0;
  while (i < MAXLINE-1 && (c=getchar()) != EOF && c != '\n') {
    line[i] = c;
    ++i;
  }
  if (i && c == '\n') {
    line[i] = c;
    ++i;
  }
  line[i] = '\0';
  return i;
}

int main() {
  int len;
  char line[MAXLINE];
  while ((len=getinput(line)) > 0) {
    print_aligned_columns(line, 8);
  }
}
