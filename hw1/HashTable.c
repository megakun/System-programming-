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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CSE333.h"
#include "HashTable.h"
#include "HashTable_priv.h"
#include "LinkedList.h"
#include "LinkedList_priv.h"

///////////////////////////////////////////////////////////////////////////////
// Internal helper functions.
//
#define INVALID_IDX -1

// Grows the hashtable (ie, increase the number of buckets) if its load
// factor has become too high.
static void MaybeResize(HashTable *ht);

int HashKeyToBucketNum(HashTable *ht, HTKey_t key) {
  return key % ht->num_buckets;
}

// Deallocation functions that do nothing.  Useful if we want to deallocate
// the structure (eg, the linked list) without deallocating its elements or
// if we know that the structure is empty.
static void LLNoOpFree(LLPayload_t freeme) { }
static void HTNoOpFree(HTValue_t freeme) { }


///////////////////////////////////////////////////////////////////////////////
// HashTable implementation.

HTKey_t FNVHash64(unsigned char *buffer, int len) {
  // This code is adapted from code by Landon Curt Noll
  // and Bonelli Nicola:
  //     http://code.google.com/p/nicola-bonelli-repo/
  static const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
  static const uint64_t FNV_64_PRIME = 0x100000001b3ULL;
  unsigned char *bp = (unsigned char *) buffer;
  unsigned char *be = bp + len;
  uint64_t hval = FNV1_64_INIT;

  // FNV-1a hash each octet of the buffer.
  while (bp < be) {
    // XOR the bottom with the current octet.
    hval ^= (uint64_t) * bp++;
    // Multiply by the 64 bit FNV magic prime mod 2^64.
    hval *= FNV_64_PRIME;
  }
  return hval;
}

HashTable* HashTable_Allocate(int num_buckets) {
  HashTable *ht;
  int i;

  Verify333(num_buckets > 0);

  // Allocate the hash table record.
  ht = (HashTable *) malloc(sizeof(HashTable));

  // Initialize the record.
  ht->num_buckets = num_buckets;
  ht->num_elements = 0;
  ht->buckets = (LinkedList **) malloc(num_buckets * sizeof(LinkedList *));
  for (i = 0; i < num_buckets; i++) {
    ht->buckets[i] = LinkedList_Allocate();
  }

  return ht;
}

void HashTable_Free(HashTable *table,
                    ValueFreeFnPtr value_free_function) {
  int i;

  Verify333(table != NULL);

  // Free each bucket's chain.
  for (i = 0; i < table->num_buckets; i++) {
    LinkedList *bucket = table->buckets[i];
    HTKeyValue_t *kv;

    // Pop elements off the chain list one at a time.  We can't do a single
    // call to LinkedList_Free since we need to use the passed-in
    // value_free_function -- which takes a HTValue_t, not an LLPayload_t -- to
    // free the caller's memory.
    while (LinkedList_NumElements(bucket) > 0) {
      Verify333(LinkedList_Pop(bucket, (LLPayload_t *)&kv));
      value_free_function(kv->value);
      free(kv);
    }
    // The chain is empty, so we can pass in the
    // null free function to LinkedList_Free.
    LinkedList_Free(bucket, LLNoOpFree);
  }

  // Free the bucket array within the table, then free the table record itself.
  free(table->buckets);
  free(table);
}

int HashTable_NumElements(HashTable *table) {
  Verify333(table != NULL);
  return table->num_elements;
}
// helper function to check if key already exist
// return true or false
static bool BucketHasKey(LLIterator *iter,
                         HTKey_t key) {
  Verify333(iter != NULL);
  while (1) {
    HTKeyValue_t* payload;
    LLIterator_Get(iter, (LLPayload_t)&payload);
    if (payload->key == key) {  // found key
      return true;

    } else if (iter->node->next == NULL) {  // if end of the list is reached
      return false;
    }  // contiue to advance until the end
    LLIterator_Next(iter);
  }
}

bool HashTable_Insert(HashTable *table,
                      HTKeyValue_t newkeyvalue,
                      HTKeyValue_t *oldkeyvalue) {
  int bucket;
  LinkedList *chain;

  Verify333(table != NULL);
  MaybeResize(table);

  // Calculate which bucket and chain we're inserting into.
  bucket = HashKeyToBucketNum(table, newkeyvalue.key);
  chain = table->buckets[bucket];

  // STEP 1: finish the implementation of InsertHashTable.
  // This is a fairly complex task, so you might decide you want
  // to define/implement a helper function that helps you find
  // and optionally remove a key within a chain, rather than putting
  // all that logic inside here.  You might also find that your helper
  // can be reused in steps 2 and 3.
  HTKeyValue_t* newpayload = (HTKeyValue_t*)malloc(sizeof(HTKeyValue_t));
  Verify333(newpayload != NULL);
  newpayload->key = newkeyvalue.key;
  newpayload->value = newkeyvalue.value;

  if (LinkedList_NumElements(chain) == 0) {  // empty bucket
    LinkedList_Push(chain, (LLPayload_t*)newpayload);
    table->num_elements++;
    return false;
  }
  LLIterator* iter = LLIterator_Allocate(chain);
  Verify333(iter != NULL);

  if (BucketHasKey(iter, newkeyvalue.key)) {  // if same key exists
      HTKeyValue_t* oldpayload;
      LLIterator_Get(iter, &oldpayload);
      oldkeyvalue->key = oldpayload->key;
      oldkeyvalue->value = oldpayload->value;
      oldpayload->value = newkeyvalue.value;
      free(newpayload);
      LLIterator_Free(iter);
      return true;
    } else {
      LinkedList_Push(chain, (LLPayload_t*)newpayload);
      table->num_elements++;
      LLIterator_Free(iter);
      return false;
    }
}

bool HashTable_Find(HashTable *table,
                    HTKey_t key,
                    HTKeyValue_t *keyvalue) {
  Verify333(table != NULL);
  int bucket;
  LinkedList *chain;
  // Calculate which bucket and chain we're finding.
  bucket = HashKeyToBucketNum(table, key);
  chain = table->buckets[bucket];
  // STEP 2: implement HashTable_Find.
  if (LinkedList_NumElements(chain) == 0) {  // if the bucket is empty
    return false;
  }
  LLIterator* iter = LLIterator_Allocate(chain);
  Verify333(iter != NULL);

  if (BucketHasKey(iter, key)) {
    HTKeyValue_t* payload;
    LLIterator_Get(iter, &payload);
    keyvalue->key = key;
    keyvalue->value = payload->value;
    LLIterator_Free(iter);
    return true;
  }

  LLIterator_Free(iter);
  return false;  // you may need to change this return value
}

bool HashTable_Remove(HashTable *table,
                      HTKey_t key,
                      HTKeyValue_t *keyvalue) {
  Verify333(table != NULL);

  // STEP 3: implement HashTable_Remove.
  int bucket;
  LinkedList *chain;
  // Calculate which bucket and chain we're finding.
  bucket = HashKeyToBucketNum(table, key);
  chain = table->buckets[bucket];

  if (LinkedList_NumElements(chain) == 0) {  // if the bucket is empty
    return false;
  }
  LLIterator* iter = LLIterator_Allocate(chain);
  Verify333(iter != NULL);


  if (BucketHasKey(iter, key)) {
    HTKeyValue_t* payload;
    LLIterator_Get(iter, &payload);
    keyvalue->key = key;
    keyvalue->value = payload->value;
    free(payload);
    LLIterator_Remove(iter, LLNoOpFree);
    LLIterator_Free(iter);
    table->num_elements--;
    return true;
  } else {
  // cannot find payload
  LLIterator_Free(iter);
  return false;  // you may need to change this return value
  }
}


///////////////////////////////////////////////////////////////////////////////
// HTIterator implementation.

HTIterator* HTIterator_Allocate(HashTable *table) {
  HTIterator *iter;
  int         i;

  Verify333(table != NULL);

  iter = (HTIterator *) malloc(sizeof(HTIterator));

  // If the hash table is empty, the iterator is immediately invalid,
  // since it can't point to anything.
  if (table->num_elements == 0) {
    iter->ht = table;
    iter->bucket_it = NULL;
    iter->bucket_idx = INVALID_IDX;
    return iter;
  }

  // Initialize the iterator.  There is at least one element in the
  // table, so find the first element and point the iterator at it.
  iter->ht = table;
  for (i = 0; i < table->num_buckets; i++) {
    if (LinkedList_NumElements(table->buckets[i]) > 0) {
      iter->bucket_idx = i;
      break;
    }
  }
  Verify333(i < table->num_buckets);  // make sure we found it.
  iter->bucket_it = LLIterator_Allocate(table->buckets[iter->bucket_idx]);
  return iter;
}

void HTIterator_Free(HTIterator *iter) {
  Verify333(iter != NULL);
  if (iter->bucket_it != NULL) {
    LLIterator_Free(iter->bucket_it);
    iter->bucket_it = NULL;
  }
  free(iter);
}

bool HTIterator_IsValid(HTIterator *iter) {
  Verify333(iter != NULL);
  LLIterator* lliter = iter->bucket_it;
  // STEP 4: implement HTIterator_IsValid.
  if (iter->bucket_it == NULL) {
    return false;
  } else if (iter->bucket_idx == iter->ht->num_buckets) {
    LLIterator_Free(iter->bucket_it);
    return false;
  } else if (iter->ht->num_elements == 0) {
    LLIterator_Free(iter->bucket_it);
    return false;
  } else if (!LLIterator_IsValid(lliter)) {
    LLIterator_Free(iter->bucket_it);
    return false;
  } else {
  return true;
  }
}

bool HTIterator_Next(HTIterator *iter) {
  Verify333(iter != NULL);

  // STEP 5: implement HTIterator_Next.
  if (!HTIterator_IsValid(iter)) {
    return false;
  }
  // has next node on current list
  if (LLIterator_Next(iter->bucket_it)) {
    return true;
  }

  // finding next bucket
  for (int i = iter->bucket_idx + 1; i < iter->ht->num_buckets; i++) {
     if (LinkedList_NumElements(iter->ht->buckets[i]) > 0) {
      LLIterator_Free(iter->bucket_it);
      iter->bucket_idx = i;
      iter->bucket_it = LLIterator_Allocate(iter->ht->buckets[i]);
      return true;
    }
  }
  iter->bucket_it = NULL;
  return false;
}

bool HTIterator_Get(HTIterator *iter, HTKeyValue_t *keyvalue) {
  Verify333(iter != NULL);

  // STEP 6: implement HTIterator_Get.

  if (!HTIterator_IsValid(iter)) {
    return false;
  } else {
    HTKeyValue_t* kv;
    LLIterator_Get(iter->bucket_it, (LLPayload_t **)&kv);
    keyvalue->key = kv->key;
    keyvalue->value = kv->value;
    return true;
  }
}

bool HTIterator_Remove(HTIterator *iter, HTKeyValue_t *keyvalue) {
  HTKeyValue_t kv;

  Verify333(iter != NULL);

  // Try to get what the iterator is pointing to.
  if (!HTIterator_Get(iter, &kv)) {
    return false;
  }

  // Advance the iterator.  Thanks to the above call to
  // HTIterator_Get, we know that this iterator is valid (though it
  // may not be valid after this call to HTIterator_Next).
  HTIterator_Next(iter);

  // Lastly, remove the element.  Again, we know this call will succeed
  // due to the successful HTIterator_Get above.
  Verify333(HashTable_Remove(iter->ht, kv.key, keyvalue));
  Verify333(kv.key == keyvalue->key);
  Verify333(kv.value == keyvalue->value);

  return true;
}

static void MaybeResize(HashTable *ht) {
  HashTable *newht;
  HashTable tmp;
  HTIterator *it;

  // Resize if the load factor is > 3.
  if (ht->num_elements < 3 * ht->num_buckets)
    return;

  // This is the resize case.  Allocate a new hashtable,
  // iterate over the old hashtable, do the surgery on
  // the old hashtable record and free up the new hashtable
  // record.
  newht = HashTable_Allocate(ht->num_buckets * 9);

  // Loop through the old ht copying its elements over into the new one.
  for (it = HTIterator_Allocate(ht);
       HTIterator_IsValid(it);
       HTIterator_Next(it)) {
    HTKeyValue_t item, unused;

    Verify333(HTIterator_Get(it, &item));
    HashTable_Insert(newht, item, &unused);
  }

  // Swap the new table onto the old, then free the old table (tricky!).  We
  // use the "no-op free" because we don't actually want to free the elements;
  // they're owned by the new table.
  tmp = *ht;
  *ht = *newht;
  *newht = tmp;

  // Done!  Clean up our iterator and temporary table.
  HTIterator_Free(it);
  HashTable_Free(newht, &HTNoOpFree);
}
