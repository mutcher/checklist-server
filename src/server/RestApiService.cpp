#include "RestApiService.h"

namespace {
const uint16_t SERVICE_PORT = 9090;
}

RestApiService::RestApiService()
    : m_http_server(new HttpServer(this, SERVICE_PORT)),
      m_data_manager(new DataManager()) {}

void RestApiService::run() {
  m_http_server->run();
}

void RestApiService::on_request_received(const HttpRequest& request,
                                         HttpResponse& response) {
  response.set_status_code(HttpStatusCode::HTTP_STATUS_NOT_FOUND);
  if (request.get_method() == "GET" &&
      request.is_uri_match_to_pattern("/list")) {
    std::string buffer;
    if (m_data_manager->get_list_serialized(DataManager::INVALID_ID, buffer) ==
        ErrorCode::ERROR_OK) {
      response.set_status_code(HttpStatusCode::HTTP_STATUS_OK);
      response.set_body(buffer);
    }
  } else if (request.get_method() == "GET" &&
             request.is_uri_match_to_pattern("/list/:id")) {
    
  }
}
