#ifndef _NP_HTTP_CONNECTION_MANAGER_H_
#define _NP_HTTP_CONNECTION_MANAGER_H_

#include <memory>
#include <set>
#include "np/http_server/connection.h"
using namespace std;

namespace np {
namespace http {

class Connection;
class ConnectionManager {
 private:
  set<shared_ptr<Connection>> connections_;

 public:
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;

  ConnectionManager() {}

  void Start(shared_ptr<Connection> c);
  void Stop(shared_ptr<Connection> c);
  void StopAll();
};

}  // namespace http
}  // namespace np

#endif
