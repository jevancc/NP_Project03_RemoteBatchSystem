#include <array>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include "np/http_server/server.h"
using namespace std;

int main(int argc, char* const argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " [port]" << endl;
    return 1;
  }

  try {
    short port = atoi(argv[1]);
    boost::asio::io_service io_service;
    np::http::Server server(io_service, port);
    cout << "Server is listening on port " << port << endl;
    server.Run();
  } catch (exception& e) {
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
