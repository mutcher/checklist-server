#include <mutex>
#include <thread>
#include <fstream>
#include <sys/stat.h>

#include "list_processor.h"
#include "u16stream.h"

list_processor::list_processor()
    :m_lock(), m_lists(), m_workingThread(), m_isWorking(true)
{
}

void list_processor::init()
{
    syncFromFile();
    m_workingThread = std::thread(&list_processor::workingThread, this);
}

void list_processor::shutdown()
{
    m_isWorking = false;
    m_workingThread.join();
    syncToFile();
}

void list_processor::workingThread()
{
    short counter = 0;
    while(m_isWorking)
    {
        if (counter != 30)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            counter++;
        }
        else
        {
            counter = 0;
            syncToFile();
        }
    }
}

void list_processor::syncFromFile()
{
    char dbFilename[] = "main.db";

    struct stat st;
    if (stat(dbFilename, &st) != 0) //database file doesn't exists
    {
        u16ofstream writer(dbFilename);
        writer.close();
    }

    u16ifstream reader(dbFilename);
    std::pair<unsigned char, std::u16string> item;
    while(reader.readRecord(item))
    {
        if (item.first == 0)
        {
            m_lists.push_back(std::make_pair(item.second, std::vector<std::u16string>()));
        }
        else
        {
            m_lists[item.first - 1].second.push_back(item.second);
        }
    }
    reader.close();
}

void list_processor::syncToFile()
{
    m_lock.lock();
    auto listCopy = m_lists;
    m_lock.unlock();

    u16ofstream writer("main.db");
    if (writer)
    {
        for(size_t i = 0; i < listCopy.size(); ++i)
        {
            unsigned char listId = 0;
            writer.writeRecord(std::make_pair(listId, listCopy[i].first));

            for(const auto& itemName : listCopy[i].second)
            {
                listId = static_cast<unsigned char>(i + 1);
                writer.writeRecord(std::make_pair(listId, itemName));
            }
        }
    }
    writer.close();
}

uchar_t list_processor::createList(const std::u16string& listName)
{
    std::lock_guard<atomic_lock> lock(m_lock);
    uchar_t ret = 0;
    if (m_lists.size() < std::numeric_limits<uchar_t>::max())
    {
        bool isListExists = false;
        for (const auto& pair : m_lists)
        {
            if (listName == pair.first)
            {
                isListExists = true;
                break;
            }
        }
        if (!isListExists)
        {
            m_lists.push_back(std::make_pair(listName, std::vector<std::u16string>()));
            ret = static_cast<uchar_t>(m_lists.size());
        }
    }
    return ret;
}

bool list_processor::deleteList(const std::u16string& listName)
{
    bool ret = false;
    std::lock_guard<atomic_lock> lock(m_lock);
    for (auto iter = m_lists.begin(); iter != m_lists.end(); ++iter)
    {
        if (iter->first == listName)
        {
            m_lists.erase(iter);
            ret = true;
            break;
        }
    }
    return ret;
}

bool list_processor::addListItem(const uchar_t& listID, const std::u16string& itemName)
{
    bool bret = false;
    std::lock_guard<atomic_lock> lock(m_lock);
    auto& pair = m_lists[listID - 1];
    bool isItemExists = false;
    for(const auto& item : pair.second)
    {
        if (item == itemName)
        {
            isItemExists = true;
            break;
        }
    }
    if (!isItemExists)
    {
        pair.second.push_back(itemName);
        bret = true;
    }
    return bret;
}

bool list_processor::deleteListItem(const uchar_t& listID, const std::u16string& listName)
{
    bool bret = false;
    std::lock_guard<atomic_lock> lock(m_lock);
    auto& pair = m_lists[listID - 1];
    for(auto iter = pair.second.begin(); iter != pair.second.end(); ++iter)
    {
        if (*iter == listName)
        {
            pair.second.erase(iter);
            bret = true;
            break;
        }
    }
    return bret;
}

std::vector<std::u16string> list_processor::getList(const uchar_t& listID)
{
    std::lock_guard<atomic_lock> lock(m_lock);
    std::vector<std::u16string> list;
    if (listID == 0)
    {
        for(const auto& pair : m_lists)
        {
            list.push_back(pair.first);
        }
    }
    else
    {
        if (m_lists.size() >= listID)
        {
            const auto& pair = m_lists[listID - 1];
            list = pair.second;
        }
    }
    return list;
}
