#include "np/cgi/console/terminal.h"
#include <cstdlib>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "fmt/core.h"
#include "np/cgi/console/cgi.h"
#include "np/cgi/console/client.h"
using namespace std;
using json = nlohmann::json;

namespace np {
namespace cgi {
namespace console {

WebTerminal::WebTerminal(int id, ClientOption& opt) : id_(id) {
  WriteMsg(fmt::format(
      "<script>"
      "ct({0},{1},{2});"
      "</script>\r\n",
      json(opt.host).dump(), json(opt.port).dump(), json(opt.file).dump()));
}

void WebTerminal::AddCommand(const string& s) {
  WriteMsg(
      fmt::format("<script>"
                  "ac({0},{1});"
                  "</script>\r\n",
                  id_, json(s).dump()));
}
void WebTerminal::GotoNextCommand() {
  WriteMsg(
      fmt::format("<script>"
                  "gnc({0});"
                  "</script>\r\n",
                  id_));
}

void WebTerminal::DisplayOutput(const string& s) {
  WriteMsg(
      fmt::format("<script>"
                  "dop({0},{1});"
                  "</script>\r\n",
                  id_, json(s).dump()));
}

void WebTerminal::DisplayError(const string& s) {
  WriteMsg(
      fmt::format("<script>"
                  "derr({0},{1});"
                  "</script>\r\n",
                  id_, json(s).dump()));
}

}  // namespace console
}  // namespace cgi
}  // namespace np
