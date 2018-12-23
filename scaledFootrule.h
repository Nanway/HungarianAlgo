// Header file for scaled foot rule

#ifndef SCALEDFOOTRULE_H
#define SCALEDFOOTRULE_H
#include "URLArray.h"
#include "FloatBST.h"
#include "bipartite.h"
#define MAX_SIZE 1024

// Populates an AVL tree with the URLs (uses the tree as a set)
// Also populates a URL Array with the URLs in their given rank order
FloatBST createRankList(char *file, FloatBST set, URLArray toPopulate);
// Reads the input files and plugs them into the tree set and rank 
// matrix
URLArray readFiles(int numFiles, char **files, URLArray *rankMatrix);
// Calcualtes the footrule distance between each URL and position
// in the bipartite graph
void calculateFRD(URLArray *rankMatrix, BiGraph g, int numFiles);
// Performs the hungarian algorithm
URLArray findMinCost(BiGraph g);
// Does the step 1/2 of the algorithm (setup work)
void hungarianSetup(BiGraph g);
// Marks the vertices/ positions that correspond to highlighted
// rows/ cols
void markMatrix(BiGraph g, int *vertToPosMatch, int *posToVertMatch,
	int *markedCols, int *unmarkedRows);
// Find the next best minimum cost using the marked matrix
float  findNextMin(BiGraph g, int *markedCols, int *unmarkedRows);
// Using logic outlined in the C file, this recalcualtes the weights
// of the matrix when a next best cost is found
void recalculateMatrix(BiGraph g, int *markedCols, int *unmarkedRows,
	float nextBestMin);
// Recursive function that marks columns then rows then columns of that 
// row
void markColumns(BiGraph g, Vert row, int *posToVertMatch,
	int *markedCols, int *markedRows);


#endif