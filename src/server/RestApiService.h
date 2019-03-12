#pragma once
#include <data/DataManager.h>

#include <network/HttpRequestHandler.h>
#include <network/HttpServer.h>

class RestApiService : public HttpRequestHandler {
 public:
  RestApiService();

  void run();

  virtual void on_request_received(const HttpRequest& request,
                                   HttpResponse& response);

 private:
  std::unique_ptr<HttpServer> m_http_server;
  std::unique_ptr<DataManager> m_data_manager;
};