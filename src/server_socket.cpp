#include "server_socket.h"
#include "platform.h"
#include <iostream>

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef int socklen_t;
#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define INVALID_SOCKET static_cast<uint64_t>(-1)
#endif

typedef class socket client_socket;

server_socket::server_socket(const address& addr)
    :client_socket(addr), m_clientList()
{
}

bool server_socket::accept(client_socket& client)
{
    bool ret = false;
    sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    uint64_t client_handle = ::accept(m_handle, reinterpret_cast<sockaddr*>(&addr), &addr_size);
    if (client_handle != INVALID_SOCKET)
    {
        client = client_socket(client_handle, address(addr.sin_addr.s_addr, ntohs(addr.sin_port)));
        m_clientList.push_back(client);
        ret = true;
    }
    return ret;
}

bool server_socket::open()
{
    m_handle = socket::createSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_handle <= 0)
    {
        std::cout << "Cannot create socket.ret=" << m_handle << std::endl;
        return false;
    }

    sockaddr_in addr;
    addr.sin_addr.s_addr = m_addr.getAddress();
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_addr.getPort());

    int ret = bind(m_handle, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
    if (ret != 0)
    {
        std::cout << "Cannot bind to " << m_addr.getPort() << " port.ret=" << ret << std::endl;
#if PLATFORM == PLATFORM_WINDOWS
        std::cout << "Windows errcode: " << WSAGetLastError() << std::endl;
#endif
        return false;
    }

    ret = ::listen(m_handle, 10);
    if (ret != 0)
    {
        std::cout << "Cannot start listen ret=" << ret << std::endl;
        return false;
    }

    return true;
}

void server_socket::shutdown()
{
    for(client_socket& clientSocket : m_clientList)
    {
        clientSocket.close();
    }
    this->close();
}