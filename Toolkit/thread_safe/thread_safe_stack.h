#ifndef THREAD_SAFE_STACK_H
#define THREAD_SAFE_STACK_H

#include <stack>
#include <mutex>
#include <exception>
#include <memory>

using namespace std;

class stack_exception : public exception
{
public:
    stack_exception(const char* in_pWhat)
        : m_pWhat(in_pWhat)
    {

    }
    virtual char const* what() const noexcept
    {
        return m_pWhat;
    }

    const char* m_pWhat;
};

template<typename T>
class thread_safe_stack
{
protected:
    stack<T> m_data;
    mutable mutex m_mutex;
public:
    thread_safe_stack() {}
    thread_safe_stack(const thread_safe_stack& other)
    {
        lock_guard<mutex> lock(other.m_mutex);
        m_data = other.data;
    }
    thread_safe_stack& operator = (const thread_safe_stack&) = delete;
    unsigned int size()
    {
        lock_guard<mutex> lock(m_mutex);
        return m_data.size();
    }
    void swap(stack<T>& in_stack)
    {
        if (!in_stack.empty())
            throw stack_exception("swap stack not empty");

        lock_guard<mutex> lock(m_mutex);
        in_stack.swap(m_data);
    }
    void push (T new_value)
    {
        lock_guard<mutex> lock(m_mutex);
        m_data.push(new_value);
    }
    shared_ptr<T> pop()
    {
        lock_guard<mutex> lock(m_mutex);
        if (m_data.empty())
            throw stack_exception("Empty stack");

        shared_ptr<T> const res(make_shared<T>(move(m_data.top())));
        m_data.pop();
        return res;
    }
    void pop(T& value)
    {
        lock_guard<mutex> lock(m_mutex);
        if (m_data.empty())
            throw stack_exception("Empty stack");

        value = move(m_data.top());
        m_data.pop();
    }
    bool empty() const
    {
        lock_guard<mutex> lock(m_mutex);
        return m_data.empty();
    }

    unsigned int size() const
    {
        lock_guard<mutex> lock(m_mutex);
        return m_data.size();
    }
};

#endif // THREAD_SAFE_STACK_H
