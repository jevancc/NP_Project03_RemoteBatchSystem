#define BOOST_FILESYSTEM_VERSION 3
#include "np/http_server/connection.h"
#include <fmt/core.h>
#include <unistd.h>
#include <cstring>
#include <tuple>
#include <utility>
#include <vector>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
using namespace std;
using namespace boost;

namespace np {
namespace http {

void Connection::DoRead_() {
  auto self(shared_from_this());
  socket_.async_read_some(
      asio::buffer(buffer_, buffer_.size()),
      [this, self](system::error_code ec, size_t bytes_transferred) {
        if (!ec) {
          request_parser_.Parse(request_, buffer_.data(),
                                buffer_.data() + bytes_transferred);

          if (request_parser_.GetStatus() == RequestParser::kHeaderReady) {
            HandleRequest_();
          } else if (request_parser_.GetStatus() == RequestParser::kBad) {
            HandleRequest_(false);
          } else {
            DoRead_();
          }
        } else if (ec != asio::error::operation_aborted) {
          connection_manager_.Stop(shared_from_this());
        }
      });
}

void Connection::HandleRequest_(bool is_good_request) {
  auto self(shared_from_this());

  bool is_ok = false;
  string reply_msg;
  boost::filesystem::path execfile;
  if (is_good_request) {
    execfile = boost::filesystem::current_path() / request_.short_uri;
    if (filesystem::is_regular_file(execfile)) {
      is_ok = true;
      reply_msg +=
          "HTTP/1.1 200 OK\r\n"
          "Connection: close\r\n";
    } else {
      reply_msg +=
          "HTTP/1.1 404 Not Found\r\n"
          "Connection: close\r\n"
          "Content-Length: 20\r\n"
          "\r\n"
          "<h1>Not Found</h1>\r\n";
    }
  } else {
    reply_msg +=
        "HTTP/1.0 400 Bad Request\r\n"
        "Connection: close\r\n"
        "\r\n";
  }

  socket_.async_send(
      asio::buffer(reply_msg, reply_msg.size()),
      [this, self, is_ok, execfile](system::error_code ec,
                                    size_t bytes_transferred) {
        if (!ec && is_ok) {
          setenv("REQUEST_METHOD", request_.method.c_str(), 1);
          setenv("REQUEST_URI", request_.short_uri.c_str(), 1);
          setenv("QUERY_STRING", request_.query_string.c_str(), 1);
          setenv("SERVER_PROTOCOL",
                 fmt::format("HTTP{}.{}", request_.http_version_major,
                             request_.http_version_minor)
                     .c_str(),
                 1);
          setenv("HTTP_HOST", request_.headers["Host"].c_str(), 1);
          setenv("SERVER_ADDR",
                 socket_.local_endpoint().address().to_string().c_str(), 1);
          setenv("SERVER_PORT",
                 fmt::format("{}", socket_.local_endpoint().port()).c_str(), 1);
          setenv("REMOTE_ADDR",
                 socket_.remote_endpoint().address().to_string().c_str(), 1);
          setenv("REMOTE_PORT",
                 fmt::format("{}", socket_.remote_endpoint().port()).c_str(),
                 1);

          io_service_.notify_fork(asio::io_service::fork_prepare);
          if (fork() != 0) {
            io_service_.notify_fork(asio::io_service::fork_parent);
          } else {
            io_service_.notify_fork(asio::io_service::fork_child);
            int sockfd = socket_.native_handle();
            dup2(sockfd, 0);
            dup2(sockfd, 1);
            dup2(sockfd, 2);
            this->connection_manager_.StopAll();

            if (execlp(execfile.c_str(), execfile.c_str(), NULL) < 0) {
              cout << "Content-type:text/html\r\n"
                      "\r\n"
                      "<h1>Execution failed</h1>\r\n";
            }

            system::error_code ignored_ec;
            socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
          }
        }

        if (ec != asio::error::operation_aborted) {
          connection_manager_.Stop(self);
        }
      });
}

}  // namespace http
}  // namespace np
