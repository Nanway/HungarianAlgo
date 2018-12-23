// Bipartite Graph
#ifndef BIPARTITE_H
#define BIPARTITE_H
#define END_OF_REF -1
#define NOT_INIT -1.0
typedef struct bipartiteG *BiGraph;
typedef int Vert;
typedef int Pos;

// Function signatures
// new bipartite graph
BiGraph newBG(int);
// delete it
void  deleteBG(BiGraph);
// add a weighted edge
int   addWeight(BiGraph,char *, Pos, float);
// show
void  showBiGraph(BiGraph,int);
// get number of positions/ vertices
int numPositions(BiGraph);
// map a url name to a vertex ID
Vert convURLtoID(BiGraph g, char *url);
// reverse of the above
char *convIDToURL(BiGraph g, Vert x);
// gets the cost of the edge from Vertex ID to Position
float getWeight(BiGraph, Vert, Pos);
// sets the cost of an edge from vertex id to psotion
void setWeight(BiGraph g, Vert v, Pos p, float val);
// same as set but subtracts from existing value
void subtractWeight(BiGraph g, Vert v, Pos p, float val);
// same as above but adds
void increaseWeight(BiGraph g, Vert v, Pos p, float val);
// Find the minimum position index for a given vertex
Pos findMinPos(BiGraph g, Vert v);
// Finds the minimum vertex index for a given position
Vert findMinVert(BiGraph g, Pos v);
// find the maximum amount of vertices that can be matched
// to a unique position on a 0 cost edge
int findMaximalMatching(BiGraph g, int *posToVertMatch, 
	int *vertToPosMatch);
// Essentially a DFS that sees if we can match a vertex to a position,
// if the position already has a vertex matched, we recursively call
// and try to re-match it again
int canMatch(BiGraph g, Vert v, int *seen, int *matches, int *v2pMatch);



#endif
