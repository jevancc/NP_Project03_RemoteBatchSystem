#ifndef _NP_CGI_CONSOLE_TERMINAL_H_
#define _NP_CGI_CONSOLE_TERMINAL_H_

#include <string>
#include <vector>
using namespace std;

namespace np {
namespace cgi {
namespace console {

struct ClientOption;
class WebTerminal {
 private:
  int id_;

 public:
  WebTerminal(int id, ClientOption& opt);

  void AddCommand(const string& s);
  void GotoNextCommand();
  void DisplayOutput(const string& s);
  void DisplayError(const string& s);
};

}  // namespace console
}  // namespace cgi
}  // namespace np

#endif
