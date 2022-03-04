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
            Connection(owner parent, asio::io_context context, asio::ip::tcp::socket socket, uint16_t port, TSQueue<OwnedMessage<T>> &qMessagesIn) // owner = server | client, context = needs an asio context to run in, takes over the socket and handles connection, port = port at which to connect, Queue of Messages comming in
                : m_context(context);
            m_socket(socket);
            m_qMessagesIn(m_qMessagesIn)
            {
                m_nowner = parent;
            }
            virtual ~Connection()
            {
            }

        public:
            bool connect_to_server(const asio::ip::tcp::resolver::results_type &endpoint)
            {
                if (m_nowner == owner::client)
                {
                    asio::async_connect(m_socket, endpoint,
                                        [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                                        {
                                            if (!ec)
                                            {
                                                mf_read_header();
                                            }
                                        });
                }
            }

            bool connect_to_client(uint32_t uid = 0)
            {
                if (m_nowner == owner::server)
                {
                    if (m_socket.is_open())
                    {
                        m_id = uid;
                        mf_read_header();
                    }
                }
            }

            void disconnect()
            {
                if (is_connected())
                {
                    asio::post(m_context,
                               [this]()
                               { m_socket.close(); });
                }
            }

            bool is_connected() const
            {
                return m_socket.is_open();
            }

            uint32_t get_uid() const
            {
                return m_id;
            }

        private:
            void mf_read_header()
            {
                asio::async_read(m_socket, asio::buffer(m_tempMessageIn.header, sizeof(MessageHeader<T>)),
                                 [this](std::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         if (m_tempMessagesIn.header.size > 0)
                                         {
                                             m_tempMessagesIn.body.resize(m_tempMessagesIn.header.size);
                                             mf_read_body();
                                         }
                                         else
                                         {
                                             mf_add_to_incomming_message_queue();
                                         }
                                     }
                                     else
                                     {
                                         // failed to read from socket
                                         m_socket.close();
                                     }
                                 });
            }

            void mf_read_body()
            {
                asio::async_read(m_socket, asio::buffer(m_tempMessageIn.body, m_tempMessageIn.body.size()),
                                 [this](std::error_code ec, size_t lenght)
                                 {
                                     if (!ec)
                                     {
                                         mf_add_to_incomming_message_queue();
                                     }
                                     else
                                     {
                                         // failed to read from socket so close it
                                         m_socket.close();
                                     }
                                 });
            }

            void mf_add_to_incomming_message_queue()
            {
                if (m_nowner == owner::server)
                {
                    m_qMessagesIn.push_back({this->shared_from_this(), m_tempMessageIn});
                }
                else
                {
                    m_qMessagesIn.push_back({nullptr, m_tempMessageIn});
                }

                mf_read_header();
            }

            

        protected:
            asio::io_context &m_context;    // does not own the context since it only runs in it
            asio::ip::tcp::socket m_socket; // socket it handles

            TSQueue<OwnedMessage<T>> &m_qMessagesIn;
            Message<T> m_tempMessageIn;
            TSQueue<Message<T>> m_qMessagesOut;

            owner m_nowner = owner::server;
            uint32_t m_id = 0; // unique user id
        };
    }
}
