#include "np/http_server/connection_manager.h"
#include "np/http_server/connection.h"
using namespace std;

namespace np {
namespace http {

void ConnectionManager::Start(shared_ptr<Connection> c) {
  connections_.insert(c);
  c->Start();
}

void ConnectionManager::Stop(shared_ptr<Connection> c) {
  connections_.erase(c);
  c->Stop();
}

void ConnectionManager::StopAll() {
  for (auto& c : connections_) {
    c->Stop();
  }
  connections_.clear();
}

}  // namespace http
}  // namespace np
