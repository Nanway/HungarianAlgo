# Make file for assignment

CC=gcc
CFLAGS=-Wall -Werror -std=c11 -g

all: scaledFootrule

scaledFootrule : scaledFootrule.o  strdup.o searchAndSort.o bipartite.o URLArray.o FloatBST.o
	$(CC) $(CFLAGS) -o scaledFootrule scaledFootrule.o  strdup.o bipartite.o searchAndSort.o URLArray.o FloatBST.o -lm
bipartite.o : bipartite.c bipartite.h

scaledFootrule.o : scaledFootrule.c scaledFootrule.h

FloatBST.o : FloatBST.c FloatBST.h

URLArray.o : URLArray.c URLArray.h

searchAndSort.o : searchAndSort.c searchAndSort.h

strdup.o : strdup.c strdup.h

clean:
	rm -f *.o



