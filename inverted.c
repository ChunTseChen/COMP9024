// #include <ctype.h>
// #include "DataOperate.h"

// int main(void)
// {

//     //1. Read file "collection.txt" and add into the list

//     //create new list
//     DLListStr dataUrls = newDLListStr();
//     DLListStr wordList = newDLListStr();
//     //read and save the data to the list
//     getCollection(dataUrls);

//     //2.Read the datas from section-2 and input to the list
//     readSection2(dataUrls, wordList);

//     //5.Output to the file by order
//     outputInvertedFile(wordList);

//     //6.Free the space
//     freeDLListStr(dataUrls);
//     freeDLListStr(wordList);

//     return 0;
// }