#include "HttpUriPatternProcessor.h"
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

HttpUriPatternProcessor::HttpUriPatternProcessor() : m_is_uri_matched(false) {}

void HttpUriPatternProcessor::process_uri(const std::string& uri,
                                          const std::string& pattern) {
  m_is_uri_matched = false;
  m_parameters.clear();

  const auto delimeter = '/';
  auto splitted_uri = split_string(uri, delimeter);
  auto splitted_pattern = split_string(pattern, delimeter);

  if (splitted_uri.size() == splitted_pattern.size()) {
    for (size_t i = 0u; i < splitted_pattern.size(); ++i) {
      if (splitted_pattern[i][0] == ':') {
        m_parameters[splitted_pattern[i]] = splitted_uri[i];
      } else {
        if (splitted_uri[i] != splitted_pattern[i]) {
          return;
        }
      }
    }
    m_is_uri_matched = true;
  }
}

bool HttpUriPatternProcessor::is_uri_matched() const {
  return m_is_uri_matched;
}

const std::map<std::string, std::string>&
HttpUriPatternProcessor::get_parameters() const {
  return m_parameters;
}