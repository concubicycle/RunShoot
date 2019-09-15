#ifndef _EVENTS_CODEPROJE_H__
#define _EVENTS_CODEPROJE_H__

#include <functional>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <list>

namespace sz
{

template <typename... Args>
class event_handler
{
    typedef std::function<void(Args...)> handler_func_type;

public:
    explicit event_handler(const handler_func_type &handlerFunc)
        : m_handlerFunc(handlerFunc)
    {
        m_handlerId = ++m_handlerIdCounter;
    }

    bool operator==(const event_handler &other) const
    {
        return m_handlerId == other.m_handlerId;
    }

    std::uint32_t id() const
    {
        return m_handlerId;
    }

    void operator()(Args... params) const
    {
        if (m_handlerFunc)
        {
            m_handlerFunc(params...);
        }
    }

private:
    static std::atomic_uint m_handlerIdCounter;

    std::uint32_t m_handlerId;
    handler_func_type m_handlerFunc;
};

template <typename... Args>
std::atomic_uint event_handler<Args...>::m_handlerIdCounter(0);

} // namespace sz

template <typename... Args>
class event
{
public:
    typedef event_handler<Args...> handler_type;

    typename handler_type::handler_id_type add(const handler_type &handler)
    {
        std::lock_guard<std::mutex> lock(m_handlersLocker);

        m_handlers.push_back(handler);
        return handler.id();
    }

    inline typename handler_type::handler_id_type add(const typename handler_type::handler_func_type &handler)
    {
        return add(handler_type(handler));
    }

    bool remove(const handler_type &handler)
    {
        std::lock_guard<std::mutex> lock(m_handlersLocker);

        auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
        if (it != m_handlers.end())
        {
            m_handlers.erase(it);
            return true;
        }

        return false;
    }

    bool remove_id(const typename handler_type::handler_id_type &handlerId)
    {
        std::lock_guard<std::mutex> lock(m_handlersLocker);

        auto it = std::find_if(m_handlers.begin(), m_handlers.end(),
                               [handlerId](const handler_type &handler) { return handler.id() == handlerId; });
        if (it != m_handlers.end())
        {
            m_handlers.erase(it);
            return true;
        }

        return false;
    }

protected:
    typedef std::list<handler_type> handler_collection_type;

private:
    handler_collection_type m_handlers;
};

#endif