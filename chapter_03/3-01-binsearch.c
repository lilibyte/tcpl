/*
 * Exercise 3-1.
 *
 * Our binary search makes two tests inside the loop, when
 * one would suffice (at the price of more tests outside).
 * Write a version with only one test inside the loop and
 * measure the difference in run-time.
*/

// i cheated at this one because i couldn't figure it out
// in a reasonable amount of time and its not really a
// C-specific exercise. already planning on coming back
// to algorithms later.
// see link below for help. any solution that's moving the
// comparisons from the loop body to the loop condition
// are missing the point i think
// https://www.clc-wiki.net/wiki/K&R2_solutions:Chapter_3:Exercise_1#Solution_by_Colin_Barker

// when i was doing a cursory study of algorithms last
// november i learned about "branch prediction", so my
// assumption would be binsearch2 will be faster because
// its logic is more predictable (less incorrect branches
// to go down) but i dont know if these are related concepts
// and i really dont feel like putting the time into this
// one right now.

// my guess above appears to be incorrect based on my
// searches online, but whatever i'll leave it

// but the logic makes more sense to me now after looking
// it over: the goal of binsearch2 is to reduce the range
// until low == high and then compare the value at that
// index to x once outside the loop to determine whether
// the array contains x at all

#include <stdio.h>

int binsearch(int x, int v[], int n) {
  int low, high, mid;
  low = 0;
  high = n - 1;
  while (low <= high) {
    mid = (low + high) / 2;
    if (x < v[mid])
      high = mid - 1;
    else if (x > v[mid])
      low = mid + 1;
    else
      return mid;
  }
  return -1;
}

int binsearch2(int x, int v[], int n) {
  int low = 0;
  int high = n - 1;
  int mid;

  // if low == high then the range has been reduced as far as possible
  // and only one value is left to compare against x
  while (low < high) {
    mid = (low + high) / 2;
    if (x > v[mid])
      low = mid + 1;
    else
      high = mid;
  }
  // low and high could be equivalently used here for comparison
  return v[high] == x ? high : -1;
}

int main() {
  // not a big enough array to practically test run-time speeds
  int arr[] = {3, 67, 393, 471, 472, 789, 1042};

  printf("%d\n", binsearch(108, arr, 7));
  printf("%d\n", binsearch(789, arr, 7));
  printf("%d\n", binsearch(3, arr, 7));

  printf("%d\n", binsearch2(108, arr, 7));
  printf("%d\n", binsearch2(789, arr, 7));
  printf("%d\n", binsearch2(3, arr, 7));
}
