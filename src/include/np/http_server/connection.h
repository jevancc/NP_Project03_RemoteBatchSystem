#ifndef _NP_HTTP_CONNECTION_H_
#define _NP_HTTP_CONNECTION_H_
#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <memory>
#include "np/http_server/connection_manager.h"
#include "np/http_server/request.h"
#include "np/http_server/request_parser.h"
using namespace std;

namespace np {
namespace http {

class ConnectionManager;
class Connection : public enable_shared_from_this<Connection> {
 private:
  boost::asio::ip::tcp::socket socket_;
  boost::array<char, 4096> buffer_;
  Request request_;
  RequestParser request_parser_;
  ConnectionManager& connection_manager_;
  boost::asio::io_service& io_service_;
  void DoRead_();
  void HandleRequest_(bool is_good_request = true);

 public:
  Connection(boost::asio::ip::tcp::socket socket,
             ConnectionManager& connection_manager,
             boost::asio::io_service& io_service)
      : socket_(move(socket)),
        connection_manager_(connection_manager),
        io_service_(io_service){};

  void Stop() { socket_.close(); }
  void Start() { DoRead_(); };
};
}  // namespace http
}  // namespace np

#endif
