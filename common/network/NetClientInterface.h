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
        class Client_Interface //Client Interface abstracts the connection to the server 
        {
        public:
            Client_Interface()
            {
            }
            virtual ~Client_Interface()
            {
                // disconnect
            }

            bool connect_to_server(const std::string &host, const uint16_t port) // host can be a domain or IP4 Adress 
            {
                try
                {
                    asio::ip::tcp::resolver resolver(m_AsioContext);
                    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                    m_uptrConnection = std::make_unique<Connection<T>>( //creates a connection object
                        Connection<T>::owner::client, m_AsioContext,
                        asio::ip::tcp::socket(m_AsioContext), m_qMessagesIn);

                    m_uptrConnection->connect_to_server(endpoints); //try connect to the server via the provided entpoint

                    m_ContextThread = std::thread([this]() 
                                                  { m_AsioContext.run(); }); // give asio context a thread to run in
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Client Exception: " << e.what() << '\n';
                    return false;
                }
                return true;
            }

            void disconnect()
            {
                if (is_connected())
                    m_uptrConnection->disconnect(); //disconnect from server

                m_AsioContext.stop();   //stop asio context

                if (m_ContextThread.joinable()) //join thread
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