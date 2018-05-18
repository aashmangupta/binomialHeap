//
// Created by Aashman Gupta on 4/5/18.
//

#include "binomial.h"
#include "dll.h"
#include <stdlib.h>
#include <math.h>
#include "queue.h"

typedef struct binomNode {
    void* val;
    void* owner;
    struct binomNode* parent;
    DLL* children;
} binomNODE;

struct binomial {
    DLL* rootList;
    int size;
    binomNODE* extreme;
    void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*update)(void *,void *);
    void (*free)(void *);
};

BINOMIAL *newBINOMIAL(void (*display)(void *,FILE *), int (*compare)(void *,void *), void (*update)(void *,void *), void (*free)(void *)) {
    BINOMIAL* b = malloc(sizeof(BINOMIAL));
    b->display = display;
    b->compare = compare;
    b->update = update;
    b->free = free;
    b->rootList = newDLL(0, 0);
    b->extreme = 0;
    b->size = 0;
    return b;
}

static binomNODE* newBinomNODE(void* val) {
    binomNODE* tempNode = malloc(sizeof(binomNODE));

    tempNode->val = val;
    tempNode->parent = 0;
    tempNode->owner = 0;
    tempNode->children = 0;
    return tempNode;
}

static binomNODE* combine(BINOMIAL* b, binomNODE* x, binomNODE* y) {
    if((b->compare(x->val, y->val)) < 0) {
        insertDLL(x->children, sizeDLL(x->children), y);
        y->parent = x;
        y->owner = x->children;
        return x;
    }
    else {
        insertDLL(y->children, sizeDLL(y->children), x);
        x->parent = y;
        x->owner = y->children;
        return y;
    }
}

static void updateConsolidationArray(BINOMIAL* b, binomNODE** array, binomNODE* spot) {
    int degree = sizeDLL(spot->children);
    while(array[degree] != 0) {
        spot = combine(b, spot, array[degree]);
        array[degree] = 0;
        degree++;
    }
    array[degree] = spot;
}

void consolidate(BINOMIAL* b) {
    int sizeArray = log2(b->size) + 1;
    binomNODE* spot;
    binomNODE* array[sizeArray];

    for(int i = 0; i < sizeArray; i++) {    //Initialize array to NULL.
        array[i] = 0;
    }

    //place all the sub-heaps in the D array
    while(sizeDLL(b->rootList) > 0) {
        firstDLL(b->rootList);
        spot = currentDLL(b->rootList);
        removeDLL(b->rootList, 0);
        updateConsolidationArray(b, array, spot);
    }

    //transfer the D array back to the heap
    b->extreme = 0;
    for(int i = 0; i < sizeArray; i++) {
        if(array[i] != 0) {
            insertDLL(b->rootList, sizeDLL(b->rootList), array[i]);
            if((b->extreme == 0) || ((b->compare(array[i]->val, b->extreme->val)) < 0)) {
                b->extreme = array[i];
            }
        }
        if(array[i] != 0) {
            array[i] = 0;
        }
    }
}

void *insertBINOMIAL(BINOMIAL *b, void *value) {
    binomNODE* currNode = newBinomNODE(value);
    currNode->parent = currNode;
    currNode->children = newDLL(0, 0);
    currNode->owner = b->rootList;
    insertDLL(b->rootList, sizeDLL(b->rootList), currNode);
    b->size++;

    consolidate(b);
    return currNode;
}

void displayBINOMIAL(BINOMIAL *b,FILE *fp) {
    if(b == 0) {
        return;
    }
    int size = sizeDLL(b->rootList);

    firstDLL(b->rootList);
    binomNODE* temp;

    fprintf(fp, "rootlist: ");

    for(int i = 0; i < size; i++) {
        temp = currentDLL(b->rootList);
        if(i == sizeDLL(temp->children)) {
            b->display(temp->val, fp);
            if(temp == b->extreme) {
                fprintf(fp, "*");
            }
            nextDLL(b->rootList);
        }
        else {
            fprintf(fp, "NULL");
            size++;
        }
        if(i == size - 1) {
            fprintf(fp, "\n");
        }
        else {
            fprintf(fp, " ");
        }
    }
}

void displayBINOMIALdebug(BINOMIAL *b,FILE *fp) {
    fprintf(fp, "{{");

    QUEUE* queue1 = newQUEUE(0, 0);

    binomNODE* temp;
    firstDLL(b->rootList);

    for(int i = 0; i < sizeDLL(b->rootList); i++) {
        temp = currentDLL(b->rootList);

        if(sizeDLL(temp->children) > 0) {
            enqueue(queue1, temp->children);
        }

        b->display(temp->val, fp);
        if(i != sizeDLL(b->rootList) - 1) {
            fprintf(fp, ",");
        }
        nextDLL(b->rootList);
    }
    fprintf(fp, "}}\n");

    int currSize = sizeQUEUE(queue1);
    int count = 0;

    DLL* currDll;
    while(sizeQUEUE(queue1) > 0) {
        while (currSize > 0) {
            currDll = dequeue(queue1);
            firstDLL(currDll);
            temp = currentDLL(currDll);
            fprintf(fp, "{{");
            count = 0;
            while (temp != 0) {
                b->display(temp->val, fp);
                if(count != sizeDLL(currDll) - 1) {
                    fprintf(fp, ",");
                }
                if (sizeDLL(temp->children) > 0) {
                    enqueue(queue1, temp->children);
                }
                nextDLL(currDll);
                if(moreDLL(currDll)) {
                    temp = currentDLL(currDll);
                }
                else {
                    temp = 0;
                }
                count++;
            }
            fprintf(fp, "}}");
            currSize--;
        }
        fprintf(fp, "\n");
        currSize = sizeQUEUE(queue1);
    }
    freeQUEUE(queue1);
}

int sizeBINOMIAL(BINOMIAL *b) {
    if(b == 0) {
        return 0;
    }
    else {
        return b->size;
    }
}

void *peekBINOMIAL(BINOMIAL *b) {
    if(b == 0) {
        return 0;
    }
    return b->extreme->val;
}

static binomNODE* bubbleUp(BINOMIAL *b, binomNODE* node) {
    binomNODE* p = node->parent;
    if(p == node) {
        return node;
    }

    if(node->val != 0) {
        if (b->compare(node->val, p->val) >= 0) {
            return node;
        }
    }

    if(b->update != 0) {
        b->update(node->val, p);
        b->update(p->val, node);
    }

    void* temp = node->val;
    node->val = p->val;
    p->val = temp;

    return bubbleUp(b, p);
}

void decreaseKeyBINOMIAL(BINOMIAL *b,void *node,void *value) {
    binomNODE* n = node;
    n->val = value;
    binomNODE* result = bubbleUp(b, n);
    if(result->val != 0) {
        if (b->compare(result->val, b->extreme->val) < 0) {
            b->extreme = result;
        }
    }
}

void *extractBINOMIAL(BINOMIAL *b) {
    if(b == 0) {
        return 0;
    }

    if(b->size == 0) {
        return 0;
    }

    binomNODE* y = b->extreme;
    void* retVal = b->extreme->val;

    firstDLL(b->rootList);
    binomNODE* temp0 = currentDLL(b->rootList);
    int count = 0;

    for(int i = 0; i < sizeDLL(b->rootList); i++) {
        if(y == temp0) {
            break;
        }
        count++;
        nextDLL(b->rootList);
        temp0 = currentDLL(b->rootList);
    }

    removeDLL(b->rootList, count);

    firstDLL(y->children);
    binomNODE *temp;

    if(moreDLL(y->children)) {
        temp = currentDLL(y->children);
    }
    else {
        temp = 0;
    }

    for(int i = 0; i < sizeDLL(y->children); i++) {
        temp->parent = temp;
        nextDLL(y->children);
        if(moreDLL(y->children)) {
            temp = currentDLL(y->children);
        }
        else {
            temp = 0;
        }
    }

    unionDLL(y->children, b->rootList);
    unionDLL(b->rootList, y->children);

    consolidate(b);
    b->size--;

    freeDLL(y->children);
    free(y);
    return retVal;
}

void deleteBINOMIAL(BINOMIAL *b,void *node) {
    decreaseKeyBINOMIAL(b, node, NULL);
    extractBINOMIAL(b);
}

void unionBINOMIAL(BINOMIAL *recipient,BINOMIAL *donor) {
    unionDLL(recipient->rootList, donor->rootList);
    recipient->size = recipient->size + donor->size;
    donor->rootList = newDLL(donor->display, donor->free);
    donor->size = 0;
    donor->extreme = 0;
    consolidate(recipient);
}

void statisticsBINOMIAL(BINOMIAL *b,FILE *fp) {
    fprintf(fp, "size: %d\n", b->size);
    fprintf(fp, "rootlist size: %d\n", sizeDLL(b->rootList));
    if(b->size > 0) {
        fprintf(fp, "extreme: ");
        b->display(b->extreme->val, fp);
        fprintf(fp, "\n");
    }
}

static void freeHelper(BINOMIAL* b, DLL* list, QUEUE* queue1) {
    enqueue(queue1, list);

    if(sizeDLL(list) == 0) {
        return;
    }

    firstDLL(list);
    binomNODE* temp = 0;
    while(moreDLL(list) != 0) {
        temp = currentDLL(list);
        if(temp != 0) {
            freeHelper(b, temp->children, queue1);
        }
        b->free(temp->val);
        free(temp);
        nextDLL(list);
    }
}

void freeBINOMIAL(BINOMIAL *b) {
    if(b == 0) {
        return;
    }
    QUEUE* queue1 = newQUEUE(0, 0);
    freeHelper(b, b->rootList, queue1);

    DLL* temp = 0;
    while(sizeQUEUE(queue1) > 0) {
        temp = dequeue(queue1);
        freeDLL(temp);
    }
    freeQUEUE(queue1);
    free(b);
}
