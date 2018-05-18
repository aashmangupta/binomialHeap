//
// Created by Aashman Gupta on 2/16/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "heap.h"
#include "scanner.h"
#include <stdarg.h>

char fileType = 'i';
char orderSort = 'I';
int authorInfo = 0;
char *passedFile = 0;

void heapSort(HEAP* heap1, void (*display)(void *,FILE *), void (*free)(void *), FILE* fp) {
   	void* ptr;
    buildHEAP(heap1);
    ptr = extractHEAP(heap1);
    (*display)(ptr, fp);
	printf(" ");
    (*free)(ptr);

    while(sizeHEAP(heap1) != 0) {
        ptr = extractHEAP(heap1);
        (*display)(ptr, fp);
        if(sizeHEAP(heap1) >= 1) {
			printf(" ");
		}
		(*free)(ptr);
    }
	printf("\n");
}

void processOptions(int, char **);

void Fatal(char *, ...);

int main(int argc, char **argv) {
  if (argc == 1) Fatal("%d arguments!\n", argc - 1);

   	processOptions(argc,argv);

	passedFile = argv[argc - 1];

    if (authorInfo == 1) {
        printf("\nWritten by: Aashman Gupta...\n");       //FIXME add explanantion
        printf("Adding a value to the heap takes constant time since you just add it to the last node in a BST.\n");
        printf("Ordering takes linear time, since you don't have to do log(n) for each n.\n");
        printf("Extracting min/max takes linear time, since finding the value is constant, and heapify is log(n)\n");
        exit(0);
    }

    FILE *fp = fopen(passedFile, "r");

    if (fp == 0) {
        exit(0);
    }

    HEAP* currHeap;

    if (fileType == 's') {
        if(orderSort == 'D') {
            currHeap = newHEAP(displaySTRING, compareSTRINGdecr, freeSTRING);
        }

        else {
            currHeap = newHEAP(displaySTRING, compareSTRING, freeSTRING);
        }

		int quoted = stringPending(fp);
		char *str;

		if(quoted != 0) {
			str = readString(fp);
		}
		else {
			str = readToken(fp);
		}

        while (!feof(fp)) {
			insertHEAP(currHeap, newSTRING(str));
            quoted = stringPending(fp);
			if(quoted != 0) {
				str = readString(fp);
			}
 			else {
	 			str = readToken(fp);
 			}
        }

		heapSort(currHeap, displaySTRING, freeSTRING, stdout);
        fclose(fp);
    }

    else if (fileType == 'r') {
        if(orderSort == 'D') {
            currHeap = newHEAP(displayREAL, compareREALdecr, freeREAL);
        }
        else {
            currHeap = newHEAP(displayREAL, compareREAL, freeREAL);
        }

        double decimal = readReal(fp);
        while (!feof(fp)) {
            insertHEAP(currHeap, newREAL(decimal));
            decimal = readReal(fp);
        }

		heapSort(currHeap, displayREAL, freeREAL, stdout);
        fclose(fp);
    }

    else {
        if(orderSort == 'D') {
			currHeap = newHEAP(displayINTEGER, compareINTEGERdecr, freeINTEGER);
        }

        else {
            currHeap = newHEAP(displayINTEGER, compareINTEGER, freeINTEGER);
        }

        int num = readInt(fp);	
		while (!feof(fp)) {
            insertHEAP(currHeap, newINTEGER(num));
        	num = readInt(fp);
		}

        fclose(fp);
    	heapSort(currHeap, displayINTEGER, freeINTEGER, stdout);
	}
	freeHEAP(currHeap);

    return 0;
}

void processOptions(int argc, char **argv) {
	int argIndex = 1;
	char argChar = 0;

	while(argIndex < argc) {
		if(argv[argIndex][0] == '-') {
			//printf("Made it here %c\n", argv[argIndex][1]);
			argChar = argv[argIndex][1];
			
			if(argChar == 'v') {
				authorInfo = 1;
			}

			else if(argChar == 'r') {
				fileType = argChar;
			}

			else if(argChar == 's') {
				fileType = argChar;
			}

			else if(argChar == 'D') {
				orderSort = argChar;
			}
		}
		argIndex++;
	}
	return;
}

void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}

