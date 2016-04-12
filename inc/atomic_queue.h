#pragma once
#include <queue>
#include "atomic_lock.h"
#include <mutex>

template<typename T>
class atomic_queue
{
private:
    atomic_lock m_lock;
    std::queue<T> m_queue;

public:
    atomic_queue()
        :m_lock(), m_queue()
    {
    }

    void push(const T& _item)
    {
        std::lock_guard<atomic_lock> lock(m_lock);
        m_queue.push(_item);
    }

    bool try_pop(T& _item)
    {
        bool bret = false;
        std::lock_guard<atomic_lock> lock(m_lock);
        if (!m_queue.empty())
        {
            _item = m_queue.front();
            m_queue.pop();
            bret = true;
        }
        return bret;
    }

    bool empty()
    {
        std::lock_guard<atomic_lock> lock(m_lock);
        return m_queue.empty();
    }
};