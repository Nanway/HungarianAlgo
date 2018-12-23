
// Scaled foot rule
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include "scaledFootrule.h"
#include <math.h>
#include <float.h>
// REFERENCES:
// Of course this algorithm wasn't invented by me (if you wanted to 
// see my attempt at it then check the bottom document which outlines
// my initial thought at approaching step 4 of the below)

// The following were used for further theoretical understanding
// on how to solve the problem
// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.28.8702&rep=rep1&type=pdf
// https://www.geeksforgeeks.org/maximum-bipartite-matching/
// https://en.wikipedia.org/wiki/Hungarian_algorithm

// ALGORITHM OVERVIEW
// Note that the pseudocode on wikipedia was the one i ended up
// following as it was the easiest to understand (in the matrix
// interpretation section of the page)

// 1. Create a 2D array to store each file's ranking of URLs
	// FILEA -> rankings
	// FILEB -> rankings 
	// FILE C -> rankings
	// e.g. rankA.txt -> [url1 url4 url7 NULL]

	// At the same time, this also creates a set of all urls implemented
	// as a binary search tree. This is then converted into an array 
	// afterwards to map the URL names to integers representing
	// vertices in a graph

// 2. From the set of all URLs, create a bipartite graph 
// 	  (using adjacency matrix):
//		URL 	Position
//		URL 	Position
//		URL 	Position
//		URL 	Position
// 3. We now fully connect the bipartite graph i.e. for each URL we 
// 	  calculate the scaled footrule if it was in position 1 2 3 4 etc.
//		so in essence the edges represent scaled footrule distances
// 		to each position for each URL
// 4. So now essentially what we have is a bipartite graph with 
//    urls on the left and positions on the right. Connecting the two
//    sides are edges that are labelled with cost. We want to build 
//    a subgraph in such a way that all urls are uniquely matched to 
//    a position with minimum total cost. 

//    By framing our problem like this, we have reduced the search space
//    as we are no longer generating and testing at random but we know
//    the costs for matching URL to Position and can somehow use this
//    to build our answer from ground up instead of randomly searching.

//To solve this we shall use the !!!hungarian algorithm.!!!!

// HUNGARIAN ALGORITHM
// In this we will jump between thinking of our problem in terms of a
// bipartite graph as well as in terms of a matrix where the rows
// are the URLs, columns are the positions and the values are the costs.

// Step 1: Normalise the rows and columns. We want to preprocess 
// our matrix or graph so that for each URL, we select the minimum
// cost edge and subtract it from all other edges connected to that url
// i.e. normalise each row in the matrix by subtracting the min cost
// from all other costs in the row, By doing this we set the edges
// with minimum cost to be 0 and everything else is relative to the min
// cost. This normalising allows us to compare between URLs since 
// URL1 might have min weight 1000 whislt URL2 has min weight 10 but 
// by normalising we know that 0 = min weight and we don't have to think
// about the values. 

// We now do the same thing for the columns/ positions. If each column
// has yet to have a 0, then we find the min and subtract from each
// element in the column.

// Step 2: Find maximal matching
// Using only 0 cost edges, we see if we can build a subgraph such that
// every URL is matched to a position. To do this, we perform something
// similar to a DFS on our graph. We go through the URLs and for each 
// URL we select a 0 cost position. If that position has been selected
// by another URL we goto that URL (using recursion) and see if we 
// can select another position instead. By keeping track of which 
// URLs/ Positions we have tried moving, we can get the maximal matching
// in this bipartite graph.

// If we can build a subgraph with perfect matching, then problem solved
// and we display our answer. If not, we have to select the next least
// cost edge. To do this, we cover the 0 cost elements of our matrix 
// with the minimum number of rows/ columns. From the uncovered elements
// we find the lowest value and subtract from everything except covered
// elements. We also add the lowest value to double covered elements.
// The reason we do this instead of just finding hte min cost is because
// sometimes the minimum cost doesn't lead to an optimal solution
// Having found the next best cost and editing our matrix/ edge values
// we now perform the first part of step 2 again. Rinse and repeat 
// until we obtain perfection matching. 

// 5. Display the results by matching the vertex IDs back to a string,
// summing up the cost values in a corresponding matrix and then 
// dispalying.


int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "Usage ./scaledFootrule FILE1 ...");
		exit(EXIT_FAILURE);
	}

	// READ FILES
	// Number of files = argument count - 1(ignore the ./scaledFootrule)
	int numFiles = argc - 1;

	// Create a rank matrix where we will store each files ranking
	// of the URLs
	URLArray *rankMatrix = malloc((numFiles) * sizeof(URLArray));

	// Populates the rank matrix, generate a set of URLs to be ranked
	URLArray urlSet = readFiles(numFiles, argv + 1, rankMatrix);

	// Creates a weighted bipartite graph
	BiGraph g = newBG(length(urlSet));

	// Initialises the graph with the urls
	for (int i =0; i < length(urlSet); i++) {
		for (int j = 0; j < length(urlSet); j++) {
			addWeight(g, getURLName(urlSet, i), j, 0);
		}
	}
	dropURLArray(urlSet);
	// For each element calculate the footrule distance
	// for each potential position
	// in essence -> populate the graph with the weights
	calculateFRD(rankMatrix, g, numFiles);
	//showBiGraph(g, 1);
	// Perform the hungarian algorithm on the bipartite graph we made
	URLArray optimalMatching = findMinCost(g);
	// Print out the matching
	printf("%.6f\n", getRankVal(optimalMatching, 0));
	for (int i = 0; i < length(optimalMatching); i++) {
		printf("%s\n", getURLName(optimalMatching, i));
	}

	// Kill everything
	deleteBG(g);
	for (int i = numFiles - 1; i >= 0; i--) {
		dropURLArray(rankMatrix[i]);
	}
	dropURLArray(optimalMatching);
	free(rankMatrix);
	return 0;
}

// Scans the file and creates a list that represents the ranking
// of the URLs in the file, it returns the search tree so we don't
// lose it
FloatBST createRankList(char *file, FloatBST set, URLArray toPopulate) {
	// Open file and see if it worked
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	// The 'count' in the url array will now be the ranking
	int i = 1;
	// Scan each url into the array and into the tree
	// the tree is designed to be like a set 
	// Increase the 'position' of each subsequent url
	char read[MAX_SIZE];
	while (fscanf(fp, "%s", read) != EOF) {
		toPopulate = URLInsert(toPopulate, read, i, 0);
		set = floatBSTInsert(set, read, 0);
		i++;
	}
	fclose(fp);
	return set;
}

URLArray readFiles(int numFiles, char **files, URLArray *rankMatrix) {
	// Initialise tree to store the set of URLs
	FloatBST urlTree = newFloatBST();

	// Populate the rank matrix and URL set
	for (int i = 0; i < numFiles; i++) {
		rankMatrix[i] = newURLArray(1);
		urlTree = createRankList(files[i], urlTree, rankMatrix[i]);
	}

	// convert tree to an array
	URLArray urlSet = convertToArray(urlTree);
	// If the tree had no URLs it would be null and in this case
	// this means no need to rank the URLs cos there are none
	if (urlTree == NULL) {
		exit(0);
	}
	dropFloatBST(urlTree);
	return urlSet;
}

void calculateFRD(URLArray *rankMatrix, BiGraph g, int numFiles) {
	int n = numPositions(g);

	// Loop through the rank matrix
	for (int i = 0; i < numFiles; i++) {
		int l = length(rankMatrix[i]);
		// For each element in the ranking list
		for (int j = 0; j < l; j++) {
			// Calculate that elements frd at each position
			for (int position = 1; position <= n; position++) {
				float frd = 
					fabs((float)getCount(rankMatrix[i], j)/(float)l
					- (float)position/(float)n);
				// Update its frd in the graph
				addWeight(g, getURLName(rankMatrix[i], j), 
					position -1, frd);
			}
		}
	}
}


URLArray findMinCost(BiGraph g) {
	// Create a copy of the matrix, sunce the actual bipartite
	// graph is going to have some matrix operations performed on it
	int numPos = numPositions(g);
	float matrixCopy[numPos][numPos];

	for (int i = 0; i < numPos; i++) {
		for (int j = 0; j < numPos; j++) {
			matrixCopy[i][j] = getWeight(g, i, j);
		}
	}

	// What we want to do first is to go through each row (Vertex)
	// and find the edge that has minimum cost. Then subtract that
	// from every other edge connected to that vertex so that we get
	// a relative minimum cost
	hungarianSetup(g);

	// Now that we have normalised the matrix, we seek to join up
	// each vertex to position (on a 0 weight edge). If we can join
	// every vertex to a unique position, we are done, minimum cost
	// assignment has been done. The way we try to join each vertex to a
	// position is through a recursive DFS. Essentially for each vertex
	// we try to match it to a 0 cost position. Then if there is a 
	// clash, we recursively call the match function to see if the 
	// clashing vertex can match to something else whilst simultaneously
	// keeping track of the Positions.

	// If we have found a perfect matching  (i.e. number of matches = 
	// number of vertices) then we are done.
	// Otherwise (see further explanation) inside the while loop

	// This array is an array of vertices that denote which vertex
	// each position index has been matched to
	Vert posToVertMatch[numPos];
	Pos vertToPosMatch[numPos];
	int numMatch = findMaximalMatching(g, posToVertMatch, vertToPosMatch); 
	while (numMatch < numPos) {
		//showBiGraph(g, 1);
		// Otherwise, because we were unable to find a perfect match
		// with  the minimum costs, we now have to include the second 
		// least expensive cost. But the second least expensive cost
		// doesn't just mean the   next minimum cost as that may not 
		// guaruantee the correct minimum solution. Instead (according 
		// to wikipedia and online articles) we now return to the matrix
		// representation instead of graph.
		// We seek to cover all zeros in our matrix with minimum number 
		// of rows and columns. And from the uncovered elements that is 
		// our next 'least' cost value

		// Mark Matrix will cover the zeros with min rows/ columns
		// by filling in the below arrays to tell us which ones to cover
		int markedCols[numPos];
		int unmarkedRows[numPos];
		for (int i = 0; i < numPos; i++) {
			markedCols[i] = -1;
			unmarkedRows[i] = -1;
		}
		markMatrix(g, vertToPosMatch, posToVertMatch, markedCols,
			unmarkedRows);
		// Now using our marked matrix we can find hte next best
		// edge to include in our minimum cost edges
		float nextBestMin = findNextMin(g, markedCols, unmarkedRows);

		// Now we subtract the next best minimum from all unmarked 
		// values and add it to all values who are at the intersection 
		// of our marked cols and unmarked rows. This way we redefine
		// our weight matrix so we can solve it again. The reasoning
		// behind this weird addition and subtraction normalises the 
		//next newest minimum with the existing
		// ones so that it will lead us to an optimal solution
		recalculateMatrix(g, markedCols, unmarkedRows, nextBestMin);

		// Try to find a new match with the recalculated graph
		numMatch = findMaximalMatching(g, posToVertMatch, 
			vertToPosMatch);
	}

	// At this stage here we now have the positions and the vertex that 
	// was matched to that position. We just need to compute the total 
	// cost we do this by going back to our matrix copy
	float totalCost = 0;
	for (Pos p = 0; p < numPos; p++) {
		totalCost = totalCost + matrixCopy[posToVertMatch[p]][p];
	}

	// Now just return a URL Array with
		// Name
		// Count as position
		// floatVal as the total cost
	URLArray rankings = newURLArray(numPos);
	for (int i = 0; i < numPos; i++) {
		Vert vertexID = posToVertMatch[i];
		Pos rank = i + 1;
		char *name = convIDToURL(g, vertexID);
		rankings = URLInsert(rankings, name, rank, totalCost);
		free(name);
	}

	return rankings;
}

void hungarianSetup(BiGraph g) {
	int numPos = numPositions(g);
	// Normalise the rows
	for (Vert i = 0; i < numPos; i++) {
		// Finds value of minimum cost edge
		float minCost = getWeight(g, i, findMinPos(g, i));
		// Subtracts the value of minimum cost edge
		for (Pos p = 0; p <numPos; p++) {
			subtractWeight(g, i, p, minCost);
		}
	}

	// Normalise the columns if not already done
	for (Pos i = 0; i < numPos; i++) {
		Vert minVert = findMinVert(g, i);
		// zero already exists
		if (minVert == -1) continue;
		// otherwise normalise the column
		float minCost = getWeight(g, minVert, i);
		for (Vert v = 0; v < numPos; v++) {
			subtractWeight(g, v, i, minCost);
		}
	}

}

void markMatrix(BiGraph g, int *vertToPosMatch, int *posToVertMatch,
	int *markedCols, int *unmarkedRows) {

	int numPos = numPositions(g);
	int markedRows[numPos];
	for (int i = 0; i < numPos; i++) markedRows[i] = -1;

	// So first we find unassigned vertices, we mark it by setting a 1
	for (Vert v = 0; v < numPos; v++) {
		// If vertex was matched then conintue
		if (vertToPosMatch[v] != -1) continue;
		// Mark the vertex (row)
		markedRows[v] = 1;
		// Mark each position (column) that has a zero in the given row
		// i.e. mark the positions that are connected to the given 
		// vertex with a 0 cost edge and then mark the columns that have
		// zeros in that given row (markColumns is recursively done)
		markColumns(g, v, posToVertMatch, markedCols, markedRows);
	}

	// Invert our marked rows to get unmarked rows
	for (int i = 0; i < numPos; i++) {
		if (markedRows[i] == -1) unmarkedRows[i] = 1;
	}
}

void markColumns(BiGraph g, Vert row, int *posToVertMatch,
	int *markedCols, int *markedRows) {

	int numPos = numPositions(g);
	// If column has a 0 then mark it
	for (Pos p = 0; p < numPos; p++) {
		if (getWeight(g, row, p) < FLT_EPSILON) {
			// Already marked
			if (markedCols[p] == 1) continue;
			// Mark it
			markedCols[p] = 1;

			// Mark the row which has assignment in this column
			int vMatch = posToVertMatch[p];
			if (vMatch == -1) continue;
			// Already marked
			if (markedRows[vMatch] == 1) continue;
			markedRows[vMatch] = 1;
			// Mark the columns which have assignment in that row
			markColumns(g, vMatch, posToVertMatch, markedCols, 
				markedRows);
		}
	}
}

float  findNextMin(BiGraph g, int *markedCols, int *unmarkedRows) {
	int minDefined = 0;
	float min;
	int numPos = numPositions(g);
	for (Vert v = 0; v < numPos; v++) {
		// If we're in a unmarked row continue
		if (unmarkedRows[v] != -1) continue;
		for (Pos p = 0; p < numPos; p++) {
			// If we're in a marked column continue
			if (markedCols[p] != -1) continue;
			float matrixVal = getWeight(g, v, p);
			// If min hasn't been defined, define it
			if (minDefined == 0) {
				min = matrixVal;
				minDefined = 1;
			// Reset the minimum if the value at 
			// given point is lower than the min
			} else if (matrixVal < min) {
				min = matrixVal;
			}
		}
	}

	return min;
}

void recalculateMatrix(BiGraph g, int *markedCols, int *unmarkedRows, 
	float nextBestMin) {
	// Subtract from all elements that are not in markedCOls/
	// unmarkedRows
	// Add to all elements that are simultanesouly in a markedCols AND
	// unmarked rows
	// Do nothing for anything else

	int numPos = numPositions(g);

	for (Vert v = 0; v < numPos; v++) {
		for (Pos p = 0; p < numPos; p++) {
			// Intersection -> add
			if (unmarkedRows[v] == 1 && markedCols[p] == 1) {
				increaseWeight(g, v, p, nextBestMin);
			// Exclusion -> subtract
			} else if (unmarkedRows[v] == -1 && markedCols[p] == -1) {
				subtractWeight(g, v, p, nextBestMin);
			}
		}
	}
}




// My original approach at the algorithm: 
// 4. Now is the actual algorithm step, we seek to connect this tree up (create a subgraph)
// 		in such a way that the edges have a MINIMUM sum. And each vertex
// 		can only be connected to each position exclusively (i.e.) no two
// 		vertices share the same position. I.e. each URL has a 1 to 1 
// 		How am I going to attempt to do this?
// 		idk lol
// 		THIS IS A RECURSIVE ALGORITHM
// 		For each vertex we are going to find the minimum cost edge and
// 		match it directly if it's available.
// 		Now lets say we are at Vertex A and the minimum cost edge leads to a Position
// 		that has already been matched to Vertex B. Then what we do is we compare the relative
// 		cost of the next lowest edge for both A and B. Whoever has the lowest next relative cost
// 		is the vertex that is going to concede to this matching and decide to match to the
// 		next lowest relative cost edge instead And then we repeat this algorithm again (recursive call)
// 		to ensure that we don't revisit edges we have already ruled out, each iteration of this algo
// 		we keep track of which edges we have already seen and ignore those in finding the next lowest 
// 		relative cost.

// 		If the next lowest relative cost is the same, then we look at the next next one
