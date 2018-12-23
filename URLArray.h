// Array list of URLs with the number of times
// they occurred and their float rank values

#ifndef URLARRAY_H
#define URLARRAY_H

typedef struct URLArrayWrapper *URLArray;

URLArray newURLArray(int);
// free memory
void dropURLArray(URLArray);
// Get length
int length(URLArray);
// insert a new value into the URLArray
URLArray URLInsert(URLArray, char *, int, float);
// Sort by count
URLArray sortCount(URLArray);
// Sort by pagerank
URLArray sortRankVal(URLArray);
// Get url name for an item in the array
char *getURLName(URLArray, int);
// Get the count for an item in the array
int getCount(URLArray, int);
// Get the rank value for an item in the array
float getRankVal(URLArray, int);


#endif