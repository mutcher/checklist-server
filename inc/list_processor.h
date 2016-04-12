#pragma once
#include <string>
#include <vector>
#include <thread>
#include "atomic_lock.h"
#include "socket.h"


class list_processor
{
private:
    atomic_lock m_lock;
    std::vector<std::pair<std::u16string, std::vector<std::u16string>>> m_lists;
    std::thread m_workingThread;
    bool m_isWorking;

private:
    void workingThread();
    void syncFromFile();
    void syncToFile();

public:
    list_processor();
    void init();
    void shutdown();

    uchar_t createList(const std::u16string& listName);
    bool deleteList(const std::u16string& listName);

    bool addListItem(const uchar_t& listID, const std::u16string& itemName);
    bool deleteListItem(const uchar_t& listID, const std::u16string& listName);

    std::vector<std::u16string> getList(const uchar_t& listID);
};