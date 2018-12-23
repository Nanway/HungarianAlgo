// Same as BSTree.c but slightly different
// as the values stored are different -> we store a float value inside
// CREDITS TO UNSW COMP2521 labs for their implementation of trees in C

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "FloatBST.h"
#include "strdup.h"

typedef struct floatBSTNode *floatBSTLink;

typedef struct floatBSTNode {
	char *key;
	int count;
	float rankVal;
	floatBSTLink left, right;
    int height;
} floatBSTNode;

void copyToArray(floatBSTNode *node, URLArray u);

static
int max(int a, int b) 
{ 
    return (a > b)? a : b; 
} 
  
FloatBST newFloatBST() {
	return NULL;
}

// make a new node containing a key
static
floatBSTLink newfloatBSTNode(char *key, float rankVal)
{
	floatBSTLink new = malloc(sizeof(floatBSTNode));
	assert(new != NULL);
	new->left = new->right = NULL;
    new->key = strdup(key);
    new->height = 1;
    new->count = 1;
    new->rankVal = rankVal;
	return new;
}

// free memory associated with FloatBST
void dropFloatBST(FloatBST t)
{
	if (t == NULL) return;
	dropFloatBST(t->left);
	dropFloatBST(t->right);
	free(t->key);
	free(t);
}


// insert a new key into a FloatBST
FloatBST floatBSTInsert(FloatBST t, char *key, float rankVal)
{
	if (t == NULL) {
		return newfloatBSTNode(key, rankVal);
    }

    // Insert the new key
	int cmp = strcmp(key, t->key);
	if (cmp < 0) {
		t->left = floatBSTInsert(t->left, key, rankVal);
    } else if (cmp > 0) {
		t->right = floatBSTInsert(t->right, key, rankVal);
    } else if (cmp == 0) {
	    return t;
    }

    // Update node height
    t->height = 1 + max(f_height(t->left), f_height(t->right));
    int balance = f_height(t->left) - f_height(t->right);
    // Rotate tree to maintain balance
    // LL
    if (balance > 1 && strcmp(key, t->left->key) < 0) {
        return f_rightRotate(t);
    // RR
    } else if (balance < - 1 && strcmp(key, t->right->key) > 0) {
        return f_leftRotate(t);
    // LR
    } else if (balance > 1 && strcmp(key, t->left->key) > 0) {
        t->left = f_leftRotate(t->left);
        return f_rightRotate(t);
    // RL
    } else if (balance < - 1 && strcmp(key, t->right->key) < 0) {
        t->right = f_rightRotate(t->right);
        return f_leftRotate(t);
    }
    // no rebalancing neeeded
    return t;
}

// check whether a key is in a FloatBST
int floatBSTFind(FloatBST t, char *v)
{
	if (t == NULL)
		return 0;
	int cmp = strcmp(v, t->key);
	if (cmp < 0)
		return floatBSTFind(t->left, v);
	else if (cmp > 0)
		return floatBSTFind(t->right, v);
	else // (v == t->key)
		return 1;
}


// Updates the number of nodes
int findAndUpdateCount(FloatBST t, char *key) {
    if (t == NULL)
        return 0;
    int cmp = strcmp(key, t->key);
    if (cmp < 0) {
        return findAndUpdateCount(t->left, key);
    } else if (cmp > 0) {
        return findAndUpdateCount(t->right, key);
    } else {
        t->count++;
        return 1;
    }
}
// Updates the rank value
int findAndUpdateRank(FloatBST t, char *key, float rankVal) {
    if (t == NULL)
        return 0;
    int cmp = strcmp(key, t->key);
    if (cmp < 0) {
        return findAndUpdateRank(t->left, key, rankVal);
    } else if (cmp > 0) {
        return findAndUpdateRank(t->right, key, rankVal);
    } else {
        t->rankVal = t->rankVal + rankVal;
        return 1;
    }
}

void printFloatBST(FILE *outFile, FloatBST node) {
    if (node == NULL) {
        return;
    }
    printFloatBST(outFile, node->left);
    fprintf(stdout, "Key %s with count %d and pr %f\n", node->key,
        node->count, node->rankVal);
    printFloatBST(outFile, node->right);
}

// Traverse 
URLArray convertToArray(FloatBST t) {
    if (t == NULL) {
        return NULL;
    }

    int numNodes = floatBSTNumNodes(t);
    URLArray u = newURLArray(numNodes);
    copyToArray(t, u);
    return u;

}

void copyToArray(floatBSTNode *node, URLArray u) {
    if (node == NULL) {
        return;
    }

    copyToArray(node->left, u);
    URLInsert(u, node->key, node->count, node->rankVal);
    copyToArray(node->right, u);
}

// count #nodes in FloatBST
int floatBSTNumNodes(FloatBST t)
{
    if (t == NULL)
        return 0;
    else
        return 1 + floatBSTNumNodes(t->left)
                 + floatBSTNumNodes(t->right);
}

// Height of FloatBST
int f_height(FloatBST t) {
    if (t == NULL) {
        return 0;
    }

    return t->height;
}

// Left rotate
FloatBST f_leftRotate(FloatBST t) {
    FloatBST right = t->right;
    if (right == NULL || t == NULL) {
        return t;
    }
    FloatBST rightL = right->left;
    t->right = rightL;
    right->left = t;

    // Update heights
    t->height = max(f_height(t->right), f_height(t->left)) + 1;
    right->height = max(f_height(right->right), f_height(right->left))
                        + 1;
    return right;
}

// Right rotate
FloatBST f_rightRotate(FloatBST t) {
    FloatBST left = t->left;
    if (left == NULL || t == NULL) {
        return t;
    }
    FloatBST leftR = left->right;
    t->left = leftR;
    left->right = t;

    // Update f_heights
    t->height = max(f_height(t->right), f_height(t->left)) + 1;
    left->height = max(f_height(left->right), f_height(left->left)) + 1;
    return left;
}


