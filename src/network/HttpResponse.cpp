#include "HttpResponse.h"

#include <sstream>

namespace {
std::string http_status_code_to_string(HttpStatusCode code) {
  switch (code) {
    case HttpStatusCode::HTTP_STATUS_OK:
      return "OK";
    case HttpStatusCode::HTTP_STATUS_NOT_FOUND:
      return "Page Not Found";
    default:
      return "";
  }
}
}  // namespace

HttpResponse::HttpResponse() : m_status_code(HttpStatusCode::HTTP_STATUS_OK) {}

void HttpResponse::set_status_code(HttpStatusCode status_code) {
  m_status_code = status_code;
}

void HttpResponse::set_body(const std::string& body) {
  m_body = body;
}

std::string HttpResponse::serialize() const {
  std::stringstream ss;
  ss << "HTTP/1.1 " << static_cast<uint32_t>(m_status_code) << " "
     << http_status_code_to_string(m_status_code) << std::endl
     << "Connection: Closed" << std::endl;
  if (m_body.empty()) {
    ss << std::endl;
  } else {
    ss << "Content-Type: application/json" << std::endl
       << "Content-Length: " << m_body.size() << std::endl
       << std::endl
       << m_body << std::endl;
  }
  return std::move(ss.str());
}