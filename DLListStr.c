/*
 Implementation of doubly-linked list ADT for string values.
 Written by Ashesh Mahidadia Jan 2020, based on code writted by John Shepherd
 2015. You may need to modify the following implementation and test it properly
 before using in your program.
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// data structures representing DLListStr

typedef struct DLListRep *DLListStr;

typedef struct DLListNode {
  char value[100]; // value of this list item (string)
  int degree;
  double pagerank;
  int count;
  int index;
  struct DLListNode *prev;
  // pointer previous node in list
  struct DLListNode *next;
  // pointer to next node in list
  DLListStr sub; // sub list
  DLListStr urls;
} DLListNode;

typedef struct DLListRep {
  int nitems;        // count of items in list
  DLListNode *first; // first node in list
  DLListNode *curr;  // current node in list
  DLListNode *last;  // last node in list
} DLListRep;

// create a new DLListNode (private function)
static DLListNode *newDLListNode(char *val, int outdegree, double pagerank,
                                 int count) {
  DLListNode *new;
  new = malloc(sizeof(DLListNode));
  assert(new != NULL);

  strcpy(new->value, val); // for int, new->value = it;

  new->prev = new->next = NULL;
  new->degree = outdegree;
  new->pagerank = pagerank;
  new->sub = NULL;
  new->count = count;
  return new;
}

// create a new empty DLListStr
DLListStr newDLListStr() {
  struct DLListRep *L;

  L = malloc(sizeof(struct DLListRep));
  assert(L != NULL);
  L->nitems = 0;
  L->first = NULL;
  L->last = NULL;
  L->curr = NULL;
  return L;
}

bool checkVal(DLListStr L, char *val) {
  DLListNode *temp = L->first;
  while (temp != NULL) {
    if (strcmp(temp->value, val) == 0) {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

int compareToByVal(DLListNode *first, DLListNode *second) {
  assert(first != NULL && second != NULL);
  // if first < second : return -1
  // if first == second : return 0
  // if first > second : return 1
  int result = 0;
  result = strcmp(first->value, second->value);
  // if (first->value < second->value) {
  //     result = -1;
  // } else if (first->value > second->value) {
  //     result = 1;
  //}
  return result;
}

int compareByPagerank(char *val, int outdegree, double pagerank,
                      DLListNode *present) {
  int result = 0;
  if (pagerank < present->pagerank) {
    result = -1;
  } else if (pagerank > present->pagerank) {
    result = 1;
  } else {
    if (outdegree < present->degree) {
      result = -1;
    } else if (outdegree > present->degree) {
      result = 1;
    } else {
      result = strcmp(val, present->value);
    }
  }
  return result;
}

/*
   pre-reqisite: L is ordered (increasing) with no duplicates
   post-condition: val is inserted in L, L is ordered (increasing) with no
   duplicates
*/
void insertSetOrd(DLListStr L, char *val) {

  /*
     implement this function to
     insert val in L  (in order), no duplicates (set)

        */
  DLListNode *newNode = newDLListNode(val, 0, 0.0, 0);
  L->nitems++;
  int c = 1;
  newNode->index = c;
  c++;
  if (L->first == NULL) {
    L->first = newNode;
    L->last = L->first;
  } else {
    if (compareToByVal(newNode, L->first) < 0) {
      newNode->next = L->first;
      L->first->prev = newNode;
      L->first = newNode;
    } else if (compareToByVal(newNode, L->last) > 0) {
      L->last->next = newNode;
      newNode->prev = L->last;
      L->last = newNode;
    } else if (checkVal(L, val) == false) {
      DLListNode *pre = L->first;
      DLListNode *curr = L->first->next;
      while (curr != NULL) {
        if (compareToByVal(newNode, pre) > 0 &&
            compareToByVal(newNode, curr) < 0) {
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

// insert by order from pagerank, degree, val
void insertSetOrdPageRankAndoutDegree(DLListStr L, char *val, int outdegree,
                                      double pagerank, int count) {
  DLListNode *newNode = newDLListNode(val, outdegree, pagerank, count);
  L->nitems++;
  if (L->first == NULL) {
    L->first = newNode;
    L->last = L->first;
  } else {
    if (compareByPagerank(val, outdegree, pagerank, L->first) > 0) {
      newNode->next = L->first;
      L->first->prev = newNode;
      L->first = newNode;
    } else if (compareByPagerank(val, outdegree, pagerank, L->last) < 0) {
      L->last->next = newNode;
      newNode->prev = L->last;
      L->last = newNode;
    } else if (checkVal(L, val) == false) {
      DLListNode *pre = L->first;
      DLListNode *curr = L->first->next;
      while (curr != NULL) {
        if (compareByPagerank(val, outdegree, pagerank, pre) < 0 &&
            compareByPagerank(val, outdegree, pagerank, curr) > 0) {
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

// find the val which can pare to the page link and insert to the sub-list
void insertSubList(DLListStr L, char *val, char *url) {
  DLListNode *present = L->first;
  while (present != NULL) {
    if (strcmp(present->value, val) == 0) {
      if (present->sub == NULL) {
        present->sub = newDLListStr();
      }
      insertSetOrd(present->sub, url);
    }
    present = present->next;
  }
}

int compareByCountAndPagerank(DLListNode *new, DLListNode *present) {
  int result = 0;
  if (new->count < present->count) {
    result = -1;
  } else if (new->count > present->count) {
    result = 1;
  } else {
    if (new->pagerank < present->pagerank) {
      result = -1;
    } else if (new->pagerank > present->pagerank) {
      result = 1;
    } else {
      result = strcmp(new->value, present->value);
    }
  }
  return result;
}

// insert by order from count, pagerank, val
void insertSetOrdCountAndPageRank(DLListStr L, char *val, int count,
                                  double pagerank, int outdegree) {
  DLListNode *newNode = newDLListNode(val, outdegree, pagerank, count);
  L->nitems++;
  if (L->first == NULL) {
    L->first = newNode;
    L->last = L->first;
  } else {
    if (compareByCountAndPagerank(newNode, L->first) > 0) {
      newNode->next = L->first;
      L->first->prev = newNode;
      L->first = newNode;
    } else if (compareByCountAndPagerank(newNode, L->last) < 0) {
      L->last->next = newNode;
      newNode->prev = L->last;
      L->last = newNode;
    } else if (checkVal(L, val) == false) {
      DLListNode *pre = L->first;
      DLListNode *curr = L->first->next;
      while (curr != NULL) {
        if (compareByCountAndPagerank(newNode, pre) < 0 &&
            compareByCountAndPagerank(newNode, curr) > 0) {
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

void countByVal(DLListStr L, char *val, int count) {
  DLListNode *container = L->first;
  while (container != NULL) {
    if (strcmp(container->value, val) == 0) {
      container->count += count;
    }
    container = container->next;
  }
}

// display items from a DLListStr, comma separated
void showDLListStr(DLListStr L, FILE *fp) {
  assert(L != NULL);
  DLListNode *curr;
  int count = 0;
  for (curr = L->first; curr != NULL; curr = curr->next) {
    count++;
    if (count > 1) {
      fprintf(fp, " ");
    }
    fprintf(fp, "%s", curr->value);
  }
  fprintf(fp, "\n");
}

// free up all space associated with list
void freeDLListStr(DLListStr L) {
  assert(L != NULL);
  DLListNode *curr, *prev;
  curr = L->first;
  while (curr != NULL) {
    prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(L);
}
