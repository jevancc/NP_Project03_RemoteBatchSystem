#include <unistd.h>
#include <array>
#include <boost/asio.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include "fmt/core.h"
#include "np/cgi/console/cgi.h"
#include "np/cgi/console/client.h"
using namespace std;
using namespace boost::asio;
using namespace np::cgi::console;

#include "./static_html.hhpp"

int main() {
  WriteMsg(
      string("Content-type: text/html\r\n"
             "Cache-Control: no-store, must-revalidate\r\n"
             "Pragma: no-cache\r\n"
             "Expires: 0\r\n"
             "\r\n"));
  WriteMsg(CONSOLE_CGI_HTML);

  boost::asio::io_service io_service;
  set<shared_ptr<Client>> clients;
  auto v = np::cgi::console::ParseQueryString(getenv("QUERY_STRING"));
  for (int i = 0; i < int(v.size()); i++) {
    auto& opt = v[i];
    if (!opt.host.empty() && !opt.port.empty() && !opt.file.empty()) {
      auto ptr = make_shared<Client>(io_service, i, opt);
      clients.insert(ptr);
      ptr->Run();
    }
  }
  io_service.run();
}
