#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class thread_safe_queue
{
protected:
    bool m_break_waiting;
    queue<T> m_data;
    mutable mutex m_mutex;
    condition_variable m_cond;
public:
    thread_safe_queue() : m_break_waiting(false) {}

    void break_waiting()
    {
        lock_guard<mutex> lk(m_mutex);
        m_break_waiting = true;
        m_cond.notify_one();
    }
    void wait_and_pop(T& out_value)     // блокировка потока с ожиданием появления данных
    {
        unique_lock<mutex> lk(m_mutex);
        m_cond.wait(lk, [this]{
            return !m_data.empty() || m_break_waiting; });

        out_value = m_data.front();
        m_data.pop();

        m_break_waiting = false;
    }

    bool try_pop(T& out_value)
    {
        lock_guard<mutex> lk(m_mutex);
        if (m_data.empty())
            return false;

        out_value = m_data.front();
        m_data.pop();
        return true;
    }

    void push(T& in_value)
    {
        lock_guard<mutex> lk(m_mutex);
        bool is_empty = m_data.empty();
        m_data.push(move(in_value));

        if (is_empty)
            m_cond.notify_one();
    }

    void push(T&& in_value)
    {
        lock_guard<mutex> lk(m_mutex);

        bool is_empty = m_data.empty();
        m_data.push(move(in_value));

        if (is_empty)
            m_cond.notify_one();
    }

    void append(queue<T>& in_data)
    {
        lock_guard<mutex> lk(m_mutex);
        bool is_empty = m_data.empty();
        while(in_data.empty())
        {
            m_data.push(in_data.front());
            in_data.pop();
        }

        if (is_empty)
            m_cond.notify_one();
    }

    void swap(queue<T>& out_data)
    {
        lock_guard<mutex> lk(m_mutex);
        m_data.swap(out_data);
    }

    void wait_and_swap(queue<T>& in_data)   // блокировка потока с ожиданием появления данных
    {
        unique_lock<mutex> lk(m_mutex);
        m_cond.wait(lk, [this]{
            return !m_data.empty() || m_break_waiting; });

        m_break_waiting = false;
        m_data.swap(in_data);
    }

    bool empty() const
    {
        lock_guard<mutex> lk(m_mutex);
        return m_data.empty();
    }
};

#endif // THREAD_SAFE_QUEUE_H
