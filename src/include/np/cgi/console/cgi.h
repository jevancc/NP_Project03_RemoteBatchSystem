#ifndef _NP_CGI_CONSOLE_CGI_H_
#define _NP_CGI_CONSOLE_CGI_H_

#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

namespace np {
namespace cgi {
namespace console {
struct ClientOption;

vector<ClientOption> ParseQueryString(string s);

template <class T>
void WriteMsg(const T& v) {
  const size_t kSendBufferSize = 512;
  for (size_t i = 0; i < v.size();) {
    size_t t =
        send(STDIN_FILENO, &v[0] + i, min(kSendBufferSize, v.size() - i), 0);
    if (t != size_t(-1)) {
      i += t;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // keep
    } else if (errno == EINTR) {
      // keep
    } else {
      break;
    }
  }
}

}  // namespace console
}  // namespace cgi
}  // namespace np

#endif
