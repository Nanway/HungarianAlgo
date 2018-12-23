// This BST is slightly different to the one in FloatBST
// as each node stores integer/ float values as opposed to
// another t of URLs
#ifndef FloatBST_H
#define FloatBST_H
#include "URLArray.h"

typedef struct floatBSTNode *FloatBST;

FloatBST newFloatBST();
// f memory associated with FloatBST
void dropFloatBST(FloatBST);
// Get height of a FloatBST
int f_height(FloatBST);
// insert a new value into a FloatBST
FloatBST floatBSTInsert(FloatBST, char *, float);
// Prints a binary search t in LNR order
void printFloatBST(FILE *outFile, FloatBST node);
// Find number of nodes
int floatBSTNumNodes(FloatBST t);
// Left rotate
FloatBST f_leftRotate(FloatBST);
// Right rotate
FloatBST f_rightRotate(FloatBST);
// Updates the number of nodes
int findAndUpdateCount(FloatBST t, char *);
// Updates the rank value
int findAndUpdateRank(FloatBST t, char *, float);
// Converts the contents of a BST to a URL array
URLArray convertToArray(FloatBST t);


// check whether a key is in a FloatBST
int floatBSTFind(FloatBST t, char *key);
#endif