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

// This file contains a number of HTTP and HTML parsing routines
// that come in useful throughput the assignment.

#ifndef HW4_HTTPUTILS_H_
#define HW4_HTTPUTILS_H_

#include <stdint.h>

#include <string>
#include <utility>
#include <map>

namespace hw4 {

// This function tests whether the file "testfile" is contained below
// the root directory "rootdir".  If so, i.e., if the path is safe,
// the function returns true.  Otherwise, returns false.
//
// So, for example, the following combination is safe:
//
//   rootdir:  "test_files"
//   testfile: "test_files/ok/bar"
//
// but the following is not:
//
//   rootdir:  "test_files"
//   testfile: "libhw1/libhw1.a"
//
// nor is:
//
//   rootdir:  "test_files"
//   testfile: "test_files/../libhw1/libhw1.a"
//
bool IsPathSafe(const std::string &rootdir, const std::string &testfile);

// This function performs HTML escaping in place.  It scans a string
// for dangerous HTML tokens (such as "<") and replaces them with the
// escaped HTML equivalent (such as "&lt;").  This helps to prevent
// XSS attacks.
std::string EscapeHtml(const std::string &from);

// This function performs URI decoding.  It scans a string for
// the "%" escape character and converts the token to the
// appropriate ASCII character.  See the wikipedia article on
// URL encoding for an explanation of what's going on here:
//
//    http://en.wikipedia.org/wiki/Percent-encoding
//
std::string URIDecode(const std::string &from);

// A URL that's part of a web request has the following structure:
//
//   /foo/bar/baz?field=value&field2=value2
//
//      path     ?   args
//
// This class accepts a URL and splits it into these components and
// URIDecode()'s them, allowing the caller to access the components
// through convenient methods.
class URLParser {
 public:
  URLParser() { }
  virtual ~URLParser() { }

  void Parse(const std::string &url);

  // Return the "path" component of the url, post-uri-decoding.
  std::string path() const { return path_; }

  // Return the "args" component of the url post-uri-decoding.
  // The args component is parsed into a map from field to value.
  std::map<std::string, std::string> args() const { return args_; }

 private:
  std::string url_;
  std::string path_;
  std::map<std::string, std::string> args_;
};

// Return a randomly generated port number between 10000 and 40000.
uint16_t GetRandPort();

// A wrapper around "read" that shields the caller from dealing
// with the ugly issues of partial reads, EINTR, EAGAIN, and so
// on.
//
// Reads at most "readlen" bytes from the file descriptor fd
// into the buffer "buf".  Returns the number of bytes actually
// read.  On fatal error, returns -1.  If EOF is hit and no
// bytes have been read, returns 0.  Might read fewer bytes
// than requested.
int WrappedRead(int fd, unsigned char *buf, int readlen);

// A wrapper around "write" that shields the caller from dealing
// with the ugly issues of partial writes, EINTR, EAGAIN, and so
// on.
//
// Writes "writelen" bytes to the file descriptor fd from
// the buffer "buf".  Blocks the caller until either writelen
// bytes have been written, or an error is encountered.  Returns
// the total number of bytes written; if this number is less
// than writelen, it's because some fatal error was encountered,
// like the connection being dropped.
int WrappedWrite(int fd, unsigned char *buf, int writelen);

// A convenience routine to manufacture a (blocking) socket to the
// hostname and port number provided as arguments.  Hostname can
// be a DNS name or an IP address, in string form.  On success,
// returns a file descriptor thorugh "client_fd" and returns true.
// On failure, returns false.  Caller is responsible for close()'ing
// the file descriptor.
bool ConnectToServer(const std::string &hostname, uint16_t portnum,
                    int *client_fd);

// As we learned in class, std::unique_ptr is useful for wrapping memory
// that is dynamically allocated from the heap using "new".  If, however, you
// end up using hw2 functions (like ReadFileToString from hw2/FileParser.h),
// you might be faced with a situation where memory is dynamically allocated
// using malloc, not new.  In this case, you need to teach the unique_ptr
// to use free() instead of delete.
//
// The way you do this is by specifying a "deleter" when you construct
// the unique pointer.  We provide a malloc/free deleter below.  So, to
// use it, you would do something like this:
//
//  std::unique_ptr<char,MallocDeleter<char>> str(malloc(sizeof(char) * len));
//
// Then, when str falls out of scope, its memory will automatically free()'d.
template<typename T>
class MallocDeleter {
 public:
  void operator()(T* ptr) {
    free(ptr);
  }
};


}  // namespace hw4

#endif  // HW4_HTTPUTILS_H_
