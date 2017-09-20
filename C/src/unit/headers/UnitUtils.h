#ifndef UNIT
#define UNIT

#include <sys/param.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Add a "breakpoint" and exit the program.
 * @param str The error to print.
 */
void unit_error(char * str);

/**
 * Test if two int arrays are the same.
 * @param base The array containing expected values.
 * @param compare The array containing the computed values.
 * @param length The arrays length.
 * @return 1 if the same, 0 else.
 */
int unit_arrayEquals(int * base, int * compare, int length);

#endif
