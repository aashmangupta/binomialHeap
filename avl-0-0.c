#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "queue.h"
#include "bst.h"

int
main(void)
    {
		//simple INTEGER test of AVL
    AVL *p = newAVL(displayINTEGER,compareINTEGER,freeINTEGER);
    insertAVL(p,newINTEGER(3));
    insertAVL(p,newINTEGER(2));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    INTEGER *q = newINTEGER(2);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    INTEGER *r = newINTEGER(3);
    freeINTEGER(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    setINTEGER(r,2);
    freeINTEGER(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeAVL(p));
    freeINTEGER(q);
    freeINTEGER(r);
    freeAVL(p);
    
	AVL *temp = newAVL(displayINTEGER,compareINTEGER,freeINTEGER);
	insertAVL(temp, newINTEGER(3400));
	insertAVL(temp, newINTEGER(4500));

	displayAVL(temp, stdout);
	freeAVL(temp);

	return 0;
    }
