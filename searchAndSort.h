// Search and sort algos file 
#ifndef SEARCHANDSORT_H
#define SEARCHANDSORT_H

// Generics
void mergesort(void *input, int lo, int hi,
    int (*compare) (void *, void *), 
    void (*merger)(void *, int, int, int));

void merge(void *input, int lo, int mid, int hi, 
    void (*merger)(void *, int, int, int));

// Didn't know how to make the search generic so this searches for text
// in an array of cahrs
int binarySearchURL(char **, char *search, int lo, int hi);

// Type Specific
void urlMerge(void *input, int lo, int mid, int hi);
int stringcmp(void *item1, void *item2);

#endif
