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
#include "./DocTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhw1/CSE333.h"
#include "libhw1/HashTable.h"

#define HASHTABLE_INITIAL_NUM_BUCKETS 2

// This structure represents a DocTable; it contains two hash tables, one
// mapping from docid to docname, and one mapping from docname to docid.
struct doctable_st {
  HashTable *docid_to_docname;  // mapping docid to docname
  HashTable *docname_to_docid;  // mapping docname to docid
  DocID_t    max_id;            // max docID allocated so far
};

DocTable* DocTable_Allocate(void) {
  DocTable *dt = (DocTable *) malloc(sizeof(DocTable));

  dt->docid_to_docname = HashTable_Allocate(HASHTABLE_INITIAL_NUM_BUCKETS);
  dt->docname_to_docid = HashTable_Allocate(HASHTABLE_INITIAL_NUM_BUCKETS);
  dt->max_id = 1;  // we reserve max_id = 0 for the invalid docID

  return dt;
}

void DocTable_Free(DocTable *table) {
  Verify333(table != NULL);

  // STEP 1.
  HashTable_Free(table->docid_to_docname, free);
  HashTable_Free(table->docname_to_docid, free);
  free(table);
}

int DocTable_NumDocs(DocTable *table) {
  Verify333(table != NULL);
  return HashTable_NumElements(table->docid_to_docname);
}

DocID_t DocTable_Add(DocTable *table, char *docname) {
  char *doccopy;
  DocID_t *docid;
  DocID_t res;
  HTKeyValue_t kv, oldkv;

  Verify333(table != NULL);

  // STEP 2.
  // Check to see if the document already exists.  Then make a copy of the
  // docname and allocate space for the new ID.
  HTKey_t k = FNVHash64(docname, strlen(docname));
    if (HashTable_Find(table->docname_to_docid, k, &oldkv)) {
     res = *((DocID_t *)oldkv.value);
     return res;
    }

  doccopy = (char *) malloc(1+strlen(docname));
  if (doccopy != NULL) {
     strncpy(doccopy, docname, 1+strlen(docname));
  }

  docid = (DocID_t *)malloc(sizeof(DocID_t));
  *docid = table->max_id;
  table->max_id++;

  // STEP 3.
  // Set up the key/value for the id->name mapping, and do the insert.
  kv.key = *docid;
  kv.value = doccopy;
  HashTable_Insert(table->docid_to_docname, kv, &oldkv);

  // STEP 4.
  // Set up the key/value for the name->id, and/ do the insert.
  // Be careful about how you calculate the key for this mapping.
  // You want to be sure that how you do this is consistent with
  // the provided code.
  kv.key = k;
  kv.value = docid;
  HashTable_Insert(table->docname_to_docid, kv, &oldkv);

  return *docid;
}

DocID_t DocTable_GetDocID(DocTable *table, char *docname) {
  HTKey_t key;
  HTKeyValue_t kv;
  DocID_t res;

  Verify333(table != NULL);
  Verify333(docname != NULL);
  // STEP 5.
  // Try to find the passed-in doc in docname_to_docid table.
  HTKey_t k = FNVHash64(docname, strlen(docname));
    if (HashTable_Find(table->docname_to_docid, k, &kv)) {
     res = *((DocID_t *)kv.value);
     return res;
    } else  {
     return INVALID_DOCID;
    }
}

char *DocTable_GetDocName(DocTable *table, DocID_t docid) {
  HTKeyValue_t kv;

  Verify333(table != NULL);
  Verify333(docid != INVALID_DOCID);

  // STEP 6.
  // Lookup the docid in the docid_to_docname table,
  // and either return the string (i.e., the (char *)
  // saved in the value field for that key) or
  // NULL if the key isn't in the table.
  if (HashTable_Find(table->docid_to_docname, docid, &kv)) {
     return kv.value;
  } else  {
     return NULL;
  }
}

HashTable* DT_GetDocidToDocnameTable(DocTable *table) {
  Verify333(table != NULL);
  return table->docid_to_docname;
}

HashTable* DT_GetDocnameToDocidTable(DocTable *table) {
  Verify333(table != NULL);
  return table->docname_to_docid;
}
