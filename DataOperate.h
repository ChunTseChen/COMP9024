
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DLListStr.h"
#include "InvertedIdx.h"
#include "Graph.h"
#define MAXSTRING 1000

char *trim(char *str);
void getCollection(DLListStr dataUrls);
int getIndexFromList(DLListStr dataUrls, char *val);
void readSection1(DLListStr dataUrls, Graph g);
void readSection2(DLListStr dataUrls, DLListStr wordList);
void calculatePageRank(DLListStr dataUrls, Graph g,double d, double diffPR, int maxIterations, int *outDegree, double *pr);
void outputFile(DLListStr dataUrls, int *outDegree, double *pr);
void readInvertedIndex(DLListStr invertedList, DLListStr collectedList);
void outputInvertedFile(DLListStr wordList);
void getPageRankToList(DLListStr pagerankList);
void getResultList(DLListStr resultList, DLListStr invertedList, DLListStr pagerankList);
void outputWithLimit(DLListStr resultList, int limit);