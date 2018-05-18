#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<ctype.h>
#include"binomial.h"
#include"integer.h"
#include"real.h"
#include"string.h"
#include"avl.h"
#include"queue.h"
#include"vertex.h"
#include"edge.h"
#include<string.h>
#include<ctype.h>
#include"scanner.h"

char author = 0;

static void update(void *v,void *n) { //v is a vertex, n is a binomial heap node
	VERTEX *p = v;
	setVERTEXowner(p, n);
}

static int personalCompare(void *v, void *w) {
	return getVERTEXnumber(v) - getVERTEXnumber(w);
}

static void fillUpBinom(BINOMIAL* b, DLL* list) {
	if(list == 0) {
		return;
	}
	if(sizeDLL(list) == 0) {
		return;
	}
	firstDLL(list);
	while(currentDLL(list)) {
		insertBINOMIAL(b, currentDLL(list));
		nextDLL(list);
	}
}

int main(int argc, char** argv) {
	if(argc == 3) {
		if((strcmp(argv[1], "-v") == 0) || (strcmp(argv[2], "-v") == 0)) {
			printf("This assignment was written by Aashman Gupta.\n");
			return 0;
		}
	}

	if(strcmp(argv[1], "-v") == 0) {
		printf("This assignment was written by Aashman Gupta.\n");
		return 0;
	}

	FILE* fp = fopen(argv[1], "r");
	
	if(fp == 0) {
		printf("EMPTY\n");
		return 0;
	}

	char* currString = 0;
	int count = 0;
	VERTEX *v1, *v2, *start = 0;
	EDGE* edge;
	int weight = 1;
	AVL *vertexAVL = newAVL(displayVERTEX,compareVERTEX,freeVERTEX);
	AVL *edgeAVL = newAVL(displayEDGE,compareEDGE,freeEDGE);
	BINOMIAL *heap = newBINOMIAL(displayVERTEX, compareVERTEX, update,freeVERTEX);
	
	currString = readToken(fp);
	while(!feof(fp)) {
		while(1) {
			if(strcmp(currString, ";") == 0) {
				count = 0;
				break;
			}
			count++;
			if(count == 1) {
				v1 = newVERTEX(atoi(currString));
				if(start == 0) {
					insertAVL(vertexAVL, v1);
					setVERTEXowner(v1, insertBINOMIAL(heap,v1));
					start = v1;
				}
			}
			if(count == 2) {
				v2 = newVERTEX(atoi(currString));
			}
			if(count == 3) {
				weight = atoi(currString);
			}
			currString = readToken(fp);
		}
		
		if(getVERTEXnumber(v1) != getVERTEXnumber(v2)) {
			VERTEX* tempVert = findAVL(vertexAVL, v1);

			if(tempVert == 0) {
				insertAVL(vertexAVL, v1);
				setVERTEXowner(v1, insertBINOMIAL(heap,v1));
			}
			else {
				v1 = tempVert;
			}

			tempVert = findAVL(vertexAVL, v2);
			if(tempVert == 0) {
				insertAVL(vertexAVL, v2);
				setVERTEXowner(v2, insertBINOMIAL(heap,v2));
			}
			else {
				v2 = tempVert;
			}
		
			//Storing edge in AVL and adding stuff to Vertex neighbors.
			edge = newEDGE(getVERTEXnumber(v1), getVERTEXnumber(v2), weight);
			if(findAVLcount(edgeAVL, edge) == 0) {
				//printf("Inserting edge (%d, %d) with weight %d\n", getEDGEv1(edge), getEDGEv2(edge), getEDGEweight(edge));
				insertAVL(edgeAVL, edge);
				edge = newEDGE(getVERTEXnumber(v2), getVERTEXnumber(v1), weight);
				insertAVL(edgeAVL, edge);
				//printf("Making %d and %d neighbors of each other.\n", getEDGEv1(edge), getEDGEv2(edge));
				insertVERTEXneighbor(v1, v2);
				insertVERTEXweight(v1, weight);
				insertVERTEXneighbor(v2, v1);
				insertVERTEXweight(v2, weight);
			}
		}
		weight = 1;
		currString = readToken(fp);
	}
	if(sizeAVL(edgeAVL) == 0) {
		printf("EMPTY\n");
		return 0;
	}

	//Prim starting from this point forward------>
	setVERTEXkey(start,0);
	DLL *neighbors, *weights;
	VERTEX* ptr = 0;
	decreaseKeyBINOMIAL(heap, getVERTEXowner(start), start);
	
	while(sizeBINOMIAL(heap) > 0) {
		ptr = extractBINOMIAL(heap);
		if(getVERTEXpred(ptr)) {
			insertVERTEXsuccessor(getVERTEXpred(ptr), ptr);
		}
		setVERTEXflag(ptr, 1);
		neighbors = getVERTEXneighbors(ptr);
		firstDLL(neighbors);
		VERTEX* temp = 0;
		weights = getVERTEXweights(ptr);
		firstDLL(weights);
		INTEGER *ptrWeight = 0;
		while(moreDLL(neighbors)) {
			temp = currentDLL(neighbors);
			ptrWeight = currentDLL(weights);
			if((getINTEGER(ptrWeight) < getVERTEXkey(temp) || (getVERTEXkey(temp) == -1)) && (getVERTEXflag(temp) !=
				1)) {
				setVERTEXkey(temp, getINTEGER(ptrWeight));
				if(getVERTEXflag(temp) == 0) {
					decreaseKeyBINOMIAL(heap, getVERTEXowner(temp), temp);
				}
				//printf("Setting %d predecessor to %d\n\n", getVERTEXnumber(temp), getVERTEXnumber(ptr));
				setVERTEXpred(temp, ptr);
			}
			nextDLL(neighbors);
			nextDLL(weights);
		}
	}	

	//START displaying from this point on
	QUEUE* tempQ = newQUEUE(0, 0);
	enqueue(tempQ,getVERTEXsuccessors(start));
	VERTEX* currVert = start;
	BINOMIAL* currBinom = newBINOMIAL(0, personalCompare, 0, 0);	
	int cnt = 0;

	printf("0: ");
	displayVERTEX(start, stdout);
	int totalWeight = 0;

	while(sizeQUEUE(tempQ) != 0) {	
		DLL* tempDLL = dequeue(tempQ);
		fillUpBinom(currBinom, tempDLL);
		
		if((sizeQUEUE(tempQ) == 0) &&(sizeBINOMIAL(currBinom) > 0)){
			cnt++;
			printf("\n");
			printf("%d: ", cnt);
		}
		if(sizeQUEUE(tempQ) == 0) {
			while(sizeBINOMIAL(currBinom) > 0) {
				currVert = extractBINOMIAL(currBinom);
				enqueue(tempQ, getVERTEXsuccessors(currVert));
				displayVERTEX(currVert, stdout);
				printf("(%d)", getVERTEXnumber(getVERTEXpred(currVert)));
				printf("%d", getVERTEXkey(currVert));
				totalWeight += getVERTEXkey(currVert);
				if(sizeBINOMIAL(currBinom) != 0) {
					printf(" ");
				}
			}
		}
	}
	printf("\n");
	printf("weight: %d\n", totalWeight);

return 0;
}
