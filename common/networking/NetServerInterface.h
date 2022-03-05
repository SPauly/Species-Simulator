#pragma once
#include "NetCommonInclude.h"
#include "NetTSQueue.h"
#include "NetMsg.h"
#include "NetConnection.h"

namespace sim
{
    namespace net
    {
        template <typename T>
        class Server_Interface
        {
        private:
            TSQueue<OwnedMessage<T>> m_qMessagesIn;

            std::deque<std::shared_ptr<Connection<T>> m_deqConnections;
        }
    }
}