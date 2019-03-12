#include "RestApiService.h"
#include "network/HttpUriPatternProcessor.h"

RestApiService::RestApiService(uint16_t port_number)
    : m_http_server(new HttpServer(this, port_number)),
      m_data_manager(new DataManager()) {}

void RestApiService::run() {
  m_http_server->run();
}

void RestApiService::on_request_received(const HttpRequest& request,
                                         HttpResponse& response) {
  response.set_status_code(HttpStatusCode::HTTP_STATUS_NOT_FOUND);
  std::string buffer;
  HttpUriPatternProcessor uri_processor;

  uri_processor.process_uri(request.get_uri(), "/list");
  if (request.get_method() == "GET" && uri_processor.is_uri_matched()) {
    if (m_data_manager->get_list_serialized(DataManager::INVALID_ID, buffer) ==
        ErrorCode::ERROR_OK) {
      response.set_status_code(HttpStatusCode::HTTP_STATUS_OK);
      response.set_body(buffer);
    }
  }

  uri_processor.process_uri(request.get_uri(), "/list/:id");
  if (request.get_method() == "GET" && uri_processor.is_uri_matched()) {
    assert(!uri_processor.get_parameters().at(":id").empty());
    DataManager::IdType list_id =
        std::stoull(uri_processor.get_parameters().at(":id"));
    if (m_data_manager->get_list_serialized(DataManager::INVALID_ID, buffer) ==
        ErrorCode::ERROR_OK) {
      response.set_status_code(HttpStatusCode::HTTP_STATUS_OK);
      response.set_body(buffer);
    }
  }
}
