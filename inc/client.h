#pragma once
#include "socket.h"
#include "list_processor.h"
#include "log_manager.h"

class client
{
private:
    socket& m_socket;
    list_processor& m_processor;
    log_manager* m_log;

private:
    void getList(const uchar_t& listID);
    void addListItem(const uchar_t& listID, const std::u16string& itemName);
    void deleteListItem(const uchar_t& listID, const std::u16string& itemName);

public:
    client(socket& clientSocket, list_processor& processor, log_manager* log);
    ~client();
    bool init();
    void processClientRequests();
};
