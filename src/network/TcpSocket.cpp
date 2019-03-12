#include "TcpSocket.h"

// clang-format off
#ifdef _WIN32
# include <WinSock2.h>
typedef int socklen_t;
#else
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <unistd.h>
#endif
// clang-format on

#include <cassert>
#include <iostream>
#include <vector>

namespace {

#ifdef _WIN32
const PlatformSocketType PLATFORM_INVALID_SOCKET = INVALID_SOCKET;
#else
const PlatformSocketType PLATFORM_INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;
#endif

void print_socket_error(const char* function_name) {
#ifdef _WIN32
  std::cout << function_name << " returned " << WSAGetLastError() << std::endl;
#else
  std::cout << function_name << " returned " << errno << std::endl;
#endif
}

}  // namespace

TcpSocket::TcpSocket() : m_platform_socket(PLATFORM_INVALID_SOCKET) {
#ifdef _WIN32
  WSAData wsa_data = {};

  auto error_code = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (error_code != 0) {
    std::cout << "WSAStartup returned " << error_code << std::endl;
  }
#endif
}

void TcpSocket::set_platform_socket(PlatformSocketType platform_socket) {
  m_platform_socket = platform_socket;
}

PlatformSocketType TcpSocket::get_platform_socket() const {
  return m_platform_socket;
}

bool TcpSocket::is_valid() const {
  return m_platform_socket != PLATFORM_INVALID_SOCKET;
}

void TcpSocket::bind(uint16_t port) {
  sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

#ifdef _WIN32
  m_platform_socket =
      WSASocketW(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);
#else
  m_platform_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
  if (m_platform_socket == PLATFORM_INVALID_SOCKET) {
    print_socket_error("socket");
    return;
  }

  auto error_code =
      ::bind(m_platform_socket, reinterpret_cast<const sockaddr*>(&addr),
             sizeof(addr));
  if (error_code == SOCKET_ERROR) {
    print_socket_error("bind");
    return;
  }
}

void TcpSocket::listen() {
  auto error_code = ::listen(m_platform_socket, SOMAXCONN);
  if (error_code == SOCKET_ERROR) {
    print_socket_error("listen");
  }
}

void TcpSocket::close() {
#ifdef _WIN32
  closesocket(m_platform_socket);
#else
  ::close(m_platform_socket);
#endif
}

TcpSocket TcpSocket::accept() {
  TcpSocket socket;

  sockaddr_in addr = {0};
  socklen_t addr_size = sizeof(addr);
  auto platform_socket = ::accept(
      m_platform_socket, reinterpret_cast<sockaddr*>(&addr), &addr_size);

  if (platform_socket == SOCKET_ERROR) {
    print_socket_error("accept");
  } else {
    assert(addr_size == sizeof(addr) &&
           "Address size should equals to expected");
    socket.set_platform_socket(platform_socket);
  }
  return socket;
}

size_t TcpSocket::read(std::string& buffer) {
  auto received = 0;
  size_t total_received = 0u;
  std::vector<char> internal_buffer(50);
  while (true) {
    auto received = ::recv(m_platform_socket, &internal_buffer[0],
                           internal_buffer.size(), 0);
    if (received <= 0) {
      break;
    }
    buffer.append(internal_buffer.cbegin(),
                  internal_buffer.cbegin() + received);
    total_received += received;
    if (received < internal_buffer.size()) {
      break;
    }
  }

  if (received == SOCKET_ERROR) {
    print_socket_error("recv");
    total_received = 0;
  }
  return total_received;
}

void TcpSocket::send(const std::string& buffer) {
  auto sent = ::send(m_platform_socket, buffer.data(), buffer.size(), 0);
}
