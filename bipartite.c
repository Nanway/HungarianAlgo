// Bipartite Graph
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "bipartite.h"
#include "searchAndSort.h"
#include <float.h>
#include "strdup.h"

#define strEQ(g,t) (strcmp((g),(t)) == 0)

typedef struct bipartiteG {
    // Variable to denote where we have inserted up to 
    int urlArrayIndex;

    // number of vertices
    int maxURLs;

    // array of urls to map URL to ID
    char **urlArray;

    // Adjacency rep
    float **adjMatrix;
} BipartiteGraphRep;

// Helper functions
Vert newUrl(BiGraph, char *);
void graphNull(BiGraph);

BiGraph newBG(int numPos) {
    BiGraph newG = malloc(sizeof(BipartiteGraphRep));
    assert(newG != NULL);
    newG->maxURLs = numPos;
    newG->urlArrayIndex = 0;

    // Set urlArray to be nulls
    // This array is essentially a mapping from URL name to ID
    newG->urlArray = malloc(numPos*sizeof(char *));
    assert(newG->urlArray != NULL);
    for (int i =0; i < numPos; i++) {
        newG->urlArray[i] = NULL;
    }

    // Set adjacency matrix to be zero
    newG->adjMatrix = malloc(numPos*sizeof(Vert *));
    assert(newG->adjMatrix != NULL);
    for (int i = 0; i < numPos; i++) {
        newG->adjMatrix[i] = calloc(numPos, sizeof(Vert));
        assert(newG->adjMatrix[i] != NULL);
    }

    return newG;
}

void  deleteBG(BiGraph toDelete) {
    if (toDelete == NULL) return;
    // Remove adjacency matrix
    for (int i = 0; i < toDelete->maxURLs; i++) {
        free(toDelete->adjMatrix[i]);

    }
    free(toDelete->adjMatrix);

    // Remove url array
    for (int i = 0; i < toDelete->maxURLs; i++) {
        free(toDelete->urlArray[i]);
    }

    free(toDelete->urlArray);
    // Remove graph
    free(toDelete);
}

int addWeight(BiGraph g, char *src, Pos dest, float weight) {
    graphNull(g);

    Vert srcID = convURLtoID(g, src);
    if (srcID == -1) srcID = newUrl(g, src);

    // Check we haven't exceeded graph size
    // THIS SHOULDNT HAPPEN
    if (srcID == -1) {
        fprintf(stderr, "Too many URLs?\n");
    }

    // Subtract 1 as the position index = position - 1 in the adjacency
    // matrix
    g->adjMatrix[srcID][dest] = g->adjMatrix[srcID][dest] + weight;
    return 1;
}

int numPositions(BiGraph g) {
    return g->urlArrayIndex;
}

void showBiGraph(BiGraph g, int mode) {
    graphNull(g);
    if (g->urlArrayIndex == 0)
        printf("Graph is empty\n");
    else {
        printf("Graph has %d vertices:\n",g->urlArrayIndex);
        printf("URL to URL ID\n");
        for (int i = 0; i < g->urlArrayIndex; i++) {
            printf("%s has ID %d\n", g->urlArray[i], i);
        }
        int i, j;
        if (mode == 1) {
            for (i = 0; i < g->urlArrayIndex; i++) {
                for (j = 0; j < g->urlArrayIndex; j++)
                    printf(" %f ",g->adjMatrix[i][j]);
                putchar('\n');
            }
        }
        else {
            for (i = 0; i < g->urlArrayIndex; i++) {
                printf("Vertex: %s\n", g->urlArray[i]);
                printf("connects to\n");
                for (j = 0; j < g->urlArrayIndex; j++) {
                    if (g->adjMatrix[i][j] != 0)
                        printf("   %s\n",g->urlArray[j]);
                }
            }
        }
    }
}

Vert newUrl(BiGraph g, char *newURL) {
    graphNull(g);
    if (g->urlArrayIndex == g->maxURLs) return -1;
    g->urlArray[g->urlArrayIndex++] = strdup(newURL);
    return g->urlArrayIndex - 1;
}

Vert convURLtoID(BiGraph g, char *url) {
    graphNull(g);
    Vert retID = binarySearchURL(g->urlArray, url,0, g->urlArrayIndex);
    //for (int i = 0; i < g->urlArrayIndex; i++) {
    //    if (strcmp(url, g->urlArray[i]) == 0) return i;
    //}
    return retID;
}

char *convIDToURL(BiGraph g, Vert x) {
	if (g == NULL) {
		return NULL;
	} else if (x > numPositions(g)) {
		return NULL;
	}
	return strdup(g->urlArray[x]);
}

void graphNull(BiGraph g) {
    if (g == NULL) {
        fprintf(stderr, "Graph was null\n");
        abort();
    }
}

float getWeight(BiGraph g, Vert v, Pos p) {
	if (g == NULL || v >= numPositions(g) || p > numPositions(g)) {
		return -1;
	}

	return g->adjMatrix[v][p];
}

void setWeight(BiGraph g, Vert v, Pos p, float val) {
	if (g == NULL || v >= numPositions(g) || p > numPositions(g)) {
		return;
	}

	g->adjMatrix[v][p] = val;
}

void subtractWeight(BiGraph g, Vert v, Pos p, float val) {
	if (g == NULL || v >= numPositions(g) || p > numPositions(g)) {
		return;
	}

	g->adjMatrix[v][p] = g->adjMatrix[v][p] - val;
}

void increaseWeight(BiGraph g, Vert v, Pos p, float val) {
	if (g == NULL || v >= numPositions(g) || p > numPositions(g)) {
		return;
	}

	g->adjMatrix[v][p] = g->adjMatrix[v][p] + val;
}

// Find the minimum position index for a given vertex
Pos findMinPos(BiGraph g, Vert v) {
	if (g == NULL || v >= numPositions(g)) {
		return -1;
	}

	int minIndex = 0;
	float minVal = g->adjMatrix[v][0];
	for (int i = 0; i < numPositions(g); i++) {
		if (g->adjMatrix[v][i] < minVal) {
			minIndex = i;
			minVal = g->adjMatrix[v][i];
		}
	}

	return minIndex;
}

// Finds the minimum vertex for a given position
// however if a zero already exists in the column
// then we skip
Vert findMinVert(BiGraph g, Pos p) {
	if (g == NULL || p > numPositions(g)) {
		return -1;
	}

	// Find if there's a zero in the column already
	for (int i = 0; i <numPositions(g); i++) {
		if (g->adjMatrix[i][p] < FLT_EPSILON) {
			return -1;
		}
	}

	int minIndex = 0;
	float minVal = g->adjMatrix[0][p];
	for (int i = 0; i < numPositions(g); i++) {
		if (g->adjMatrix[i][p] < minVal) {
			minIndex = i;
			minVal = g->adjMatrix[i][p];
		}
	}
	return minIndex;
}


int findMaximalMatching(BiGraph g, int *posToVertMatch,
    int *vertToPosMatch) {

	int numPos = numPositions(g);
	// Denotes which vertex a position has been matched to
	// The other array is the same thing but other way round
	for (int i = 0; i < numPos; i++) {
		posToVertMatch[i] = -1;
		vertToPosMatch[i] = -1;
	}
	// Loop through each Vertex and try find a matching position
	for (Vert v = 0; v < numPos; v++) {
		// Initialsie an array of positions that a vertex has seen 
        // (so we dont revisit)
		int seenPositions[numPos];
		for (int i = 0; i < numPos; i++) seenPositions[i] = -1;
		// Use a dfs to find out whether the vertex can match a position
		canMatch(g, v, seenPositions, posToVertMatch, vertToPosMatch);
	}

	// Count number of matches found
	int numMatches = 0;
	for (int i = 0; i < numPos; i++) {
		if(posToVertMatch[i] != -1) {
			numMatches++;
		}
	}
	return numMatches;
}

int canMatch(BiGraph g, Vert v, int *seen, int *matches, 
	int *v2pMatch) {

	// Try to match jobs (the edges)
	for (Pos i = 0; i < numPositions(g); i++) {
		// If we have already tried matching to this position continue
		if (seen[i] != -1) continue;
		// Only try matching 0 cost edges
		if (g->adjMatrix[v][i] > FLT_EPSILON) continue;
		// Mark the position as seen now
		seen[i] = v;
		// If the position hasn't been matched then match it
		// else see if the vertex that has already been matched 
		// can be matched to something else instead. If we can
		// set the Position to match to Vertex. Else try another
		// edge or until we fail
		if (matches[i] < 0 || 
			canMatch(g, matches[i], seen, matches, v2pMatch)) {

			matches[i] = v;
			v2pMatch[v] = i;
			return 1;
		}
	}
	// If we get to here, we couldn't match any edge so return 0
	return 0;
}
// Old trash
/*    for (int i = 0; i < g->urlArrayIndex; i++) {
        if (strcmp(url, g->urlArray[i]) == 0) return i;
    }
    return - 1;
}

void getNumLinks(Vertex **refMat, Vertex *numLinkArray, int nV) {
    for (int i = 0; i < nV; i++) {
        numLinkArray[i] = 0;
        for (int j = 0; refMat[i][j] != END_OF_REF; j++) {
            numLinkArray[i] = j + 1;
        }
    }
}
*/
