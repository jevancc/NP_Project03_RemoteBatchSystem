//
// request_parser.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "np/http_server/request_parser.h"
#include "np/http_server/request.h"

namespace np {
namespace http {

RequestParser::RequestParser()
    : state_(method_start), status_(kHeaderIndeterminate) {}

void RequestParser::Reset() {
  state_ = method_start;
  status_ = kHeaderIndeterminate;
}

RequestParser::ResultType RequestParser::GetStatus() { return status_; }

RequestParser::ResultType RequestParser::Consume_(Request& req, char input) {
  switch (state_) {
    case method_start:
      if (!IsChar_(input) || IsCtl_(input) || IsTSpecial_(input)) {
        return (status_ = kBad);
      } else {
        state_ = method;
        req.method.push_back(input);
        return (status_ = kHeaderIndeterminate);
      }
    case method:
      if (input == ' ') {
        state_ = uri;
        return (status_ = kHeaderIndeterminate);
      } else if (!IsChar_(input) || IsCtl_(input) || IsTSpecial_(input)) {
        return (status_ = kBad);
      } else {
        req.method.push_back(input);
        return (status_ = kHeaderIndeterminate);
      }
    case uri:
      if (input == ' ') {
        int index = req.uri.find_first_of("?");
        if (index < 0) {
          req.short_uri = req.uri;
        } else {
          req.short_uri = req.uri.substr(0, index);
          req.query_string = req.uri.substr(index + 1, string::npos);
        }
        state_ = http_version_h;
        return (status_ = kHeaderIndeterminate);
      } else if (IsCtl_(input)) {
        return (status_ = kBad);
      } else {
        req.uri.push_back(input);
        return (status_ = kHeaderIndeterminate);
      }
    case http_version_h:
      if (input == 'H') {
        state_ = http_version_t_1;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_t_1:
      if (input == 'T') {
        state_ = http_version_t_2;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_t_2:
      if (input == 'T') {
        state_ = http_version_p;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_p:
      if (input == 'P') {
        state_ = http_version_slash;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_slash:
      if (input == '/') {
        req.http_version_major = 0;
        req.http_version_minor = 0;
        state_ = http_version_major_start;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_major_start:
      if (IsDigit_(input)) {
        req.http_version_major = req.http_version_major * 10 + input - '0';
        state_ = http_version_major;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_major:
      if (input == '.') {
        state_ = http_version_minor_start;
        return (status_ = kHeaderIndeterminate);
      } else if (IsDigit_(input)) {
        req.http_version_major = req.http_version_major * 10 + input - '0';
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_minor_start:
      if (IsDigit_(input)) {
        req.http_version_minor = req.http_version_minor * 10 + input - '0';
        state_ = http_version_minor;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case http_version_minor:
      if (input == '\r') {
        state_ = expecting_newline_1;
        return (status_ = kHeaderIndeterminate);
      } else if (IsDigit_(input)) {
        req.http_version_minor = req.http_version_minor * 10 + input - '0';
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case expecting_newline_1:
      if (input == '\n') {
        state_ = header_line_start;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case header_line_start:
      if (input == '\r') {
        state_ = expecting_newline_3;
        return (status_ = kHeaderIndeterminate);
      } else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
        state_ = header_lws;
        return (status_ = kHeaderIndeterminate);
      } else if (!IsChar_(input) || IsCtl_(input) || IsTSpecial_(input)) {
        return (status_ = kBad);
      } else {
        last_header_name_ = "";
        last_header_value_ = "";

        last_header_name_.push_back(input);
        state_ = header_name;
        return (status_ = kHeaderIndeterminate);
      }
    case header_lws:
      if (input == '\r') {
        state_ = expecting_newline_2;
        return (status_ = kHeaderIndeterminate);
      } else if (input == ' ' || input == '\t') {
        return (status_ = kHeaderIndeterminate);
      } else if (IsCtl_(input)) {
        return (status_ = kBad);
      } else {
        state_ = header_value;
        last_header_value_.push_back(input);
        return (status_ = kHeaderIndeterminate);
      }
    case header_name:
      if (input == ':') {
        state_ = space_before_header_value;
        return (status_ = kHeaderIndeterminate);
      } else if (!IsChar_(input) || IsCtl_(input) || IsTSpecial_(input)) {
        return (status_ = kBad);
      } else {
        last_header_name_.push_back(input);
        return (status_ = kHeaderIndeterminate);
      }
    case space_before_header_value:
      if (input == ' ') {
        state_ = header_value;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case header_value:
      if (input == '\r') {
        state_ = expecting_newline_2;
        return (status_ = kHeaderIndeterminate);
      } else if (IsCtl_(input)) {
        return (status_ = kBad);
      } else {
        last_header_value_.push_back(input);
        return (status_ = kHeaderIndeterminate);
      }
    case expecting_newline_2:
      if (input == '\n') {
        req.headers[last_header_name_] = last_header_value_;
        state_ = header_line_start;
        return (status_ = kHeaderIndeterminate);
      } else {
        return (status_ = kBad);
      }
    case expecting_newline_3:
      return (status_ = ((input == '\n') ? kHeaderReady : kBad));
    default:
      return (status_ = kBad);
  }
}

bool RequestParser::IsChar_(int c) { return c >= 0 && c <= 127; }

bool RequestParser::IsCtl_(int c) { return (c >= 0 && c <= 31) || (c == 127); }

bool RequestParser::IsTSpecial_(int c) {
  switch (c) {
    case '(':
    case ')':
    case '<':
    case '>':
    case '@':
    case ',':
    case ';':
    case ':':
    case '\\':
    case '"':
    case '/':
    case '[':
    case ']':
    case '?':
    case '=':
    case '{':
    case '}':
    case ' ':
    case '\t':
      return true;
    default:
      return false;
  }
}

bool RequestParser::IsDigit_(int c) { return c >= '0' && c <= '9'; }

}  // namespace http
}  // namespace np
