#include <iostream>
#include "socket.h"
#include "platform.h"

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

socket::socket()
    :m_handle(0), m_addr()
{
}

socket::socket(const address & addr)
    :m_handle(0), m_addr(addr)
{
}

socket::socket(const uint64_t & handle, const address & addr)
    :m_handle(handle), m_addr(addr)
{
}

void socket::close()
{
#if PLATFORM == PLATFORM_WINDOWS
    closesocket(m_handle);
#else
    shutdown(m_handle, 2);
#endif
}

int socket::receive(char* buff, size_t offset, size_t length)
{
    int count = ::recv(m_handle, &buff[offset], static_cast<int>(length), 0);
    return count;
}

bool socket::send(const char* buff, const size_t& size)
{
    socklen_t count = ::send(m_handle, buff, static_cast<const socklen_t&>(size), 0);
    if (count != size)
    {
        return false;
    }

    return true;
}

const address & socket::getAddress() const
{
    return m_addr;
}

const uint64_t & socket::getID() const
{
    return m_handle;
}

uint64_t socket::createSocket(int af, int type, int proto)
{
    return ::socket(af, type, proto);
}

namespace socket_system
{
    bool init()
    {
    #if PLATFORM == PLATFORM_WINDOWS
        WSAData wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
    #else
        return true;
    #endif
    }

    void shutdown()
    {
    #if PLATFORM == PLATFORM_WINDOWS
        WSACleanup();
    #endif
    }
}