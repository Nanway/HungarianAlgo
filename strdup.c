
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *strdup(char *s) {
	// Create copy same size + 1 for null
	if (s == NULL) return NULL;
	char *copy = malloc(strlen(s) + 1);
	if (copy == NULL) return NULL;
	strcpy(copy, s);
	return copy;
}