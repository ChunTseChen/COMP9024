

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAXSTRING 1024

// data structures representing DLListStr

typedef struct DLListRep *DLListStr;

typedef struct DLListNode
{
    char value[100]; // value of this list item (string)
    int degree;
    double pagerank;
    int count;
    int index;
    struct DLListNode *prev;
    // pointer previous node in list
    struct DLListNode *next;
    // pointer to next node in list
    DLListStr sub; //sub list
    DLListStr urls;
} DLListNode;

typedef struct DLListRep
{
    int nitems;        // count of items in list
    DLListNode *first; // first node in list
    DLListNode *curr;  // current node in list
    DLListNode *last;  // last node in list
} DLListRep;

// create a new DLListNode (private function)
static DLListNode *newDLListNode(char *val, int outdegree, double pagerank)
{
    DLListNode *new;
    new = malloc(sizeof(DLListNode));
    assert(new != NULL);

    strcpy(new->value, val); // for int, new->value = it;

    new->prev = new->next = NULL;
    new->degree = outdegree;
    new->pagerank = pagerank;
    new->sub = NULL;
    new->count = 0;
    return new;
}

char *trim(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    // remove the following punctuation marks, if they appear at the end of a
    // word: '.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
    while (end > str && (*end == '.' || *end == ',' || *end == ';' ||
                         *end == ':' || *end == '?' || *end == '!'))
        end--;
    end[1] = '\0';

    return str;
}

// create a new empty DLListStr
DLListStr newDLListStr()
{
    struct DLListRep *L;

    L = malloc(sizeof(struct DLListRep));
    assert(L != NULL);
    L->nitems = 0;
    L->first = NULL;
    L->last = NULL;
    L->curr = NULL;
    return L;
}

bool checkVal(DLListStr L, char *val)
{
    DLListNode *temp = L->first;
    while (temp != NULL)
    {
        if (strcmp(temp->value, val) == 0)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

int compareToByVal(DLListNode *first, DLListNode *second)
{
    assert(first != NULL && second != NULL);
    //if first < second : return -1
    //if first == second : return 0
    //if first > second : return 1
    int result = 0;
    result = strcmp(first->value, second->value);
    // if (first->value < second->value) {
    //     result = -1;
    // } else if (first->value > second->value) {
    //     result = 1;
    //}
    return result;
}

int compareByPagerank(char *val, int outdegree, double pagerank, DLListNode *present)
{
    int result = 0;
    if (pagerank < present->pagerank)
    {
        result = -1;
    }
    else if (pagerank > present->pagerank)
    {
        result = 1;
    }
    else
    {
        if (outdegree < present->degree)
        {
            result = -1;
        }
        else if (outdegree > present->degree)
        {
            result = 1;
        }
        else
        {
            result = strcmp(val, present->value);
        }
    }
    return result;
}

/* 
   pre-reqisite: L is ordered (increasing) with no duplicates
   post-condition: val is inserted in L, L is ordered (increasing) with no duplicates
*/
void insertSetOrd(DLListStr L, char *val)
{

    /* 
	   implement this function to
	   insert val in L  (in order), no duplicates (set)

	*/
    DLListNode *newNode = newDLListNode(val, 0, 0.0);
    L->nitems++;
    int c = 1;
    newNode->index = c;
    c++;
    if (L->first == NULL)
    {
        L->first = newNode;
        L->last = L->first;
    }
    else
    {
        if (compareToByVal(newNode, L->first) < 0)
        {
            newNode->next = L->first;
            L->first->prev = newNode;
            L->first = newNode;
        }
        else if (compareToByVal(newNode, L->last) > 0)
        {
            L->last->next = newNode;
            newNode->prev = L->last;
            L->last = newNode;
        }
        else if (checkVal(L, val) == false)
        {
            DLListNode *pre = L->first;
            DLListNode *curr = L->first->next;
            while (curr != NULL)
            {
                if (compareToByVal(newNode, pre) > 0 && compareToByVal(newNode, curr) < 0)
                {
                    pre->next = newNode;
                    newNode->next = curr;
                    newNode->prev = pre;
                    curr->prev = newNode;
                    break;
                }
                pre = pre->next, curr = curr->next;
            }
        }
    }
}

//insert by order from pagerank, degree, val
void insertSetOrdPageRankAndoutDegree(DLListStr L, char *val, int outdegree, double pagerank)
{
    DLListNode *newNode = newDLListNode(val, outdegree, pagerank);
    L->nitems++;
    if (L->first == NULL)
    {
        L->first = newNode;
        L->last = L->first;
    }
    else
    {
        if (compareByPagerank(val, outdegree, pagerank, L->first) > 0)
        {
            newNode->next = L->first;
            L->first->prev = newNode;
            L->first = newNode;
        }
        else if (compareByPagerank(val, outdegree, pagerank, L->last) < 0)
        {
            L->last->next = newNode;
            newNode->prev = L->last;
            L->last = newNode;
        }
        else if (checkVal(L, val) == false)
        {
            DLListNode *pre = L->first;
            DLListNode *curr = L->first->next;
            while (curr != NULL)
            {
                if (compareByPagerank(val, outdegree, pagerank, pre) < 0 && compareByPagerank(val, outdegree, pagerank, curr) > 0)
                {
                    pre->next = newNode;
                    newNode->next = curr;
                    newNode->prev = pre;
                    curr->prev = newNode;
                    break;
                }
                pre = pre->next, curr = curr->next;
            }
        }
    }
}

//find the val which can pare to the page link and insert to the sub-list
void insertSubList(DLListStr L, char *val, char *url)
{
    DLListNode *present = L->first;
    while (present != NULL)
    {
        if (strcmp(present->value, val) == 0)
        {
            if (present->sub == NULL)
            {
                present->sub = newDLListStr();
            }
            insertSetOrd(present->sub, url);
        }
        present = present->next;
    }
}

int compareByCountAndPagerank(char *val, int count, double pagerank, DLListNode *present)
{
    int result = 0;
    if (count < present->count)
    {
        result = -1;
    }
    else if (count > present->count)
    {
        result = 1;
    }
    else
    {
        if (pagerank < present->pagerank)
        {
            result = -1;
        }
        else if (pagerank > present->pagerank)
        {
            result = 1;
        }
        else
        {
            result = strcmp(val, present->value);
        }
    }
    return result;
}

//insert by order from count, pagerank, val
void insertSetOrdCountAndPageRank(DLListStr L, char *val, int count, double pagerank)
{
    DLListNode *newNode = newDLListNode(val, count, pagerank);
    L->nitems++;
    if (L->first == NULL)
    {
        L->first = newNode;
        L->last = L->first;
    }
    else
    {
        if (compareByCountAndPagerank(val, count, pagerank, L->first) > 0)
        {
            newNode->next = L->first;
            L->first->prev = newNode;
            L->first = newNode;
        }
        else if (compareByCountAndPagerank(val, count, pagerank, L->last) < 0)
        {
            L->last->next = newNode;
            newNode->prev = L->last;
            L->last = newNode;
        }
        else if (checkVal(L, val) == false)
        {
            DLListNode *pre = L->first;
            DLListNode *curr = L->first->next;
            while (curr != NULL)
            {
                if (compareByCountAndPagerank(val, count, pagerank, pre) < 0 && compareByCountAndPagerank(val, count, pagerank, curr) > 0)
                {
                    pre->next = newNode;
                    newNode->next = curr;
                    newNode->prev = pre;
                    curr->prev = newNode;
                    break;
                }
                pre = pre->next, curr = curr->next;
            }
        }
    }
}

void countByVal(DLListStr L, char *val, int count)
{
    DLListNode *container = L->first;
    while (container != NULL)
    {
        if (strcmp(container->value, val) == 0)
        {
            container->count += count;
        }
        container = container->next;
    }
}

// display items from a DLListStr, comma separated
void showDLListStr(DLListStr L)
{
    assert(L != NULL);
    DLListNode *curr;
    int count = 0;
    for (curr = L->first; curr != NULL; curr = curr->next)
    {
        count++;
        if (count > 1)
        {
            fprintf(stdout, ", ");
        }
        fprintf(stdout, "%s", curr->value);
    }
    fprintf(stdout, "\n");
}

// free up all space associated with list
void freeDLListStr(DLListStr L)
{
    assert(L != NULL);
    DLListNode *curr, *prev;
    curr = L->first;
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(L);
}

void readInvertedIndex(DLListStr invertedList, DLListStr collectedList)
{
    char delim[3] = " \n";
    char *token;
    char line[MAXSTRING];
    FILE *fp;

    if ((fp = fopen("invertedIndex.txt", "r")) == NULL)
    {
        printf("Error!\n");
    }
    while (fgets(line, MAXSTRING, fp) != NULL)
    {
        /*first token*/
        token = strtok(line, delim);

        /* iterate over the rest of the tokens */
        //check the first token(word) is in the collectedList or not
        if (checkVal(collectedList, token) == true)
        {
            //get second token(first url)
            token = strtok(NULL, delim);
            while (token != NULL)
            {
                token = trim(token);
                if (token[0] != '\0')
                {
                    insertSetOrd(invertedList, token);
                    countByVal(invertedList, token, 1);
                }
                token = strtok(NULL, delim);
            }
        }
    }
    fclose(fp);
}

void getPageRankToList(DLListStr pagerankList)
{
    char delim[3] = ", ";
    char *token;
    char line[MAXSTRING];
    FILE *fp;

    if ((fp = fopen("pagerankList.txt", "r")) == NULL)
    {
        printf("Error!\n");
    }
    while (fgets(line, MAXSTRING, fp) != NULL)
    {
        /*first token*/
        token = strtok(line, delim);
        char *url = token;
        int out_degree = 0;
        double page_rank = 0.0;

        int count = 0;
        /* iterate over the rest of the tokens */
        while (token != NULL)
        {
            token = trim(token);
            if (token[0] != '\0')
            {
                count++;
                if (count == 2)
                    out_degree = atoi(token);
                else if (count == 3)
                    page_rank = atof(token);
            }
            token = strtok(NULL, delim);
        }
        insertSetOrdPageRankAndoutDegree(pagerankList, url, out_degree, page_rank);
    }
    fclose(fp);
}

void getResultList(DLListStr resultList, DLListStr invertedList, DLListStr pagerankList)
{
    DLListNode *invNode = invertedList->first;
    DLListNode *pagNode = pagerankList->first;
    while (invNode != NULL)
    {
        pagNode = pagerankList->first;
        while (pagNode != NULL)
        {

            if (strcmp(invNode->value, pagNode->value) == 0)
            {
                insertSetOrdCountAndPageRank(resultList, invNode->value, invNode->count, pagNode->pagerank);
            }
            pagNode = pagNode->next;
        }
        invNode = invNode->next;
    }
}

int main(int argc, char *argv[])
{

    // 1.read argv to collectedList;
    // 2.read invertedInde.txt to inverted list;
    // 3.read pagerankList.txt to pagerank list;

    //1.get the input to the collectedList
    DLListStr collectedList = newDLListStr();
    for (int i = 1; i < argc; i++)
    {
        insertSetOrd(collectedList, argv[i]);
    }

    showDLListStr(collectedList);

    //2.read the invertedIndex.txt and input to the List
    //moreover, count the urls
    DLListStr invertedList = newDLListStr();
    readInvertedIndex(invertedList, collectedList);

    showDLListStr(invertedList);

    DLListStr pagerankList = newDLListStr();
    getPageRankToList(pagerankList);
    showDLListStr(pagerankList);

    DLListStr resultList = newDLListStr();
    getResultList(resultList, invertedList, pagerankList);
    showDLListStr(resultList);

    return 0;
}