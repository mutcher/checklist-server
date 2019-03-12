#include "HttpRequest.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace {}  // namespace

HttpRequest::HttpRequest(const std::string& method,
                         const std::string& uri,
                         const std::string& body)
    : m_method(method), m_uri(uri), m_body(body) {}

const std::string& HttpRequest::get_method() const {
  return m_method;
}

const std::string& HttpRequest::get_uri() const {
  return m_uri;
}

const std::string& HttpRequest::get_body() const {
  return m_body;
}

bool HttpRequest::deserialize(const std::string& buffer, HttpRequest& request) {
  std::string method;
  std::string uri;
  std::string body;
  std::stringstream ss(buffer);
  ss >> method;
  ss >> uri;

  // looking for empty line before "body"
  size_t pos = 0u, prev_pos = 10u;
  while (true) {
    pos = buffer.find('\n', pos + 1);
    if (pos == std::string::npos || pos >= buffer.size() ||
        pos - prev_pos <= 2u) {
      break;
    }
    prev_pos = pos;
  }

  if (pos == std::string::npos || method.empty() || uri.empty()) {
    return false;
  }

  body.append(buffer.cbegin() + pos + 1, buffer.cend());
  request = HttpRequest(method, uri, body);
  return true;
}
