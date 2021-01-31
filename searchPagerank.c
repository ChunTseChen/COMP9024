// #include <stdio.h>
// #include <stdlib.h>
// #include "DLListStr.h"
// #include "DataOperate.h"

// int main(int argc, char *argv[])
// {

//     // 1.read argv to collectedList;
//     // 2.read invertedInde.txt to inverted list;
//     // 3.read pagerankList.txt to pagerank list;

//     //1.get the input to the collectedList
//     DLListStr collectedList = newDLListStr();
//     for (int i = 1; i < argc; i++)
//     {
//         insertSetOrd(collectedList, argv[i]);
//     }

//     //2.read the invertedIndex.txt and input to the List
//     //moreover, count the urls
//     DLListStr invertedList = newDLListStr();
//     readInvertedIndex(invertedList, collectedList);

//     //3.read the pagerankList.txt and input to a List
//     DLListStr pagerankList = newDLListStr();
//     getPageRankToList(pagerankList);

//     //4.get the final result List
//     DLListStr resultList = newDLListStr();
//     getResultList(resultList, invertedList, pagerankList);

//     //5.output the result
//     outputWithLimit(resultList, 30);

//     //6.release the space
//     freeDLListStr(collectedList);
//     freeDLListStr(invertedList);
//     freeDLListStr(pagerankList);
//     freeDLListStr(resultList);

//     return 0;
// }