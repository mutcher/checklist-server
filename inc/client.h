#ifndef CLIENT_H_9192016
#define CLIENT_H_9192016
#pragma once

#include "client_socket.h"
#include "list_processor.h"
#include "log_manager.h"

class client
{
private:
    client_socket& m_socket;
    list_processor& m_processor;
    log_manager* m_log;

private:
    void getList(network_packet packet);
    void addListItem(network_packet packet);
    void deleteListItem(network_packet packet);

public:
    client(client_socket& clientSocket, list_processor& processor, log_manager* log);
    ~client();
    bool init();
    void processClientRequests();
};
#endif //CLIENT_H_9192016
