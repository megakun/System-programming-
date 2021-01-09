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

#include <list>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "./QueryProcessor.h"
#include "./test_suite.h"

using std::list;
using std::vector;
using std::string;

namespace hw3 {

TEST(Test_QueryProcessor, TestQueryProcessorSingleIndex) {
  // Test the Single Index Single Word QueryProcessor

  // Set up the list of index files
  list<string> idxlist;
  idxlist.push_back("./unit_test_indices/books.idx");

  // Construct the QueryProcessor.
  QueryProcessor qp(idxlist);

  // Try the a single word query.
  vector<string> query;
  query.push_back("reborn");

  vector<QueryProcessor::QueryResult> res = qp.ProcessQuery(query);
  ASSERT_EQ(3U, res.size());
  ASSERT_EQ(string("test_tree/books/leavesofgrass.txt"), res[0].documentName);
  ASSERT_EQ(3, res[0].rank);
  ASSERT_EQ(string("test_tree/books/ulysses.txt"), res[1].documentName);
  ASSERT_EQ(2, res[1].rank);
  ASSERT_EQ(string("test_tree/books/warandpeace.txt"), res[2].documentName);
  ASSERT_EQ(1, res[2].rank);

  // Test the Single index multi word QueryProcessor
  query.push_back("unbroken");
  res = qp.ProcessQuery(query);
  ASSERT_EQ(2U, res.size());
  ASSERT_EQ(string("test_tree/books/warandpeace.txt"), res[0].documentName);
  ASSERT_EQ(6, res[0].rank);
  ASSERT_EQ(string("test_tree/books/ulysses.txt"), res[1].documentName);
  ASSERT_EQ(3, res[1].rank);

  // Test with expecting no result.
  query.push_back("bio");
  res = qp.ProcessQuery(query);
  ASSERT_EQ(0U, res.size());
  // Done!
  HW3Environment::AddPoints(30);
}

TEST(Test_QueryProcessor, TestQueryMultiIndex) {
  // Set up the list of index files.
  list<string> idxlist;
  idxlist.push_back("./unit_test_indices/bash.idx");
  idxlist.push_back("./unit_test_indices/books.idx");
  idxlist.push_back("./unit_test_indices/enron.idx");

  // Construct the QueryProcessor.
  QueryProcessor qp(idxlist);

  // Try single word query
  vector<string> query;
  query.push_back("kuo");
  vector<QueryProcessor::QueryResult> res = qp.ProcessQuery(query);
  // note how we only have results in 2 of the 3 indexes.
  ASSERT_EQ(2U, res.size());
  ASSERT_EQ(string("test_tree/books/artofwar.txt"), res[0].documentName);
  ASSERT_EQ(8, res[0].rank);
  ASSERT_EQ(string("test_tree/enron_email/613."), res[1].documentName);
  ASSERT_EQ(2, res[1].rank);

  // Try multi word query
  query.clear();
  query.push_back("whale");
  query.push_back("ocean");
  query.push_back("ravenous");

  res = qp.ProcessQuery(query);
  ASSERT_EQ(3U, res.size());
  ASSERT_EQ(string("test_tree/books/mobydick.txt"), res[0].documentName);
  ASSERT_EQ(1314, res[0].rank);
  ASSERT_EQ(string("test_tree/books/leavesofgrass.txt"),
            res[1].documentName);
  ASSERT_EQ(42, res[1].rank);
  ASSERT_EQ(string("test_tree/books/ulysses.txt"), res[2].documentName);
  ASSERT_EQ(15, res[2].rank);

  query.push_back("huckleberry");
  res = qp.ProcessQuery(query);
  ASSERT_EQ(0U, res.size());

  // Done!
  HW3Environment::AddPoints(50);
}

}  // namespace hw3
