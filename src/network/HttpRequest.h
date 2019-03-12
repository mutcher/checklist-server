#pragma once
#include <map>
#include <string>

class HttpRequest {
 public:
  HttpRequest() = default;
  HttpRequest(const std::string& method,
              const std::string& uri,
              const std::string& body);

  // Function returns a method: GET, POST, PUT, DELETE
  const std::string& get_method() const;

  // Function returns a relative uri
  const std::string& get_uri() const;

  // Function returns a request body
  const std::string& get_body() const;

  static bool deserialize(const std::string& buffer, HttpRequest& request);

 private:
  std::string m_method;
  std::string m_uri;
  std::string m_body;
};
