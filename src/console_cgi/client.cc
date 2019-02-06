#include <boost/asio.hpp>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <memory>
#include <vector>
#include "fmt/core.h"
#include "np/cgi/console/cgi.h"
#include "np/cgi/console/client.h"
using namespace std;
using namespace boost::asio;

namespace np {
namespace cgi {
namespace console {

Client::Client(boost::asio::io_service& io_service, int id, ClientOption& opt)
    : terminal_(id, opt),
      host_(opt.host),
      port_(opt.port),
      file_(opt.file),
      current_command_index_(0),
      resolver_(io_service),
      socket_(io_service) {}

void Client::Run() { LoadCommands_(); }

void Client::LoadCommands_() {
  boost::system::error_code ec;
  auto path = boost::filesystem::path("test_case") / file_;
  if (!boost::filesystem::is_regular_file(path, ec) || ec) {
    terminal_.DisplayError(fmt::format(
        "is_regular_file(file): An error occurred:\n{}\n", ec.message()));
  } else {
    string line;
    boost::filesystem::ifstream f(path);
    while (getline(f, line)) {
      terminal_.AddCommand(line);
      commands_.push_back(line);
    }
    f.close();
    ResolveHost_();
  }
}

void Client::ResolveHost_() {
  ip::tcp::resolver::query query(host_, port_);
  resolver_.async_resolve(query, [this](const boost::system::error_code& ec,
                                        ip::tcp::resolver::iterator it) {
    if (!ec) {
      ConnectHost_(it);
    } else {
      terminal_.DisplayError(fmt::format(
          "resolver.async_resolve(): An error occurred:\n{}\n", ec.message()));
    }
  });
}

void Client::ConnectHost_(ip::tcp::resolver::iterator it) {
  socket_.async_connect(
      it->endpoint(), [this](const boost::system::error_code& ec) {
        if (!ec) {
          DoRead_();
        } else {
          terminal_.DisplayError(
              fmt::format("socket.async_connect(): An error occurred:\n{}\n",
                          ec.message()));
        }
      });
}

void Client::DoRead_() {
  socket_.async_read_some(
      buffer(buffer_, buffer_.size()),
      [this](const boost::system::error_code& ec, size_t bytes_transferred) {
        if (!ec) {
          if (buffer_[0] == '%' && buffer_[1] == ' ') {
            DoWrite_();
          } else {
            terminal_.DisplayOutput(
                string(buffer_.begin(), buffer_.begin() + bytes_transferred));
            DoRead_();
          }
        } else if (ec != boost::asio::error::eof) {
          terminal_.DisplayError(
              fmt::format("socket.async_read_some(): An error occurred:\n{}\n",
                          ec.message()));
        }
      });
}

void Client::DoWrite_() {
  auto& command = commands_[current_command_index_++];

  if (command.empty()) {
    terminal_.GotoNextCommand();
  } else {
    socket_.async_send(
        buffer(command + "\n", command.size() + 1),
        [this](boost::system::error_code ec, size_t bytes_transferred) {
          if (!ec) {
            terminal_.GotoNextCommand();
            DoRead_();
          }
        });
  }
}

}  // namespace console
}  // namespace cgi
}  // namespace np
