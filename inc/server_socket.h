#pragma once
#include <vector>
#include "socket.h"

class server_socket : public socket
{
private:
    std::vector<socket> m_clientList;

public:
    server_socket(const address& addr);
    bool accept(socket& client);
    bool open();
    void shutdown();
};