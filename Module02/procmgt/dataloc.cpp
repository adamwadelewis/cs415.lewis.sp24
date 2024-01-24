//
// File:   dataloc.cpp
// Author: Your Glorious Instructor
// Purpose:
// #include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
using namespace std;
char globBuf[65536];         /* Uninitialized data segment */
int primes[] = { 2, 3, 5, 7 }; /* Initialized data segment */
static int square(int x) {
    int result;         /* Allocated in frame for square() */
    result = x * x;
    return result;     /* Return value passed via register */

}
static void doCalc(int val) {
    printf("The square of %d is %d\n", val, square(val));
    if (val < 1000) {
      int t;          /* allocated in frame for docCalc() */
      t = val * val * val;
      printf("The cube of %d is %d\n", val, t); }
}
int main(int argc, char *argv[])
{
    static int key = 9973;     /* Initialized data segment */
    static char mbuf[10240000];  /* Uninitialized data seg.*/
    char *p;               /* Allocated in frame for main()*/
    p = (char *) malloc(1024);      /* Points to memory in heap     */
    doCalc(key);
    exit(EXIT_SUCCESS);
}
