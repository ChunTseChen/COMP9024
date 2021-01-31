#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DLListStr.h"
#include "DataOperate.h"

char *collection_file_path = "../aEx1/collection.txt";
char *file_dir = "../aEx1/";

int main(int argc, char * argv[]) {
  // 1. Accept argument and store them in local variables
  if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
    printf("Please enter all the three arguments:\n");
    printf("pagerank <d> <diffPR> <maxIterations>\n");
    printf("d - damping factor, diffPR - difference in PageRank sum, "
           "maxIterations - maximum iterations\n");
    return 1;
  }

  double d = atof(argv[1]);
  double diffPR = atof(argv[2]);
  int maxIteration = atoi(argv[3]);

  //2. Read file "collection.txt" and add into the list
  
  //create a list
  DLListStr dataUrls =newDLListStr(); 
  //read and save the data to the list
  getCollection(dataUrls);

  //3. Read the data in the list:dataUrls and create the graph 

  //create a new graph
  Graph g =newGraph(dataUrls->nitems);
  //read the datas from section-1 and input to the graph
  readSection1(dataUrls, g);
  showGraph(g);



  //4.Calulate the pagerank

  int *outDegree =malloc(sizeof(int)* dataUrls->nitems);
  double *pr =malloc(sizeof(double)*dataUrls->nitems);
  calculatePageRank(dataUrls, g, d, diffPR, maxIteration, outDegree, pr);

  //5.Output to the file by order
  outputFile(dataUrls, outDegree, pr);


  //6.Free the space
  freeDLListStr(dataUrls);
  freeGraph(g);
  free(outDegree);
  free(pr);



  return 0;
}
