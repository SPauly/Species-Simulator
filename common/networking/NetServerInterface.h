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
        public:
            Server_Interface(uint16_t port) : m_AsioAcceptor(m_AsioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
            {
            }
            virtual ~Server_Interface()
            {
            }

            bool start_server()
            {
                try
                {
                    wait_for_client_connection();
                    m_ContextThread = std::thread([this]()
                                                  { m_AsioContext.run(); });
                }
                catch (const std::exception &e)
                {
                    // throw a net error here
                    // std::cerr << e.what() << '\n';
                    return false;
                }

                // inform that new connection was made
                return true;
            }

            void stop_server()
            {
                m_AsioContext.stop();

                if (m_ContextThread.joinable())
                    m_ContextThread.join();

                // inform about server stop
            }

            void wait_for_client_connection()
            {
                m_AsioAcceptor.async_accept(
                    [this](std::error_code ec, asio::ip::tcp::socket socket)
                    {
                        if (!ec)
                        {
                            std::shared_ptr<Connection<T>> new_connection = std::make_shared<Connection<T>>(Connection<T>::owner::server, m_AsioContext, std::move(socket), m_qMessagesIn);

                            if (on_client_connect(new_connection))
                            {
                                m_deqConnections.push_back(std::move(new_connection));

                                m_deqConnections.back()->connect_to_client(m_nIDCounter++);
                            }
                            else
                            {
                                // inform that client has been denied
                            }
                        }
                        else
                        {
                            // new connection error
                        }

                        wait_for_client_connection();
                    });
            }

            void message_client(std::shared_ptr<Connection<T>> client, const Message<T> msg)
            {
                if(client && client->is_connected())
                {
                    client->send(msg);
                }
                else
                {
                    on_client_disconnect(client);
                    client.reset();
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
                }
            }

            void message_all_clients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
            {
                bool invalid_client_exists = false;

                for(auto &client : m_deqConnections)
                {
                    if(client && client->is_connected())
                    {
                        if(client != pIgnoreClient)
                            client->send(msg);
                    }
                    else
                    {
                        on_client_disconnect(client);
                        client.reset();
                        invalid_client_exists = true;
                    }
                }

                if(invalid_client_exists)
                {
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
                }
            }

            void update(size_t nMaxMessages = -1)
            {
                size_t nMessageCount = 0;
                while(nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
                {
                    auto msg = m_qMessagesIn.pop_front();
                    on_message(msg.remote, msg.msg);
                    nMessageCount++;
                }
            }

        protected:
            virtual bool on_client_connect(std::shared_ptr<Connection<T>> client)
            {
                return false;
            }
            virtual void on_client_disconnect(std::shared_ptr<Connection<T>> client)
            {
            }
            virtual void on_message(std::shared_ptr<Connection<t>> client, Message<T> &msg)
            {
            }
            
        private:
            asio::io_context m_AsioContext;
            std::thread m_ContextThread;
            asio::ip::tcp::acceptor m_AsioAcceptor;

            TSQueue<OwnedMessage<T>> m_qMessagesIn;

            std::deque < std::shared_ptr<Connection<T>> m_deqConnections;

            uint32_t m_nIDCounter = 10000;
        };
    }
}