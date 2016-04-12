#include "log_manager.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

std::string get_time_stamp(std::chrono::system_clock::time_point time_point)
{
    std::time_t timestamp = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm;

#ifdef _MSC_VER
    gmtime_s(&tm, &timestamp);
#else
    std::tm* tmp = std::gmtime(&timestamp);
    std::memcpy(tmp, &tm, sizeof(tm));
#endif // _MSC_VER

    std::stringstream ss;
    ss << std::put_time(&tm, "%m/%d/%Y_%H:%M:%SUTC");
    return ss.str();
}

log_manager::log_manager()
    :m_messages(), m_worker(), m_work_flag()
{
}

log_manager::~log_manager()
{
    shutdown();
}

void log_manager::log(const std::string& message)
{
    trace_item item;
    item.first = std::chrono::system_clock::now();
    item.second = message;
    m_messages.push(item);
}

void log_manager::init()
{
    m_work_flag.store(true);
    m_worker = std::thread(&log_manager::thread_worker, this);
}

void log_manager::shutdown()
{
    m_work_flag.store(false);
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

void log_manager::print_to_stream(std::ofstream& stream)
{
    trace_item item;
    while(m_messages.try_pop(item))
    {
        std::stringstream ss;
        ss << "[";
        ss << get_time_stamp(item.first);
        ss << "]: ";
        ss << item.second;

        std::string str = ss.str();

        std::cout << str << std::endl;
        stream << str << std::endl;
    }
}

void log_manager::thread_worker()
{
    std::stringstream ss;
    ss << "trace.";
    ss << std::chrono::system_clock::now().time_since_epoch().count();
    ss << ".trc";

    std::ofstream trace_file(ss.str(), std::ios_base::out);
    while(m_work_flag)
    {
        if (!m_messages.empty())
        {
            print_to_stream(trace_file);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(75));
        }
    }

    if (!m_messages.empty())
    {
        print_to_stream(trace_file);
    }
    trace_file.close();
}