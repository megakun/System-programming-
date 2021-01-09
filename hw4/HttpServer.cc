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

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include "./FileReader.h"
#include "./HttpConnection.h"
#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpServer.h"
#include "./libhw3/QueryProcessor.h"

using std::cerr;
using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::stringstream;
using std::unique_ptr;

namespace hw4 {
///////////////////////////////////////////////////////////////////////////////
// Constants, internal helper functions
///////////////////////////////////////////////////////////////////////////////
static const char *kThreegleStr =
  "<html><head><title>333gle</title></head>\n"
  "<body>\n"
  "<center style=\"font-size:500%;\">\n"
  "<span style=\"position:relative;bottom:-0.33em;color:orange;\">3</span>"
    "<span style=\"color:red;\">3</span>"
    "<span style=\"color:gold;\">3</span>"
    "<span style=\"color:blue;\">g</span>"
    "<span style=\"color:green;\">l</span>"
    "<span style=\"color:red;\">e</span>\n"
  "</center>\n"
  "<p>\n"
  "<div style=\"height:20px;\"></div>\n"
  "<center>\n"
  "<form action=\"/query\" method=\"get\">\n"
  "<input type=\"text\" size=30 name=\"terms\" />\n"
  "<input type=\"submit\" value=\"Search\" />\n"
  "</form>\n"
  "</center><p>\n";

// static
const int HttpServer::kNumThreads = 100;

// This is the function that threads are dispatched into
// in order to process new client connections.
static void HttpServer_ThrFn(ThreadPool::Task *t);

// Given a request, produce a response.
static HttpResponse ProcessRequest(const HttpRequest &req,
                            const string &basedir,
                            const list<string> *indices);

// Process a file request.
static HttpResponse ProcessFileRequest(const string &uri,
                                const string &basedir);

// Process a query request.
static HttpResponse ProcessQueryRequest(const string &uri,
                                 const list<string> *indices);


///////////////////////////////////////////////////////////////////////////////
// HttpServer
///////////////////////////////////////////////////////////////////////////////
bool HttpServer::Run(void) {
  // Create the server listening socket.
  int listen_fd;
  cout << "  creating and binding the listening socket..." << endl;
  if (!ss_.BindAndListen(AF_INET6, &listen_fd)) {
    cerr << endl << "Couldn't bind to the listening socket." << endl;
    return false;
  }

  // Spin, accepting connections and dispatching them.  Use a
  // threadpool to dispatch connections into their own thread.
  cout << "  accepting connections..." << endl << endl;
  ThreadPool tp(kNumThreads);
  while (1) {
    HttpServerTask *hst = new HttpServerTask(HttpServer_ThrFn);
    hst->basedir = staticfileDirpath_;
    hst->indices = &indices_;
    if (!ss_.Accept(&hst->client_fd,
                    &hst->caddr,
                    &hst->cport,
                    &hst->cdns,
                    &hst->saddr,
                    &hst->sdns)) {
      // The accept failed for some reason, so quit out of the server.
      // (Will happen when kill command is used to shut down the server.)
      break;
    }
    // The accept succeeded; dispatch it.
    tp.Dispatch(hst);
  }
  return true;
}

static void HttpServer_ThrFn(ThreadPool::Task *t) {
  // Cast back our HttpServerTask structure with all of our new
  // client's information in it.
  unique_ptr<HttpServerTask> hst(static_cast<HttpServerTask *>(t));
  cout << "  client " << hst->cdns << ":" << hst->cport << " "
       << "(IP address " << hst->caddr << ")" << " connected." << endl;

  // Read in the next request, process it, write the response.

  // Use the HttpConnection class to read and process the next
  // request from our current client, then write out our response.  If
  // the client sends a "Connection: close\r\n" header, then shut down
  // the connection -- we're done.
  //
  // Hint: the client can make multiple requests on our single connection,
  // so we should keep the connection open between requests rather than
  // creating/destroying the same connection repeatedly.

  // STEP 1:
  bool done = false;
  HttpConnection hc(hst->client_fd);
  while (!done) {
     HttpRequest request;
    if (!hc.GetNextRequest(&request)) {  //  get the next request from client
      close(hst->client_fd);
      done = true;
      break;
    }

    // shut down the connection if requested by client
    if (request.GetHeaderValue("connection") == "close") {
      close(hst->client_fd);
      done = true;
      break;
    }

    HttpResponse response = ProcessRequest(request,
                                              hst->basedir, hst->indices);
    // fail to repsond
    if (!hc.WriteResponse(response)) {
      close(hst->client_fd);
      done = true;
      break;
    }
  }
}

static HttpResponse ProcessRequest(const HttpRequest &req,
                            const string &basedir,
                            const list<string> *indices) {
  // Is the user asking for a static file?
  if (req.uri().substr(0, 8) == "/static/") {
    return ProcessFileRequest(req.uri(), basedir);
  }

  // The user must be asking for a query.
  return ProcessQueryRequest(req.uri(), indices);
}

static HttpResponse ProcessFileRequest(const string &uri,
                                const string &basedir) {
  // The response we'll buildparser.
  HttpResponse ret;

  // Steps to follow:
  //  - use the URLParser class to figure out what filename
  //    the user is asking for.
  //
  //  - use the FileReader class to read the file into memory
  //
  //  - copy the file content into the ret.body
  //
  //  - depending on the file name suffix, set the response
  //    Content-type header as appropriate, e.g.,:
  //      --> for ".html" or ".htm", set to "text/html"
  //      --> for ".jpeg" or ".jpg", set to "image/jpeg"
  //      --> for ".png", set to "image/png"
  //      etc.
  //
  // be sure to set the response code, protocol, and message
  // in the HttpResponse as well.
  string fname = "";

  // STEP 2:
  // parse and config file
  URLParser parser;
  parser.Parse(uri);

  fname = parser.path().substr(8);
  FileReader fr(basedir, fname);
  string str;

  // read file
  if (fr.ReadFile(&str)) {
    ret.AppendToBody(str);  // copy to body

    int pos = fname.rfind(".");
    string suffix = fname.substr(pos);
    // set content
    if (suffix == ".html" || suffix == ".htm") {
      ret.set_content_type("text/html");
    } else if (suffix == ".csv") {
      ret.set_content_type("text/csv");
    } else if (suffix == ".css") {
      ret.set_content_type("text/css");
    } else if (suffix == ".js") {
      ret.set_content_type("text/javascript");
    } else if (suffix == ".txt" || suffix == ".") {
      ret.set_content_type("text/plain");
    } else if (suffix == ".xml") {
      ret.set_content_type("text/xml");
    } else if (suffix == ".jpg" || suffix == ".jpeg") {
      ret.set_content_type("image/jpeg");
    } else if (suffix == ".gif") {
      ret.set_content_type("image/gif");
    } else if (suffix == ".png") {
      ret.set_content_type("image/png");
    } else if (suffix == ".tiff") {
      ret.set_content_type("image/tiff");
    } else {
      ret.set_content_type("application/octet-stream");
    }

    ret.set_protocol("HTTP/1.1");
    ret.set_response_code(200);
    ret.set_message("OK");
    return ret;
  }

  // If you couldn't find the file, return an HTTP 404 error.
  ret.set_protocol("HTTP/1.1");
  ret.set_response_code(404);
  ret.set_message("Not Found");
  ret.AppendToBody("<html><body>Couldn't find file \""
                   + EscapeHtml(fname)
                   + "\"</body></html>");
  return ret;
}

static HttpResponse ProcessQueryRequest(const string &uri,
                                 const list<string> *indices) {
  // The response we're buildingparser.
  HttpResponse ret;

  // Your job here is to figure out how to present the user with
  // the same query interface as our solution_binaries/http333d server.
  // A cparserle of notes:
  //
  //  - no matter what, you need to present the 333gle logo and the
  //    search box/button
  //
  //  - if the user had previously typed in a search query, you also
  //    need to display the search results.
  //
  //  - you'll want to use the URLParser to parse the uri and extract
  //    search terms from a typed-in search query.  convert them
  //    to lower case.
  //
  //  - you'll want to create and use a hw3::QueryProcessor to process
  //    the query against the search indices
  //
  //  - in your generated search results, see if you can figure out
  //    how to hyperlink results to the file contents, like we did
  //    in our solution_binaries/http333d.

  // STEP 3:
  // body of frot page
  ret.AppendToBody(kThreegleStr);

  // display search result
  if (uri.find("query?terms=") != string::npos) {
    // parse query from url
    URLParser parser;
    parser.Parse(uri);
    string query = parser.args()["terms"];
    boost::trim(query);
    boost::to_lower(query);

    vector<string> words;
    boost::split(words, query, boost::is_any_of(" "),
                 boost::token_compress_on);

    hw3::QueryProcessor qp(*indices, false);
    vector<hw3::QueryProcessor::QueryResult> result = qp.ProcessQuery(words);

    // if no match
    if (result.size() == 0) {
      ret.AppendToBody("<p><br>\r\n");
      ret.AppendToBody("No results found for <b>");
      ret.AppendToBody(EscapeHtml(query));
      ret.AppendToBody("</b>\r\n");
      ret.AppendToBody("<p>\r\n\r\n");
    } else {
      ret.AppendToBody("<p><br>\r\n");
      ret.AppendToBody(std::to_string(result.size()));
      ret.AppendToBody(" result");

      if (result.size() > 1) {
        ret.AppendToBody("s");
        ret.AppendToBody(" found for <b>");
        ret.AppendToBody(EscapeHtml(query));
        ret.AppendToBody("</b>\r\n");
        ret.AppendToBody("<p>\r\n\r\n");

        // display the result
        ret.AppendToBody("<ul>\r\n");
        for (size_t i = 0; i < result.size(); i++) {
          ret.AppendToBody(" <li> <a href=\"");
          // if httplink found
          if (result[i].documentName.substr(0, 7) != "http://") {
            ret.AppendToBody("/static/");
          }
          ret.AppendToBody(result[i].documentName);
          ret.AppendToBody("\">");
          ret.AppendToBody(EscapeHtml(result[i].documentName));
          ret.AppendToBody("</a> [");
          ret.AppendToBody(std::to_string(result[i].rank));
          ret.AppendToBody("]<br>\r\n");
        }
        ret.AppendToBody("</ul>\r\n");
      }
    }
    // end of the page, which is the same for all result
    ret.AppendToBody("</body>\r\n");
    ret.AppendToBody("</html>\r\n");
  }

  ret.set_protocol("HTTP/1.1");
  ret.set_response_code(200);
  ret.set_message("OK");
  return ret;
}

}  // namespace hw4
