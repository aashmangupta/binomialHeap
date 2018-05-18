#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "binomial.h"

//extern void srandom(unsigned int);
//extern long int random(void);

int
main(void) {
    //srandom(1);
    //simple INTEGER test of BINOMIAL
    BINOMIAL *p = newBINOMIAL(displayINTEGER, compareINTEGER, 0, freeINTEGER);
    insertBINOMIAL(p, newINTEGER(0));
    insertBINOMIAL(p, newINTEGER(1));
    insertBINOMIAL(p, newINTEGER(2));
    insertBINOMIAL(p, newINTEGER(3));
    insertBINOMIAL(p, newINTEGER(4));
    insertBINOMIAL(p, newINTEGER(5));
    insertBINOMIAL(p, newINTEGER(6));
    insertBINOMIAL(p, newINTEGER(7));
    insertBINOMIAL(p, newINTEGER(8));
    insertBINOMIAL(p, newINTEGER(9));
    insertBINOMIAL(p, newINTEGER(10));
    insertBINOMIAL(p, newINTEGER(11));
    insertBINOMIAL(p, newINTEGER(12));

    printf("heap:\n");
    displayBINOMIAL(p, stdout);
    printf("debug:\n");
    displayBINOMIALdebug(p, stdout);

    printf("SIZE: %d\n", sizeBINOMIAL(p));
    printf("min should be %d\n",getINTEGER(peekBINOMIAL(p)));

    //extractBINOMIAL(p);

	freeBINOMIAL(p);
    return 0;
}

