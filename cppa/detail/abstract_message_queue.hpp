#ifndef ABSTRACT_MESSAGE_QUEUE_HPP
#define ABSTRACT_MESSAGE_QUEUE_HPP

#include <memory>

#include "cppa/message.hpp"
#include "cppa/message_queue.hpp"

namespace cppa { namespace detail {

template<class Super>
class abstract_message_queue : public Super
{

    message m_last_dequeued;

 public:

    template<typename... Args>
    abstract_message_queue(const Args&... args) : Super(args...)
    {
    }

    const message& dequeue() /*override*/
    {
        for (;;)
        {
            if (Super::dequeue_impl(m_last_dequeued))
            {
                return m_last_dequeued;
            }
        }
    }

    void dequeue(invoke_rules& rules) /*override*/
    {
        typename Super::queue_node_buffer buffer;
        for (;;)
        {
            if (Super::dequeue_impl(rules, buffer))
            {
                return;
            }
        }
    }

    bool try_dequeue(message& msg) /*override*/
    {
        for (;;)
        {
            if (Super::empty())
            {
                return false;
            }
            else if (Super::dequeue_impl(m_last_dequeued))
            {
                msg = m_last_dequeued;
                return true;
            }
        }
    }

    bool try_dequeue(invoke_rules& rules) /*override*/
    {
        typename Super::queue_node_buffer buffer;
        for (;;)
        {
            if (Super::empty())
            {
                Super::restore_mailbox(buffer);
                return false;
            }
            else if (Super::dequeue_impl(rules, buffer))
            {
                return true;
            }
        }
    }

    const message& last_dequeued() /*override*/
    {
        return m_last_dequeued;
    }

};

} } // namespace cppa::detail

#endif // ABSTRACT_MESSAGE_QUEUE_HPP