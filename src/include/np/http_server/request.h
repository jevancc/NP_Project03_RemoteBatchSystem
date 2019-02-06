#ifndef _NP_HTTP_REQUEST_H_
#define _NP_HTTP_REQUEST_H_

#include <map>
#include <string>
using namespace std;

namespace np {
namespace http {

class Request {
 public:
  int http_version_major;
  int http_version_minor;
  string method;
  string uri;
  string short_uri;
  string query_string;
  map<string, string> headers;
};

}  // namespace http
}  // namespace np
#endif
