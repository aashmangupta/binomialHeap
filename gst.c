//
// Created by Aashman Gupta on 2/26/18.
//

#include "gst.h"
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct gstV {
    void* val;
    int freq;
    void (*display)(void *,FILE *);
    int (*comparator)(void *,void *);
    void (*free)(void *);
} GSTV;

struct gst {
    BST* tree;
    int size;
    void (*display)(void *,FILE *);
    int (*comparator)(void *,void *);
    void (*free)(void *);
};

static int GSTVCompare(void* x, void* y) {
    GSTV* firstNode = x;
    GSTV* secondNode = y;

    return firstNode->comparator(firstNode->val, secondNode->val);
}

static void GSTVDisplay(void* x, FILE* fp) {
    GSTV* currNode = x;
    currNode->display(currNode->val, fp);

    if(currNode->freq > 1) {
        fprintf(fp, "[%d]", currNode->freq);
    }
}

static void GSTVFree(void* x) {
    GSTV* currNode = x;
    currNode->free(currNode->val);
    free(currNode);
}

static GSTV* newGSTV(void* val, void (*display)(void*, FILE *), int (*comparator)(void *,void *), void (*free)(void *)) {
    GSTV* currNode = malloc(sizeof(GSTV));
    currNode->val = val;
    currNode->freq = 1;
    currNode->display = display;
    currNode->comparator = comparator;
    currNode->free = free;

    return currNode;
}

GST* newGST(void (*display)(void *, FILE*), int (*comparator)(void *,void *), void (*free)(void *)) {
    GST* currTree = malloc(sizeof(GST));
    currTree->tree = newBST(GSTVDisplay, GSTVCompare, 0, GSTVFree);
    currTree->size = 0;
    currTree->display = display;
    currTree->comparator = comparator;
    currTree->free = free;
    return currTree;
}

void insertGST(GST *greenTree,void *val) {
    if(greenTree == 0) {
        return;
    }
    greenTree->size++;

    GSTV* currNode = newGSTV(val, greenTree->display, greenTree->comparator, greenTree->free);
    BSTNODE* tempNode = findBST(greenTree->tree, currNode);
    GSTV* greenNode = 0;

    if(tempNode != 0) {
        greenNode = getBSTNODEvalue(tempNode);
    }

    if(greenNode != 0) {
        greenNode->freq++;
        currNode->free(currNode->val);
        free(currNode);
        return;
    }

    insertBST(greenTree->tree, currNode);
}

int findGSTcount(GST *greenTree, void *val) {
    if(greenTree == 0) {
        return 0;
    }
    GSTV* currNode = newGSTV(val, greenTree->display, greenTree->comparator, greenTree->free);
    BSTNODE* tempNode = findBST(greenTree->tree, currNode);
    GSTV* greenNode= 0;

    if(tempNode != 0) {
        greenNode = getBSTNODEvalue(tempNode);
    }

    free(currNode);

    if(greenNode == 0) {
        return 0;
    }
    return greenNode->freq;
}

void *findGST(GST *greenTree,void *val) {
    if(greenTree == 0) {
        return 0;
    }
    GSTV* currNode = newGSTV(val, greenTree->display, greenTree->comparator, greenTree->free);
    BSTNODE* tempNode = findBST(greenTree->tree, currNode);
    GSTV* greenNode = 0;

    if(tempNode != 0) {
        greenNode = getBSTNODEvalue(tempNode);
    }

    free(currNode);

    if(greenNode == 0) {
        return 0;
    }
    else {
        return greenNode->val;
    }
}

void *deleteGST(GST *greenTree, void *val) {
    if(greenTree == 0) {
        return 0;
    }

    GSTV* currNode = newGSTV(val, greenTree->display, greenTree->comparator, greenTree->free);
    BSTNODE* tempNode = findBST(greenTree->tree, currNode);
    GSTV* greenNode = 0;

    free(currNode);

    if(tempNode == 0) {
        return 0;
    }

    greenTree->size--;

    greenNode = getBSTNODEvalue(tempNode);

    if(greenNode == 0) {
        return 0;
    }

    if(greenNode->freq > 1) {
        greenNode->freq--;
        return 0;
    }

    BSTNODE* deletedNode = deleteBST(greenTree->tree, greenNode);
    GSTV* deletedGSTNode = getBSTNODEvalue(deletedNode);
    void* retVal = deletedGSTNode->val;

    free(deletedGSTNode);
    free(deletedNode);

    return retVal;
}

int sizeGST(GST *greenTree) {
    if(greenTree == 0) {
        return 0;
    }
    return sizeBST(greenTree->tree);
}

int duplicates(GST *greenTree) {
    if(greenTree == 0) {
        return 0;
    }
    int sizeGreen = greenTree->size;
    int bstSize = sizeBST(greenTree->tree);

    return sizeGreen - bstSize;
}

void statisticsGST(GST *greenTree,FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", duplicates(greenTree));
    statisticsBST(greenTree->tree, fp);
}

void displayGST(GST *greenTree,FILE *fp) {
    if(greenTree == 0) {
        fprintf(fp, "EMPTY\n");
        return;
    }
    if(greenTree->size == 0) {
        fprintf(fp, "EMPTY\n");
        return;
    }
    displayBSTdecorated(greenTree->tree, fp);
}

void displayGSTdebug(GST *greenTree, FILE *fp) {
    displayBST(greenTree->tree, fp);
}

void freeGST(GST *greenTree) {
    freeBST(greenTree->tree);
    free(greenTree);
}

