//
// RequestParser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _NP_HTTP_REQUEST_PARSER_H_
#define _NP_HTTP_REQUEST_PARSER_H_

#include <iostream>
#include <tuple>
#include "np/http_server/request.h"
using namespace std;

namespace np {
namespace http {

/// Parser for incoming requests.
class RequestParser {
 public:
  /// Construct ready to parse the request method.
  RequestParser();

  /// Reset to initial parser state.
  void Reset();

  /// Result of parse.
  enum ResultType { kBad = 0, kHeaderIndeterminate, kHeaderReady };

  ResultType GetStatus();

  /// Parse some data. The enum return value is good when a complete request has
  /// been parsed, bad if the data is invalid, indeterminate when more data is
  /// required. The InputIterator return value indicates how much of the input
  /// has been consumed.
  template <typename InputIterator>
  InputIterator Parse(Request& req, InputIterator begin, InputIterator end) {
    while (begin != end && status_ == kHeaderIndeterminate) {
      Consume_(req, *begin++);
    }
    return begin;
  }

 private:
  /// Handle the next character of input.
  ResultType Consume_(Request& req, char input);

  /// Check if a byte is an HTTP character.
  static bool IsChar_(int c);

  /// Check if a byte is an HTTP control character.
  static bool IsCtl_(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool IsTSpecial_(int c);

  /// Check if a byte is a digit.
  static bool IsDigit_(int c);

  /// The current state of the parser.
  enum state {
    method_start,
    method,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3,
  } state_;

  ResultType status_;

  string last_header_name_;
  string last_header_value_;
};

}  // namespace http
}  // namespace np

#endif
