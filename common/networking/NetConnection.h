#pragma once
#include "NetCommonInclude.h"
#include "NetTSQueue.h"
#include "NetMsg.h"

namespace sim
{
    namespace net
    {
        template <typename T>
        class Connection : std::enable_shared_from_this<Connection<T>> // Object to handle connections between server and client -> abstracts all the communication related stuff
        {
        private:
            enum class owner : uint8_t 
            {
                server,
                client
            };
        
        public:
            Connection(owner parent, asio::io_context context, asio::ip::tcp::socket socket, uint16_t port, TSQueue<OwnedMessage<T>> &qMessagesIn)  //owner = server | client, context = needs an asio context to run in, takes over the socket and handles connection, port = port at which to connect, Queue of Messages comming in
                : m_context(context); m_socket(socket); m_qMessagesIn(m_qMessagesIn)
            {
                m_nowner = parent; 
            }
            virtual ~Connection()
            {}

        public:
            

        protected:
            asio::io_context &m_context; //does not own the context since it only runs in it
            asio::ip::tcp::socket m_socket; // socket it handles

            TSQueue<OwnedMessage<T>> &m_qMessagesIn; 
            TSQueue<Message<T>> m_qMessagesOut;

            owner m_nowner = owner::server;
            uint32_t m_id = 0; // unique user id
        };
    }
}
