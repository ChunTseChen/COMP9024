// DLListStr.h - Interface to doubly-linked list ADT
// Written by John Shepherd, March 2013
// Last modified, Ashesh Mahidadia, Jan 2020 

#ifndef DLListStr_H
#define DLListStr_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct DLListRep *DLListStr;

typedef struct DLListNode {
	char   value[100];  // value of this list item (string)
	int degree;
	double pagerank;
	int count;
	int index;
	struct DLListNode *prev;
	               // pointer previous node in list
	struct DLListNode *next;
	               // pointer to next node in list
	DLListStr sub; //sub list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep; 

// External view of DLListStr
// Implementation given in DLListStr.c
// Implements a DLListStr of string 




// create a new empty DLListStr
DLListStr newDLListStr();


//insert the data to DLListStr by order(val)
void insertSetOrd(DLListStr L, char *val);

void insertSetOrdPageRankAndoutDegree(DLListStr L, char * val, int degree, double pagerank);

void insertSubList(DLListStr L, char *val, char *url);

// free up all space associated with list
void freeDLListStr(DLListStr);

// display items from a DLListStr, comma separated
void showDLListStr(DLListStr L);

bool checkVal(DLListStr L, char *val);

void countByVal(DLListStr L, char *val, int count);

void insertSetOrdCountAndPageRank(DLListStr L, char *val, int count, double pagerank);
#endif
