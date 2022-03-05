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
        class Client_Interface
        {
        public:
            Client_Interface()
            {
            }
            virtual ~Client_Interface()
            {
                // disconnect
            }

            bool connect_to_server(const std::string &host, const uint16_t port)
            {
                try
                {
                    asio::ip::tcp::resolver resolver(m_AsioContext);
                    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                    m_uptrConnection = std::make_unique<Connection<T>>(
                        Connection<T>::owner::client, m_AsioContext,
                        asio::ip::tcp::socket(m_AsioContext), m_qMessagesIn);

                    m_uptrConnection->connect_to_server(endpoints);

                    m_ContextThread = std::thread([this]()
                                                  { m_AsioContext.run(); });
                }
                catch (const std::exception &e)
                {
                    // inform that could not connect to server
                    // std::cerr << e.what() << '\n';
                    return false;
                }
                return true;
            }

            void disconnect()
            {
                if (is_connected())
                    m_uptrConnection->disconnect();

                m_AsioContext.stop();

                if (m_ContextThread.joinable())
                    m_ContextThread.join();

                m_uptrConnection.release();
            }

            bool is_connected()
            {
                if (m_uptrConnection)
                    return m_uptrConnection->is_connected();
                else
                    return false;
            }

        public:
            void send(const Message<T>& msg)
            {
                if(is_connected())
                {
                    m_uptrConnection->send(msg);
                }
            }

            TSQueue<OwnedMessage<T>> &get_incomming_messages()
            {
                return m_qMessagesIn;
            }

        protected:
            asio::io_context m_AsioContext;
            std::thread m_ContextThread;

            std::unique_ptr<Connection<T>> m_uptrConnection;

        private:
            TSQueue<OwnedMessage<T>> m_qMessagesIn;
        };
    }
}