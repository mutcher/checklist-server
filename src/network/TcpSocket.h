#pragma once
#include <cstdint>
#include <memory>
#include <string>

// clang-format off
#ifdef _WIN32
# ifdef _WIN64
  typedef unsigned __int64 PlatformSocketType;
# else
  typedef unsigned int PlatformSocketType;
# endif // _WIN64
#else
  typedef int PlatformSocketType;
#endif //_WIN32
// clang-format on

class TcpSocket {
 public:
  TcpSocket();

  void set_platform_socket(PlatformSocketType platform_socket);
  PlatformSocketType get_platform_socket() const;
  bool is_valid() const;

  void bind(uint16_t port);
  void listen();

  void close();

  TcpSocket accept();

  size_t read(std::string& buffer);
  void send(const std::string& buffer);

 private:
  PlatformSocketType m_platform_socket;
};
