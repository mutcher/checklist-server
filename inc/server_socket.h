#pragma once
#include <vector>
#include "client_socket.h"

class server_socket : public client_socket
{
private:
    std::vector<client_socket> m_clientList;

public:
    server_socket(const address& addr);
    bool accept(client_socket& client);
    bool open();
    void shutdown();
};