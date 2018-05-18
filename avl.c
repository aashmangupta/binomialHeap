//
// Created by Aashman Gupta on 2/26/18.
//
#include <stdlib.h>
#include <stdio.h>
#include "avl.h"
#include "bst.h"

struct avl {
    BST* tree;
    int size;
    void (*display)(void *,FILE *);
    int (*comparator)(void *,void *);
    void (*free)(void *);
};

typedef struct avlV{
    void* val;
    int leftHeight;
    int rightHeight;
    int height;
    int freq;
    void (*display)(void *,FILE *);
    int (*comparator)(void *,void *);
    void (*free)(void *);
} AVLV;

static void AVLDisplay(void* x, FILE* fp) {
    AVLV* currNode = x;
    currNode->display(currNode->val, fp);

    if(currNode->freq > 1) {
        fprintf(fp, "[%d]", currNode->freq);
    }

    if((currNode->leftHeight - currNode->rightHeight) == 1) {
        fprintf(fp, "+");
    }
    else if((currNode->leftHeight - currNode->rightHeight) == -1) {
        fprintf(fp, "-");
    }
}

static int AVLCompare(void* x, void* y) {
    AVLV* firstNode = x;
    AVLV* secondNode = y;

    return firstNode->comparator(firstNode->val, secondNode->val);
}

static void AVLFree(void* x) {
    AVLV* currNode = x;
    currNode->free(currNode->val);
    free(currNode);
}

//Credit to Dr. Lusth for the function below.
static void AVLswapper(BSTNODE *a,BSTNODE *b) {
    AVLV *ta = getBSTNODEvalue(a);
    AVLV *tb = getBSTNODEvalue(b);

    /* swap the values stored in the AVL value objects */
    void *vtemp = ta->val;
    ta->val = tb->val;
    tb->val = vtemp;

    /* swap the counts stored in the AVL value objects */
    int ctemp = ta->freq;
    ta->freq = tb->freq;
    tb->freq = ctemp;

    /* note: AVL heights and balance factors are NOT swapped */
}

static AVLV* newAVLV(void* val, void (*display)(void*, FILE *), int (*comparator)(void *,void *), void (*free)(void *)) {
    AVLV* currNode = malloc(sizeof(AVLV));
    currNode->val = val;
    currNode->leftHeight = 0;
    currNode->rightHeight = 0;
    currNode->freq = 1;
    currNode->height = 1;
    currNode->display = display;
    currNode->comparator = comparator;
    currNode->free = free;

    return currNode;
}

AVL* newAVL(void (*display)(void *, FILE*), int (*comparator)(void *,void *), void (*free)(void *)) {
    AVL* currTree = malloc(sizeof(AVL));
    currTree->tree = newBST(AVLDisplay, AVLCompare, AVLswapper, AVLFree);
    currTree->size = 0;
    currTree->display = display;
    currTree->comparator = comparator;
    currTree->free = free;

    return currTree;
}

static void setBalance(BSTNODE* x) {
    if(x == 0) {
        return;
    }

    AVLV* passedNode = getBSTNODEvalue(x);
    AVLV* passedNodeLeft = 0;
    AVLV* passedNodeRight = 0;

    BSTNODE* tempNodeLeft = getBSTNODEleft(x);
    BSTNODE* tempNodeRight = getBSTNODEright(x);

    if(tempNodeLeft != 0) {
        passedNodeLeft = getBSTNODEvalue(tempNodeLeft);
    }

    if(tempNodeRight != 0) {
        passedNodeRight = getBSTNODEvalue(tempNodeRight);
    }

    int leftHeight = 0;
    int rightHeight = 0;

    if(passedNodeLeft != 0) {
        leftHeight = passedNodeLeft->height;
    }

    if(passedNodeRight != 0) {
        rightHeight = passedNodeRight->height;
    }

    passedNode->leftHeight = leftHeight;
    passedNode->rightHeight = rightHeight;

    if(leftHeight > rightHeight) {
        passedNode->height = leftHeight + 1;
    }
    else {
        passedNode->height = rightHeight + 1;
    }
}

static BSTNODE* findFavorite(BSTNODE* parent) {
    if(parent == 0) {
        return 0;
    }

    AVLV* parentBalance = getBSTNODEvalue(parent);
    if((parentBalance->leftHeight - parentBalance->rightHeight) == 1) {
        return getBSTNODEleft(parent);
    }

    else if((parentBalance->leftHeight - parentBalance->rightHeight) == -1) {
        return getBSTNODEright(parent);
    }

    return 0;
}

static int areLinear(BSTNODE* p, BSTNODE* x, BSTNODE* y) {
    if(!p || !x || !y) {
        return 0;
    }

    if(getBSTNODEleft(p) == x) {
        if(getBSTNODEleft(x) == y) {
            return 1;
        }
    }

    if(getBSTNODEright(p) == x) {
        if(getBSTNODEright(x) == y) {
            return 1;
        }
    }

    return 0;
}

static void rotate(AVL* avlTree, BSTNODE* child, BSTNODE* parent) {
    BSTNODE* grandParent = getBSTNODEparent(parent);

    if(grandParent != 0) {
        if(parent == getBSTNODEleft(grandParent)) {
            setBSTNODEleft(grandParent, child);
            setBSTNODEparent(child, grandParent);
        }
        else {
            setBSTNODEright(grandParent, child);
            setBSTNODEparent(child, grandParent);
        }
    }

    if(grandParent == 0) {
        setBSTroot(avlTree->tree, child);
        setBSTNODEparent(child, 0);
    }

    BSTNODE* temp = 0;

    if(getBSTNODEleft(parent) == child) {
        temp = getBSTNODEright(child);
        setBSTNODEright(child, parent);
        setBSTNODEparent(parent, child);

        setBSTNODEleft(parent, temp);
        if(temp != 0) {
            setBSTNODEparent(temp, parent);
        }
    }
    else {
        temp = getBSTNODEleft(child);
        setBSTNODEleft(child, parent);
        setBSTNODEparent(parent, child);
        setBSTNODEright(parent, temp);
        
        if(temp != 0) {
            setBSTNODEparent(temp, parent);
        }
    }
}

static void insertionFixUp(AVL* avlTree, BSTNODE* insertedNode) {
    AVLV* parentBalance = 0;
    while (1) {
        BSTNODE* parentInserted = getBSTNODEparent(insertedNode);

        if(getBSTroot(avlTree->tree) == insertedNode) {
            setBSTNODEparent(insertedNode, 0);
            return;
        }

        parentBalance = getBSTNODEvalue(parentInserted);
        BSTNODE* parentFav = findFavorite(parentInserted);

        if((parentFav != 0) && (insertedNode != parentFav)) {
            setBalance(parentInserted);
            return;
        }

        else if((parentBalance->leftHeight - parentBalance->rightHeight) == 0) {
            setBalance(parentInserted);
            insertedNode = parentInserted;
        }

        else {
            BSTNODE* y = findFavorite(insertedNode);
            BSTNODE* p = getBSTNODEparent(insertedNode);

            if((y != 0) && (areLinear(p, insertedNode, y) == 0)) {
                rotate(avlTree, y, insertedNode);
                rotate(avlTree, y, p);
                setBalance(insertedNode);
                setBalance(p);
                setBalance(y);
            }
            else {
                rotate(avlTree, insertedNode, p);
                setBalance(p);
                setBalance(insertedNode);
            }
            return;
        }
    }
}

void insertAVL(AVL *avlTree, void *val) {
    if (avlTree == 0) {
        return;
    }
    avlTree->size++;

    AVLV* insertNode = newAVLV(val, avlTree->display, avlTree->comparator, avlTree->free);
    BSTNODE* tempNode = findBST(avlTree->tree, insertNode);
    AVLV* foundVal = 0;

    if(tempNode != 0) {
        foundVal = getBSTNODEvalue(tempNode);
    }

    if(foundVal != 0) {
        free(insertNode);
        foundVal->freq++;
        return;
    }
    //BELOW: If the node already doesn't exist
    BSTNODE* insertedNode = insertBST(avlTree->tree, insertNode);
    insertionFixUp(avlTree, insertedNode);
    return;
}

BSTNODE* getSibling(BSTNODE* x) {
    BSTNODE* parent = getBSTNODEparent(x);
    if(parent == 0) {
        return 0;
    }

    else if(getBSTNODEleft(parent) == x) {
        return getBSTNODEright(parent);
    }

    else {
        return getBSTNODEleft(parent);
    }
}

static void deletionFixUp(AVL* currTree, BSTNODE* x) {
    AVLV* xVal = getBSTNODEvalue(x);
    //AVLV* parentBalance = 0;
    xVal->height = 0;
    xVal->leftHeight = 0;
    xVal->rightHeight = 0;

    while(1) {
        BSTNODE* parent = getBSTNODEparent(x);

        if(getBSTroot(currTree->tree) == x) {
            return;
        }

        //parentBalance = getBSTNODEvalue(parent);
        BSTNODE* parentFav = findFavorite(parent);

        if(parentFav == x) {
            setBalance(parent);
            x = parent;
        }

        else if(parentFav == 0) {
            setBalance(parent);
            return;
        }

        else {
            BSTNODE* p = getBSTNODEparent(x);
            BSTNODE* z = getSibling(x);
            BSTNODE* y = 0;
            if(z != 0) {
                y = findFavorite(z);
            }

            if((y != 0) && (areLinear(p, z, y) == 0)) {
                rotate(currTree, y, z);
                rotate(currTree, y, p);
                setBalance(p);
                setBalance(z);
                setBalance(y);
                x = y;
            }

            else {
                rotate(currTree, z, p);
                setBalance(p);
                setBalance(z);
                if(y == 0) {
                    return;
                }
                x = z;
            }
        }

    }
}

void *deleteAVL(AVL *avlTree,void *val) {
    if(avlTree == 0) {
        return 0;
    }

    AVLV* deleteNode = newAVLV(val, avlTree->display, avlTree->comparator, avlTree->free);
    BSTNODE* foundVal = findBST(avlTree->tree, deleteNode);

    free(deleteNode);
    if(foundVal == 0) {
        return 0;
    }

    avlTree->size--;

    deleteNode = getBSTNODEvalue(foundVal);
    void* retVal = deleteNode->val;

    if(deleteNode->freq > 1) {
        deleteNode->freq--;
        return 0;
    }

    foundVal = swapToLeafBST(avlTree->tree, foundVal);
    deletionFixUp(avlTree, foundVal);
    pruneLeafBST(avlTree->tree, foundVal);

    AVLV* deleteBal = getBSTNODEvalue(foundVal);

    free(foundVal);
    free(deleteBal);

    int oldSize = sizeBST(avlTree->tree);
    setBSTsize(avlTree->tree, oldSize - 1);

    return retVal;
}

void displayAVL(AVL *currTree, FILE *fp) {
    if(currTree == 0) {
        fprintf(fp, "EMPTY\n");
        return;
    }
    if(currTree->size == 0) {
        fprintf(fp, "EMPTY\n");
        return;
    }
    displayBSTdecorated(currTree->tree, fp);
}

int findAVLcount(AVL *avlTree, void *val) {
    if(avlTree == 0) {
        return 0;
    }

    AVLV* currNode = newAVLV(val, avlTree->display, avlTree->comparator, avlTree->free);
    BSTNODE* tempNode = findBST(avlTree->tree, currNode);
    AVLV* foundVal = 0;

    if(tempNode != 0) {
        foundVal = getBSTNODEvalue(tempNode);
    }

    free(currNode);

    if(foundVal == 0) {
        return 0;
    }

    return foundVal->freq;
}

void *findAVL(AVL *avlTree, void *val) {
    if(avlTree == 0) {
        return 0;
    }

    AVLV* currNode = newAVLV(val, avlTree->display, avlTree->comparator, avlTree->free);
    BSTNODE* tempNode = findBST(avlTree->tree, currNode);
    AVLV* foundVal = 0;

    if(tempNode != 0) {
        foundVal = getBSTNODEvalue(tempNode);
    }

    free(currNode);

    if(foundVal == 0) {
        return 0;
    }

    return foundVal->val;
}

int sizeAVL(AVL *avlTree) {
    if(avlTree == 0) {
        return 0;
    }
    else {
        return sizeBST(avlTree->tree);
    }
}

int duplicatesAVL(AVL *avl1) {
    if(avl1 == 0) {
        return 0;
    }
    int bstSize = sizeBST(avl1->tree);
    return avl1->size - bstSize;
}

void statisticsAVL(AVL *currTree,FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", duplicatesAVL(currTree));
    statisticsBST(currTree->tree, fp);
}

void displayAVLdebug(AVL *currTree,FILE *fp) {
    if(currTree == 0) {
        return;
    }
    displayBST(currTree->tree, fp);
}

void freeAVL(AVL *currTree) {
    freeBST(currTree->tree);
    free(currTree);
}
