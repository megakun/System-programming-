/*
 * Copyright ©2020 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Summer Quarter 2020 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
static void ProcessQueries(DocTable *dt, MemIndex *mi);
static void PrintResult(LinkedList* result, DocTable* doctable);

//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char **argv) {
  if (argc != 2) {
    Usage();
  }
  DocTable *doctable;
  MemIndex *index;
  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  printf("Indexing \' %s\' \n",  argv[1]);
  if (!CrawlFileTree(argv[1], &doctable, &index)) {
    Usage();
  }
  // Process Query
  ProcessQueries(doctable, index);

  DocTable_Free(doctable);
  MemIndex_Free(index);

  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable *dt, MemIndex *mi) {
  char *saveptr;
  char buffer[1024];

  while (1) {
    printf("enter query:\n");

    if (fgets(buffer, 1024, stdin) != NULL) {
      char **query = (char **)malloc(1024 * sizeof(char *));
      char *str = buffer;
      char* token;
      int qlen = 0;

      while (1) {
        token = strtok_r(str, " ", &saveptr);
        if (token == NULL) {
          break;
        }
        query[qlen] = token;
        qlen++;
        str = NULL;
      }

      char *p = strchr(query[qlen - 1], '\n');
      *p = '\0';

      LinkedList* result = MemIndex_Search(mi, query, qlen);
      if (result == NULL) {
        free(query);
        continue;  // prompt for next import
      }
      PrintResult(result, dt);
      free(query);
      LinkedList_Free(result, &free);
    }
  }
}
// helper function
// prints out one row of result
// including file path and rank
static void PrintResult(LinkedList* result, DocTable* doctable) {
  LLIterator* llitr = LLIterator_Allocate(result);
  Verify333(llitr != NULL);

  while (LLIterator_Next(llitr)) {
    SearchResult* res;
    LLIterator_Get(llitr, (LLPayload_t *)&res);
    DocID_t docid = res->docid;
    // find the directory name
    char *dir = DocTable_GetDocName(doctable, docid);
    printf("  %s (%d)\n", dir, res->rank);
  }
  LLIterator_Free(llitr);
}

