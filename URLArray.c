// Implementation of an array list of URLs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "URLArray.h"
#include "searchAndSort.h"
#include <assert.h>
#include "strdup.h"

typedef struct URLItem {
	char *url;
	int count;
	float rankVal;
} URLItem;

typedef struct URLArrayWrapper {
	URLItem *array;
	int maxItems;
	int numItems;
} URLArrayWrapper;

// Helpers to do merge sort
void countMerge(void *input, int lo, int mid, int hi);
void rankMerge(void *input, int lo, int mid, int hi);
int countCmp(void *item1, void *item2);
int floatCmp(void *item1, void *item2);

URLArray newURLArray(int maxItems) {
    URLArray new = malloc(sizeof(URLArrayWrapper));
    assert(new != NULL);
    new->maxItems = maxItems;
    new->numItems = 0;

    if (maxItems > 0) {
        URLItem *array = malloc(maxItems*sizeof(URLItem));
        assert(array != NULL);
        new->array = array;
    } else  {
        new->array = NULL;
    }
    return new;
}
// free memory
void dropURLArray(URLArray u) {
    if (u == NULL) return;
    for (int i = 0; i < u->numItems; i++) {
        free(u->array[i].url);
    }
    free(u->array);
    free(u);
}
// Get length
int length(URLArray u) {
    if (u == NULL) {
        return -1;
    }
    return u->numItems;
}
// insert a new value into the URLArray
URLArray URLInsert(URLArray u, char *url, int count, float rankVal) {
    // If the url array had no items (shouldn't happen)
    if (u->array == NULL) {
        URLItem *array = malloc(sizeof(URLItem));
        u->maxItems = 1;
        assert(array != NULL);
        u->array = array;
    }
    // Like an array list -> we can exapnd
    if (u->numItems == u->maxItems) {
        u->array = realloc(u->array, (u->maxItems*2)*sizeof(URLItem));
        assert(u->array != NULL);
        u->maxItems = u->maxItems*2;
    }

    // Insert the value
    char *urlDup = strdup(url);
    int index = u->numItems;
    u->array[index].url = urlDup;
    u->array[index].count = count;
    u->array[index].rankVal = rankVal;

    // Increment number of items
    u->numItems++;
    return u;
}

// Sort by count
URLArray sortCount(URLArray u) {
    if (u == NULL) return NULL;
    mergesort(u->array, 0, u->numItems-1, countCmp, countMerge);
    return u;
}
// Sort by pagerank
URLArray sortRankVal(URLArray u) {
    if (u == NULL) return NULL;
    mergesort(u->array, 0, u->numItems-1, floatCmp, rankMerge);
    return u;
}

// Function to plug into merge sort but compare by count
void countMerge(void *input, int lo, int mid, int hi) {
    URLItem *urlArray = (URLItem *) input;
    int i, j, k;
    i = lo;
    j = mid + 1;
    k = 0;
    int nitems = hi - lo + 1;
    URLItem *tmp = malloc(nitems*sizeof(URLItem));

    // Zip into temp array
    while (i <= mid && j <= hi) {
        if (countCmp(&urlArray[i], &urlArray[j]) <= 0) {
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

// Function to plug into merge sort but compare by rank
void rankMerge(void *input, int lo, int mid, int hi) {
    URLItem *urlArray = (URLItem *) input;
    int i, j, k;
    i = lo;
    j = mid + 1;
    k = 0;
    int nitems = hi - lo + 1;
    URLItem *tmp = malloc(nitems*sizeof(URLItem));

    // Zip into temp array
    while (i <= mid && j <= hi) {
        if (floatCmp(&urlArray[i], &urlArray[j]) < 0) {
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

// Comparison functions for mergesort
int countCmp(void *item1, void *item2) {
    URLItem *itemA = (URLItem *) item1;
    URLItem *itemB = (URLItem *) item2;
    if (itemA->count < itemB->count) {
        return 1;
    } else if (itemA->count == itemB->count) {
        return 0;
    } else if (itemA->count > itemB->count) {
        return -1;
    }

    return 0;
}

int floatCmp(void *item1, void *item2) {
    URLItem *itemA = (URLItem *) item1;
    URLItem *itemB = (URLItem *) item2;
    if (itemA->rankVal < itemB->rankVal) {
        return 1;
    } else if (itemA->rankVal == itemB->rankVal) {
        return strcmp(itemA->url, itemB->url);
    } else if (itemA->rankVal > itemB->rankVal) {
        return -1;
    }

    return 0;
}

// Get url name for an item in the array
char *getURLName(URLArray u, int index) {
    if (index >= u->numItems) {
        return NULL;
    } else {
        return u->array[index].url;
    }
}

// Get the count for an item in the array
int getCount(URLArray u, int index) {
    if (index >= u->numItems) {
        return -1;
    } else {
        return u->array[index].count;
    }
}

// Get the rank value for an item in the array
float getRankVal(URLArray u, int index) {
    if (index >= u->numItems) {
        return -1;
    } else {
        return u->array[index].rankVal;
    }
}
