
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DataOperate.h"
#include "DLListStr.h"
#define File_patt ".txt"

const char *output_file1 = "pagerankList.txt";
const char *output_file2 = "invertedIndex.txt";

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

char *strConcat(char *str1, char *str2)
{
  int length = strlen(str1) + strlen(str2) + 1;
  char *result = (char *)malloc(sizeof(char) * length);
  strcpy(result, str1);
  strcat(result, str2);

  return result;
}

void getCollection(DLListStr dataUrls)
{
  char delim[3] = " \n";
  char *token;
  char line[MAXSTRING];
  FILE *f;

  if ((f = fopen("collection.txt", "r")) == NULL)
  {
    printf("Error!\n");
  }
  while (fgets(line, MAXSTRING, f) != NULL)
  {
    /*first token*/
    token = strtok(line, delim);

    int count = 0;
    /* iterate over the rest of the tokens */
    while (token != NULL)
    {
      count++;
      token = trim(token);
      if (token[0] != '\0')
      {
        insertSetOrd(dataUrls, token);
      }
      token = strtok(NULL, delim);
    }
  }
  fclose(f);
}

int getIndexFromList(DLListStr dataUrls, char *val)
{
  int result = 0;
  DLListNode *container = dataUrls->first;
  while (container != NULL)
  {
    if (strcmp(container->value, val) == 0)
    {
      return result;
    }
    result++;
    container = container->next;
  }
  return result;
}

void readSection1(DLListStr dataUrls, Graph g)
{
  char delim[3] = " \n";
  char *token;
  char line[MAXSTRING];
  FILE *f;
  char filename[MAXSTRING];

  Edge edge;

  DLListNode *container = dataUrls->first;
  while (container != NULL)
  {

    strcpy(filename, container->value);
    strcat(filename, File_patt);
    filename[strlen(container->value) + strlen(File_patt)] = '\0';

    edge.v = getIndexFromList(dataUrls, container->value);

    if ((f = fopen(filename, "r")) == NULL)
    {
      printf("Error!\n");
    }
    else
    {
      int state = 0;
      while (fgets(line, MAXSTRING, f) != NULL)
      {
        /* first token */
        token = strtok(line, delim);

        while (token != NULL)
        {
          token = trim(token);

          if (strcmp(token, "#start") == 0)
          {
            state = 1;
          }
          else if (strcmp(token, "#end") == 0)
          {
            state = 3;
          }
          else if (strcmp(token, "Section-1") == 0)
          {
            state++;
          }
          else if (state == 4)
          {
            break;
          }
          else if (state == 2 && token[0] != '\0')
          {
            edge.w = getIndexFromList(dataUrls, token);
            insertEdge(g, edge);
          }

          token = strtok(NULL, delim);
        }
      }
    }

    fclose(f);

    container = container->next;
  }
}

void normalizeToken(char *token)
{
  for (int i = 0; i < strlen(token); i++)
  {
    token[i] = tolower(token[i]);
    if (strcmp(&token[i], ".") == 0)
    {
      token[i] = '\0';
    }
    else if (strcmp(&token[i], ",") == 0)
    {
      token[i] = '\0';
    }
    else if (strcmp(&token[i], ";") == 0)
    {
      token[i] = '\0';
    }
    else if (strcmp(&token[i], "?") == 0)
    {
      token[i] = '\0';
    }
  }
}

void readSection2(DLListStr dataUrls, DLListStr wordList)
{
  char delim[3] = " \n";
  char *token;
  char line[MAXSTRING];
  FILE *f;
  char filename[MAXSTRING];

  DLListNode *container = dataUrls->first;
  while (container != NULL)
  {

    strcpy(filename, container->value);
    strcat(filename, File_patt);
    filename[strlen(container->value) + strlen(File_patt)] = '\0';

    if ((f = fopen(filename, "r")) == NULL)
    {
      printf("Error!\n");
    }
    else
    {
      while (fgets(line, MAXSTRING, f) != NULL)
      {
        /* first token */
        token = strtok(line, delim);

        while (token != NULL)
        {
          token = trim(token);
          int state = 0;
          if (strcmp(token, "#start") == 0)
          {
            state = 1;
          }
          else if (strcmp(token, "#end") == 0)
          {
            state = 3;
          }
          else if (strcmp(token, "Section-2") == 0)
          {
            state++;
          }
          else if (state == 4)
          {
            break;
          }
          else if (state == 2 && token[0] != '\0')
          {
            //normalize the token
            normalizeToken(token);
            //
            insertSetOrd(wordList, token);

            insertSubList(wordList, token, container->value);
          }

          token = strtok(NULL, delim);
        }
      }
    }

    fclose(f);

    container = container->next;
  }
}

void calculatePageRank(DLListStr dataUrls, Graph g, double d, double diffPR, int maxIteration, int *outDegree, double *pr)
{

  double *prt1 = malloc(sizeof(double) * dataUrls->nitems);

  //calculate the outdegree
  for (int i = 0; i < dataUrls->nitems; i++)
  {
    //initialize the outdegree
    outDegree[i] = 0;

    //initialize pr
    pr[i] = 1.0 / dataUrls->nitems;

    for (int j = 0; j < dataUrls->nitems; j++)
    {

      if (adjacent(g, i, j) == true)
      {
        outDegree[i] += 1;
      }
    }
  }

  int iteration = 0;
  double diff = diffPR;
  while (iteration < maxIteration && diff >= diffPR)
  {
    iteration++;
    for (int i = 0; i < dataUrls->nitems; i++)
    {
      double sum = 0;
      for (int j = 0; j < dataUrls->nitems; j++)
      {
        if (adjacent(g, j, i) == true && outDegree[j] > 0)
        {
          sum += pr[j] / outDegree[j];
        }
      }
      //pr_t+1
      prt1[i] = (1 - d) / dataUrls->nitems + d * sum;
    }

    //calculate the diff
    diff = 0.0;
    for (int i = 0; i < dataUrls->nitems; i++)
    {
      diff += fabs(prt1[i] - pr[i]);
      //current become pre for the next loop
      pr[i] = prt1[i];
    }
  }
  free(prt1);
}

void outputFile(DLListStr dataUrls, int *outDegree, double *pr)
{
  FILE *fp;
  if ((fp = fopen("pagerankList.txt", "w")) == NULL)
  {
    printf("Error!\n");
  }
  else
  {

    DLListStr result = newDLListStr();
    DLListNode *container = dataUrls->first;
    int idx = 0;
    while (container != NULL)
    {
      insertSetOrdPageRankAndoutDegree(result, container->value, outDegree[idx], pr[idx]);
      //printf("%s, %d, %.7f\n", container->value, outDegree[idx], pr[idx]);
      idx++;
      container = container->next;
    }
    container = result->first;
    while (container != NULL)
    {
      printf("%s, %d, %.7f\n", container->value, container->degree, container->pagerank);
      fprintf(fp, "%s, %d, %.7f\n", container->value, container->degree, container->pagerank);
      idx++;
      container = container->next;
    }

    fclose(fp);
  }
}

void outputInvertedFile(DLListStr wordList)
{
  FILE *fp;
  if ((fp = fopen("invertedIndex.txt", "w")) == NULL)
  {
    printf("Error!\n");
  }
  else
  {
    DLListNode *container = wordList->first;

    while (container != NULL)
    {
      fprintf(fp, "%s", container->value);

      if (container->sub != NULL)
      {
        DLListNode *sub = container->sub->first;
        while (sub != NULL)
        {
          fprintf(fp, "%s", sub->value);
          sub = sub->next;
        }
      }
      printf("\n");
      container = container->next;
    }

    fclose(fp);
  }
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

void outputWithLimit(DLListStr resultList, int limit)
{
  FILE *fp;
  if ((fp = fopen(".txt", "w")) == NULL)
  {
    printf("Error!\n");
  }
  else
  {
    DLListNode *curr = resultList->first;
    int count = 0;
    while (curr != NULL)
    {
      fprintf(stdout, "%s\n", curr->value);
      count++;
      if (count == limit)
      {
        break;
      }
      curr = curr->next;
    }
  }
}
