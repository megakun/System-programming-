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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

QueryProcessor::QueryProcessor(const list<string> &indexlist, bool validate) {
  // Stash away a copy of the index list.
  indexlist_ = indexlist;
  arraylen_ = indexlist_.size();
  Verify333(arraylen_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader *[arraylen_];
  itr_array_ = new IndexTableReader *[arraylen_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = indexlist_.begin();
  for (int i = 0; i < arraylen_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < arraylen_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}
// helper function
vector<QueryProcessor::QueryResult>
getVector(DocTableReader **dtr_array_, IndexTableReader **itr_array_,
                 int arraylen_, const string word) {
  vector<QueryProcessor::QueryResult> resVector;

  for (int i = 0; i < arraylen_; i++) {
    DocIDTableReader* reader = itr_array_[i]->LookupWord(word);
    if (reader != nullptr) {
    list<DocIDElementHeader> list = reader->GetDocIDList();

      while (!list.empty()) {
        DocIDElementHeader header = list.front();
        list.pop_front();
        string doc;

        if (dtr_array_[i]->LookupDocID(header.docID, &doc)) {
          QueryProcessor::QueryResult res;
          res.documentName = doc;
          res.rank = header.numPositions;
          resVector.push_back(res);
        }
      }
    }
    delete reader;
  }
  return resVector;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t docid;  // The document ID within the index file.
  int rank;       // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string> &query) {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> finalresult;

  // get the first query, return the first vector
  string first = query.front();
  vector<QueryProcessor::QueryResult> firstresult
               = getVector(dtr_array_, itr_array_, arraylen_, first);
  if (firstresult.size() == 0) {  // return empty result
    return firstresult;
  }

  // multiple query
  if (query.size() > 1) {
  auto iter = query.begin();
  iter++;
    while (iter != query.end()) {  // loopthrough all query
    // get the next query then compare to the inital result
    // delete and update result
      vector<QueryProcessor::QueryResult> nextresult
                = getVector(dtr_array_, itr_array_, arraylen_, *iter);

      if (nextresult.size() == 0 || firstresult.size() == 0) {
        // no more match found, return
        return finalresult;
      }

      auto first_iter = firstresult.begin();
      while (first_iter!= firstresult.end()) {
        auto next_iter = nextresult.begin();
        bool found = false;

        while (next_iter != nextresult.end()) {
          if (next_iter->documentName.compare(first_iter->documentName) == 0) {
               found = true;
               break;
          }
          next_iter++;
        }

        if (found) {
          first_iter->rank += next_iter->rank;
          first_iter++;
        } else {
          firstresult.erase(first_iter);  // if no common match
        }
      }
      iter++;
    }
  }
  finalresult = firstresult;
  // Sort the final results.
  sort(finalresult.begin(), finalresult.end());
  return finalresult;
}

}  // namespace hw3
