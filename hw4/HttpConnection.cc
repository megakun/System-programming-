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

#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

using std::map;
using std::string;
using std::vector;

namespace hw4 {

static const char *kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;

bool HttpConnection::GetNextRequest(HttpRequest *request) {
  // Use "WrappedRead" to read data into the buffer_
  // instance variable.  Keep reading data until either the
  // connection drops or you see a "\r\n\r\n" that demarcates
  // the end of the request header. Be sure to try and read in
  // a large amount of bytes each time you call WrappedRead.
  //
  // Once you've seen the request header, use ParseRequest()
  // to parse the header into the *request argument.
  //
  // Very tricky part:  clients can send back-to-back requests
  // on the same socket.  So, you need to preserve everything
  // after the "\r\n\r\n" in buffer_ for the next time the
  // caller invokes GetNextRequest()!

  // STEP 1:
  if (buffer_.find(kHeaderEnd, kHeaderEndLen) == string::npos) {
    while (1) {
      char buf[1024];
      int bytes = WrappedRead(fd_, reinterpret_cast<unsigned char *>(buf),
                            1024);
      if (bytes == 0) {  // EOF
        break;
      } else if (bytes == -1) {  // error
        return false;
      } else {
        buffer_ += string(buf, bytes);
        // found end of header
        if (buffer_.find(kHeaderEnd, kHeaderEndLen) != string::npos) {
          break;
        }
      continue;
      }
    }
  }

  size_t pos = buffer_.find(kHeaderEnd, kHeaderEndLen);
  if (pos == string::npos) {
    return false;
  }
  string requestStr = buffer_.substr(0, pos);
  *request = ParseRequest(requestStr);
  // preseve buffer for nextrequest
  buffer_ = buffer_.substr(pos + kHeaderEndLen);

  return true;  // You may want to change this.
}

bool HttpConnection::WriteResponse(const HttpResponse &response) {
  string str = response.GenerateResponseString();
  int res = WrappedWrite(fd_,
                         (unsigned char *) str.c_str(),
                         str.length());
  if (res != static_cast<int>(str.length()))
    return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(const string &request) {
  HttpRequest req("/");  // by default, get "/".

  // Split the request into lines.  Extract the URI from the first line
  // and store it in req.URI.  For each additional line beyond the
  // first, extract out the header name and value and store them in
  // req.headers_ (i.e., HttpRequest::AddHeader).  You should look
  // at HttpRequest.h for details about the HTTP header format that
  // you need to parse.
  //
  // You'll probably want to look up boost functions for (a) splitting
  // a string into lines on a "\r\n" delimiter, (b) trimming
  // whitespace from the end of a string, and (c) converting a string
  // to lowercase.
  //
  // Note that you may assume the request you are parsing is correctly
  // formatted. If for some reason you encounter a header that is
  // malformed, you may skip that line.

  // STEP 2:
  // spliting header
  vector<string> lines;
  boost::split(lines, request, boost::is_any_of("\r\n"),
                     boost::token_compress_on);

  // if is not a valid request
  if (lines.size() < 2) {
    req.set_uri("BAD_");
    return req;
  }
    // deal with the first line
    boost::trim(lines[0]);
    vector<string> first;
    boost::split(first, lines[0], boost::is_any_of(" "),
                       boost::token_compress_on);
    req.set_uri(first[1]);
    lines.erase(lines.begin());

    // handle the rest of the request
    for (auto line : lines) {
      boost::trim(line);
      vector<string> header;
      boost::split(header, line, boost::is_any_of(": "),
                         boost::token_compress_on);

      string headerStr = header[0];
      boost::to_lower(headerStr);
      req.AddHeader(headerStr, header[1]);
    }
  return req;
}

}  // namespace hw4
