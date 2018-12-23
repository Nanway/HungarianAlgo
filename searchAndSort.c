// File to store search/ sort algos


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "searchAndSort.h"
#include "strdup.h"

// Generic recursive mergesort function
void mergesort(void *input, int lo, int hi,
    int (*compare) (void *, void *),
    void (*merger)(void *, int, int, int)) {

    int mid = (hi + lo)/2;
    if (hi <= lo) return;
    mergesort(input, lo, mid, compare, merger);
    mergesort(input, mid + 1, hi, compare, merger);
    merge(input, lo, mid, hi, merger);
}

// Wrapper around specific merging function
void merge(void *input, int lo, int mid, int hi,
    void (*merger)(void *, int, int, int)) {

    merger(input, lo, mid, hi);
}

// As URLs are used repeatedly, this merges an array 
// of strings together 
void urlMerge(void *input, int lo, int mid, int hi) {
    char **urlArray = (char **) input;
    int i, j, k;
    i = lo;
    j = mid + 1;
    k = 0;
    int nitems = hi - lo + 1;
    char **tmp = malloc(nitems*sizeof(char *));

    // Zip into temp array
    while (i <= mid && j <= hi) {
        if (strcmp(urlArray[i], urlArray[j]) < 0) {
            tmp[k++] = urlArray[i++];
        } else {
            tmp[k++] = urlArray[j++];
        }
    }

    // Fill in remaining spots
    while (i <= mid) tmp[k++] = urlArray[i++];
    while (j <= hi) tmp[k++] = urlArray[j++];

    // Copy back
    for(i = lo, k = 0; i <= hi; i++, k++) {
        urlArray[i] = tmp[k];
    }
    free(tmp);
}

// Wrapper around strcmp
int stringcmp(void *item1, void *item2) {
    return strcmp((char *)item1, (char *)item2);
}

// Performs a binary search for data structures who involve
// an array of strings
int binarySearchURL(char **searchArray, char *search, int lo, int hi) {
    int mid = (hi + lo)/2;
    if (searchArray[mid] == NULL) {
        return -1;
    }

    int comparison = strcmp(search, searchArray[mid]);

    if (comparison == 0) {
        return mid;
    } else if (hi <= lo) {
        return -1;
    } else if (comparison < 0) {
        return binarySearchURL(searchArray, search, lo, mid - 1);
    } else if (comparison > 0) {
        return binarySearchURL(searchArray, search, mid + 1, hi);
    } else {
        abort();
    }
}