#pragma once
#include <thread>
#include "atomic_queue.h"

class log_manager
{
private:
    typedef std::pair<std::chrono::system_clock::time_point, std::string> trace_item;
    atomic_queue<trace_item> m_messages;
    std::thread m_worker;
    std::atomic_bool m_work_flag;

private:
    void print_to_stream(std::ofstream& stream);
    void thread_worker();

public:
    log_manager();
    ~log_manager();

    void init();
    void log(const std::string& message);
    void shutdown();
};