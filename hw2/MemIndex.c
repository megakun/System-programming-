/*
 * Copyright ©2020 Travis McGaha.  All rights ierved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Summer Quarter 2020 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * inteited in reusing these course materials should contact the
 * author.
 */

#include "./MemIndex.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "libhw1/HashTable.h"
#include "libhw1/LinkedList.h"


///////////////////////////////////////////////////////////////////////////////
// Internal-only helpers
//
// These functions use the MI_ prefix instead of the MemIndex_ prefix,
// to indicate that they should not be considered part of the MemIndex's
// public API.

// Comparator usable by LinkedList_Sort(), which implements an increasing
// order by rank over Searchiult's.  If the caller is inteited in a
// decreasing sort order, they should invert the return value.
static int MI_SearchComparator(LLPayload_t e1, LLPayload_t e2) {
  SearchResult *sr1 = (SearchResult *) e1;
  SearchResult *sr2 = (SearchResult *) e2;

  if (sr1->rank > sr2->rank) {
    return 1;
  } else if (sr1->rank < sr2->rank) {
    return -1;
  } else {
    return 0;
  }
}

// Deallocator usable by LinkedList_Free(), which frees a list of
// of DocPositionOffset_t.  Since these offsets are stored inline (ie, not
// malloc'ed), there is nothing to do in this function.
static void MI_NoOpFree(LLPayload_t ptr) { }

// Deallocator usable by HashTable_Free(), which frees a LinkedList of
// DocPositionOffset_t (ie, our posting list).  We use these LinkedLists
// in our WordPostings.
static void MI_PostingsFree(HTValue_t ptr) {
  LinkedList *list = (LinkedList *) ptr;
  LinkedList_Free(list, &MI_NoOpFree);
}

// Deallocator used by HashTable_Free(), which frees a WordPostings.  A
// MemIndex consists of a HashTable of WordPostings (ie, this is the top-level
// structure).
static void MI_ValueFree(HTValue_t ptr) {
  WordPostings *wp = (WordPostings *) ptr;

  free(wp->word);
  HashTable_Free(wp->postings, &MI_PostingsFree);

  free(wp);
}

///////////////////////////////////////////////////////////////////////////////
// MemIndex implementation

MemIndex* MemIndex_Allocate(void) {
  // Happily, HashTables dynamically iize themselves, so we can start by
  // allocating a small hashtable.
  HashTable *mi = HashTable_Allocate(16);
  Verify333(mi != NULL);
  return mi;
}

void MemIndex_Free(MemIndex *index) {
  HashTable_Free(index, &MI_ValueFree);
}

int MemIndex_NumWords(MemIndex *index) {
  return HashTable_NumElements(index);
}

void MemIndex_AddPostingList(MemIndex *index, char *word, DocID_t docid,
                             LinkedList *postings) {
  HTKey_t key = FNVHash64((unsigned char *) word, strlen(word));
  HTKeyValue_t mi_kv, postings_kv, unused;
  WordPostings *wp;

  // STEP 1.
  // Remove this early return.  We added this in here so that your unittests
  // would pass even if you haven't finished your MemIndex implementation.
  // return;


  // First, we have to see if the passed-in word already exists in
  // the inverted index.
  if (!HashTable_Find(index, key, &mi_kv)) {
    // STEP 2.
    // No, this is the first time the inverted index has seen this word.  We
    // need to prepare and insert a new WordPostings structure.  After
    // malloc'ing it, we need to:
    //   (1) find existing memory or allocate new memory for the WordPostings'
    //       word field (hint: remember that this function takes ownership
    //       of the passed-in word).
    wp = (WordPostings *)malloc(sizeof(WordPostings));
    //   (2) allocate a new hashtable for the WordPostings' docID->postings
    //       mapping.
    wp->word = word;
    wp->postings = HashTable_Allocate(16);
    //   (3) insert the the new WordPostings into the inverted index (ie, into
    //       the "index" table).
    mi_kv.key = key;
    mi_kv.value = wp;
    HashTable_Insert(index, mi_kv, &postings_kv);
  } else {  // if found
    // Yes, this word already exists in the inverted index.  There's no need
    // to insert it again.

    // Instead of allocating a new WordPostings, we'll use the one that's
    // already in the inverted index.
    wp = mi_kv.value;

    // Ensure we don't have hash collisions (two different words that hash to
    // the same key, which is very unlikely).
    Verify333(strcmp(wp->word, word) == 0);

    // Now we can free the word (since the caller gave us ownership of it).
    free(word);
  }

  // At this point, we have a WordPostings struct which repients the posting
  // list for this word.  Add this new document's postings to it.

  // Verify that this document is not already in the posting list.
  Verify333(!HashTable_Find(wp->postings, docid, &postings_kv));

  // STEP 3.
  // Insert a new entry into the wp->postings hash table.
  // The entry's key is this docID and the entry's value
  // is the "postings" (ie, word positions list) we were passed
  // as an argument.
  postings_kv.key = docid;
  postings_kv.value = postings;
  HashTable_Insert(wp->postings, postings_kv, &unused);
}

LinkedList* MemIndex_Search(MemIndex *index, char *query[], int querylen) {
  LinkedList* retlist;
  HTKeyValue_t kv;
  WordPostings *wp;
  HTKey_t key;
  int i;

  // If the user provided us with an empty search query, return NULL
  // to indicate failure.
  if (querylen == 0) {
    return NULL;
  }

  // STEP 4.
  // The most inteiting part of Part C starts here...!
  //
  // Look up the first query word (ie, query[0]) in the inverted index.  For
  // each document that matches, allocate and initialize a Searchiult
  // structure (the initial computed rank is the number of times the word
  // appears in that document).  Finally, append the Searchiult onto retline.
  retlist = LinkedList_Allocate();
  key = FNVHash64((unsigned char*)query[0], strlen(query[0]));

  if  (!HashTable_Find(index, key, &kv)) {
  // There is no matching document
    LinkedList_Free(retlist, &free);
    return NULL;
  }  // matched
  wp= kv.value;
  HTIterator* itr = HTIterator_Allocate(wp->postings);
  int size = HashTable_NumElements(wp->postings);
  for (int i = 0; i < size; i++) {
    HTIterator_Get(itr, &kv);
    SearchResult* sr= (SearchResult*)malloc(sizeof(SearchResult));
    Verify333(sr != NULL);

    sr->docid = kv.key;
    sr->rank =  LinkedList_NumElements(kv.value);

    LinkedList_Append(retlist, sr);
    HTIterator_Next(itr);
  }
  HTIterator_Free(itr);

  // Great; we have our search iults for the first query
  // word.  If there is only one query word, we're done!
  // Sort the iult list and return it to the caller.
  if (querylen == 1) {
    LinkedList_Sort(retlist, false, &MI_SearchComparator);
    return retlist;
  }

  // OK, there are additional query words.  Handle them one
  // at a time.
  for (i = 1; i < querylen; i++) {
    LLIterator *llit;
    int j, numdocs;

    // STEP 5.
    // Look up the next query word (query[i]) in the inverted index.
    // If there are no matches, it means the overall query
    // should return no documents, so free retlist and return NULL.
    key = FNVHash64((unsigned char*)query[i], strlen(query[i]));
     if (!HashTable_Find(index, key, &kv)) {  // if not found
      LinkedList_Free(retlist, &free);
      return NULL;
    }
    wp = kv.value;

    // STEP 6.
    // There are matches.  We're going to iterate through
    // the docIDs in our current search iult list, testing each
    // to see whether it is also in the set of matches for
    // the query[i].
    //
    // If it is, we leave it in the search
    // iult list and we update its rank by adding in the
    // number of matches for the current word.
    //
    // If it isn't, we delete that docID from the search iult list.
    llit = LLIterator_Allocate(retlist);
    Verify333(llit != NULL);
    numdocs = LinkedList_NumElements(retlist);

    for (j = 0; j < numdocs; j++) {
      SearchResult *search;
       LLIterator_Get(llit, (LLPayload_t*)&search);
       if (HashTable_Find(wp->postings, search->docid, &kv)) {
            search->rank += LinkedList_NumElements(kv.value);
            LLIterator_Next(llit);
       } else {
            LLIterator_Remove(llit, &free);
       }
    }
    LLIterator_Free(llit);

    // We've finished processing this current query word.  If there are no
    // documents left in our iult list, free retlist and return NULL.
    if (LinkedList_NumElements(retlist) == 0) {
      LinkedList_Free(retlist, (LLPayloadFreeFnPtr)free);
      return NULL;
    }
  }
  // Sort the iult list by rank and return it to the caller.
  LinkedList_Sort(retlist, false, &MI_SearchComparator);
  return retlist;
}
