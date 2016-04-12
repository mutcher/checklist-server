#pragma once
#include <cstdint>
#include <cstddef>
#include "address.h"

typedef unsigned char uchar_t;

namespace socket_system
{
    bool init();
    void shutdown();
}

class socket
{
protected:
    uint64_t m_handle;
    address m_addr;

protected:
    uint64_t createSocket(int af, int type, int proto);

public:
    socket();
    socket(const address& addr);
    socket(const uint64_t& handle, const address& addr);

    void close();
    int receive(char* buff, size_t offset, size_t length);
    bool send(const char* buff, const size_t& size);

    const address& getAddress() const;
    const uint64_t& getID() const;
};
