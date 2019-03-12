#include "HttpRequest.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace {
std::vector<std::string> split_string(const std::string& str, char delimeter) {
  std::string tmp_line;
  std::vector<std::string> items;
  std::istringstream tokenStream(str);
  while (std::getline(tokenStream, tmp_line, '/')) {
    if (!tmp_line.empty()) {
      items.push_back(tmp_line);
    }
  }
  return items;
}
}  // namespace

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

const bool HttpRequest::is_uri_match_to_pattern(
    const std::string& pattern) const {
  const auto delimeter = '/';
  auto splitted_uri = split_string(get_uri(), delimeter);
  auto splitted_pattern = split_string(pattern, delimeter);
  if (splitted_pattern.size() != splitted_uri.size()) {
    return false;
  }

  for (size_t i = 0; i < splitted_uri.size(); ++i) {
    if (*splitted_pattern[i].begin() == ':') {
      continue;
    }
    if (splitted_pattern[i] != splitted_uri[i]) {
      return false;
    }
  }
  return true;
}

bool HttpRequest::parse_uri_parameters(
    const std::string& pattern,
    std::map<std::string, std::string>& parameters) const {
  return false;
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
