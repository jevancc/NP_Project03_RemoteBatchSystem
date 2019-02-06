#include "np/cgi/console/cgi.h"
#include <algorithm>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "np/cgi/console/client.h"
using namespace std;

namespace np {
namespace cgi {
namespace console {

vector<ClientOption> ParseQueryString(string s) {
  enum { kKeyPrefix, kKeyID, kValue, kSeperator } state = kKeyPrefix;
  map<int, ClientOption> options;

  char pfx;
  int id = 0;
  string value;
  s.push_back('&');
  for (int i = 0; i < int(s.size()); i++) {
    char c = s[i];
    switch (state) {
      case kKeyPrefix:
        if (c == 'p' || c == 'h' || c == 'f') {
          pfx = c;
          id = 0;
          state = kKeyID;
        } else {
          i--;
          state = kSeperator;
        }
        break;
      case kKeyID:
        if (c == '=') {
          value = "";
          state = kValue;
        } else if (c >= '0' && c <= '9') {
          id = id * 10 + c - '0';
        } else {
          i--;
          state = kSeperator;
        }
        break;
      case kValue:
        if (c == '&') {
          if (pfx == 'p') {
            options[id].port = value;
          } else if (pfx == 'h') {
            options[id].host = value;
          } else if (pfx == 'f') {
            options[id].file = value;
          }
          state = kKeyPrefix;
        } else {
          value.push_back(c);
        }
        break;
      case kSeperator:
        if (c == '&') {
          state = kKeyPrefix;
        }
    }
  }

  vector<ClientOption> ret;
  for (auto& it : options) {
    ret.push_back(move(it.second));
  }
  return ret;
}

}  // namespace console
}  // namespace cgi
}  // namespace np
