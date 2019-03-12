#pragma once
#include "HttpRequest.h"
#include "HttpResponse.h"

class HttpRequestHandler {
 public:
  virtual ~HttpRequestHandler();

  virtual void on_request_received(const HttpRequest& request,
                                   HttpResponse& response);
};
