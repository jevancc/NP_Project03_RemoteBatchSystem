#ifndef _NP_HTTP_SERVER_H_
#define _NP_HTTP_SERVER_H_

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <array>
#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include "np/http_server/connection.h"
#include "np/http_server/connection_manager.h"
using namespace std;

namespace np {
namespace http {

class Server {
 private:
  boost::asio::io_service& io_service_;
  boost::asio::signal_set signal_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  ConnectionManager connection_manager_;

  void DoSignalWait_();
  void DoAccept_();

 public:
  Server(boost::asio::io_service& io_service, unsigned short port);
  void Run();
};

}  // namespace http
}  // namespace np

#endif
