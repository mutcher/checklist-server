#pragma once
#include <atomic>

class atomic_lock
{
private:
    std::atomic_flag m_flag;

public:
    atomic_lock()
        :m_flag()
    {
        // Yes-yes, I know that std::atomic_flag should be initialized with ATOMIC_FLAG_INIT,
        // but f**king VS2012 not allow me to use such initialization.
        m_flag.clear(std::memory_order_release);
    }

    void lock()
    {
        while(std::atomic_flag_test_and_set_explicit(&m_flag, std::memory_order_acquire));
    }

    void unlock()
    {
        std::atomic_flag_clear_explicit(&m_flag, std::memory_order_release);
    }
};
