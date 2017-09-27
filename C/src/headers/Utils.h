#ifndef UTILS
#define UTILS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MMIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define UNUSED(x) (void)(x)

#define MALLOC(t, n) (t *) malloc((n) * sizeof(t))
#define MCHECK(x, m) if(!(x)){fprintf(stderr, "MALLOC ERROR IN FUNCTION %s.", m);exit(EXIT_FAILURE);}

#define REALLOC(t, p, n) (t *) realloc(p, (n) * sizeof(t))
#define RCHECK(x, m) if(!(x)){fprintf(stderr, "REALLOC ERROR IN FUNCTION %s.", m);exit(EXIT_FAILURE);}

/**
 * @param p The variable that will contain the allocated pointer.
 * @param t Type to allocate.
 * @param n Number of elements of this type to allocate.
 * @param m Message in case of failure.
 */
#define MMALLOC(p, t, n, m) if((n) == 0){free(p);(p)=NULL;}else{(p) = MALLOC(t, n); MCHECK(p, m)};
/**
 * @param p The variable that will contain the allocated pointer.
 * @param t Type to allocate.
 * @param n Number of elements of this type to allocate.
 * @param m Message in case of failure.
 */
#define RREALLOC(p, t, n, m) if((n) == 0){free(p);(p)=NULL;}else{(p) = REALLOC(t, p, n); RCHECK(p, m)};

typedef enum
{
	False = 0,
	True = 1
} Bool;

void warn(char * format,  ...);

void fatal_error(char * format, ...);

#endif
