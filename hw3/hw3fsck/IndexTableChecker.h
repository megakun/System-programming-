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

#ifndef HW3_HW3FSCK_INDEXTABLECHECKER_H_
#define HW3_HW3FSCK_INDEXTABLECHECKER_H_

#include <stdint.h>  // for uint32_t, etc.
#include <cstdio>    // for (FILE *)

#include "../Utils.h"
#include "./HashTableChecker.h"

namespace hw3 {

// An IndexTableChecker is a subclass of HashTableChecker used to
// read the word-->docID_table "index" within the index file.
class IndexTableChecker : public HashTableChecker {
 public:
  // Construct an IndexTableChecker.  Arguments:
  //
  // - f: an open (FILE *) for the underlying index file.  The new
  //   object takes ownership of the (FILE *) and will fclose() it
  //   on destruction.
  //
  // - offset: the file offset of the first byte of the doctable
  IndexTableChecker(FILE *f, int32_t offset, int32_t len);

  // Check an IndexTableElement.
  // Returns true if the element seems correct, false otherwise.
  virtual bool CheckElement(int32_t elementOffset,
                            int32_t bucketNumber);

 private:
  DISALLOW_COPY_AND_ASSIGN(IndexTableChecker);
};

}  // namespace hw3

#endif  // HW3_HW3FSCK_INDEXTABLECHECKER_H_
