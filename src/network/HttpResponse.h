#pragma once
#include <cstdint>
#include <string>

enum class HttpStatusCode : uint16_t {
  HTTP_STATUS_OK = 200u,
  HTTP_STATUS_NOT_FOUND = 404u 
};

class HttpResponse {
 public:
  HttpResponse();
  void set_status_code(HttpStatusCode status_code);
  void set_body(const std::string& body);

  std::string serialize() const;

 private:
  HttpStatusCode m_status_code;
  std::string m_body;
};
