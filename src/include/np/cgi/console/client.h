#ifndef _NP_CGI_CONSOLE_CLIENT_H_
#define _NP_CGI_CONSOLE_CLIENT_H_

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>
#include "np/cgi/console/terminal.h"
using namespace std;
using namespace boost::asio;

namespace np {
namespace cgi {
namespace console {

struct ClientOption {
  string host;
  string port;
  string file;
};

class WebTerminal;
class Client : enable_shared_from_this<Client> {
 private:
  WebTerminal terminal_;
  string host_;
  string port_;
  string file_;

  int current_command_index_;
  vector<string> commands_;

  ip::tcp::resolver resolver_;
  ip::tcp::socket socket_;
  boost::array<char, 4096> buffer_;

  void LoadCommands_();
  void ResolveHost_();
  void ConnectHost_(ip::tcp::resolver::iterator it);

  void DoRead_();
  void DoWrite_();

 public:
  Client(boost::asio::io_service& io_service, int id, ClientOption& opt);
  void Run();
};

}  // namespace console
}  // namespace cgi
}  // namespace np

#endif
