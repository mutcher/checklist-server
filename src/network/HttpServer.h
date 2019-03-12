#pragma once
#include "HttpRequestHandler.h"
#include "TcpSocket.h"

class HttpServer {
 public:
  HttpServer(HttpRequestHandler* listener, uint16_t port);

  void run();

 private:
  uint16_t m_port;
  TcpSocket m_server_socket;
  HttpRequestHandler* m_listener;
};
