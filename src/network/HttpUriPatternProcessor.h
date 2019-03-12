#pragma once
#include <map>
#include <string>

class HttpUriPatternProcessor {
 public:
  HttpUriPatternProcessor();

  void process_uri(const std::string& uri, const std::string& pattern);

  bool is_uri_matched() const;
  const std::map<std::string, std::string>& get_parameters() const;

 private:
  bool m_is_uri_matched;
  std::map<std::string, std::string> m_parameters;
};
