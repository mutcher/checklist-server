#include "HttpServer.h"

#include <cassert>
#include <iostream>

#include "HttpRequest.h"

HttpServer::HttpServer(HttpRequestHandler* listener, uint16_t port)
    : m_port(port), m_server_socket(), m_listener(listener) {
  assert(listener != nullptr);
}

void HttpServer::run() {
  m_server_socket.bind(m_port);
  m_server_socket.listen();

  while (true) {
    auto client_socket = m_server_socket.accept();
    if (client_socket.is_valid()) {
      std::string buffer;
      auto received = client_socket.read(buffer);
      std::cout << "------------REQUEST------------" << std::endl
                << "received=" << received << std::endl
                << buffer.c_str() << "--------------------------------"
                << std::endl;

      HttpRequest request;
      if (HttpRequest::deserialize(buffer, request)) {
        HttpResponse response;
        m_listener->on_request_received(request, response);
        buffer = response.serialize();
        std::cout << "------------RESPONSE------------" << std::endl
                  << buffer.c_str() << "--------------------------------"
                  << std::endl;
        client_socket.send(buffer);
      }
      client_socket.close();
    }
  }
}
