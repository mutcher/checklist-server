#include <iostream>
#include "inc/client_socket.h"

// 3 symbols for signature + opcode + subcode
uchar_t client_socket::min_packet_length = 5;

client_socket::client_socket()
    :_good(true), m_handle(0), m_addr()
{
}

client_socket::client_socket(const address & addr)
    :_good(true), m_handle(0), m_addr(addr)
{
}

client_socket::client_socket(const uint64_t & handle, const address & addr)
    :_good(true), m_handle(handle), m_addr(addr)
{
}

void client_socket::close()
{
#if PLATFORM == PLATFORM_WINDOWS
    closesocket(static_cast<SOCKET>(m_handle));
#else
    shutdown(m_handle, 2);
#endif
}

const address & client_socket::getAddress() const
{
    return m_addr;
}

const uint64_t & client_socket::getID() const
{
    return m_handle;
}

uint64_t client_socket::createSocket(int af, int type, int proto)
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