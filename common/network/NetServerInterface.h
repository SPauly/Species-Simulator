/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9
	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 - 2020 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.
	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020
*/

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
        class Server_Interface // Server interface to inherate from when creating a server -> Abstracts the i/o level -> expects on_client_connect(), on_client_disconnect() and on_message() to be overloaded
        {
        public:
            Server_Interface(uint16_t port) : m_AsioAcceptor(m_AsioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) // port to open on
            {
            }
            virtual ~Server_Interface()
            {
                stop_server(); // try to stop the server before destroying object
            }

            bool start_server() // starts waiting for a client to connect, provides a thread for the asio context and primes it with work
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
                    std::cerr << "[SERVER] Exception: " << e.what() << '\n';
                    return false;
                }

                std::cout << "[SERVER] Started!\n";
                return true;
            }

            void stop_server()
            {
                m_AsioContext.stop();

                if (m_ContextThread.joinable()) // see if the thread can be joined
                    m_ContextThread.join();

                std::cout << "[SERVER] Stopped!\n";
            }

            void wait_for_client_connection()
            {
                m_AsioAcceptor.async_accept(
                    [this](std::error_code ec, asio::ip::tcp::socket socket)
                    {
                        if (!ec)
                        {
                            std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
                            std::shared_ptr<Connection<T>> new_connection = std::make_shared<Connection<T>>(Connection<T>::owner::server, m_AsioContext, std::move(socket), m_qMessagesIn); // create a new connection object via which we can talk to the client

                            if (on_client_connect(new_connection)) // give the user the possibility to decide about the connection
                            {
                                m_deqConnections.push_back(std::move(new_connection)); // add connection to dequeue

                                m_deqConnections.back()->connect_to_client(m_nIDCounter++); // try to connect to the client -> primes context with tasks of reading the header

                                std::cout << "[" << m_deqConnections.back()->get_uid() << "] Connection Approved\n";
                            }
                            else
                            {
                                std::cout << "[-----] Connection Denied\n";
                            }
                        }
                        else
                        {
                            std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                        }

                        wait_for_client_connection();
                    });
            }

            void message_client(std::shared_ptr<Connection<T>> client, const Message<T> &msg)
            {
                if (client && client->is_connected())
                {
                    client->send(msg);
                }
                else // client must have been disconnected already so remove him
                {
                    on_client_disconnect(client);
                    client.reset();
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
                }
            }

            void message_all_clients(const Message<T> &msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
            {
                bool invalid_client_exists = false;

                for (auto &client : m_deqConnections)
                {
                    if (client && client->is_connected())
                    {
                        if (client != pIgnoreClient)
                            client->send(msg);
                    }
                    else // client must be disconnected -> start removing
                    {
                        on_client_disconnect(client);
                        client.reset(); // no .erase yet -> else the queue could be compromised
                        invalid_client_exists = true;
                    }
                }

                if (invalid_client_exists) // erase removed clients
                {
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
                }
            }

            void update(size_t nMaxMessages = -1, bool bwait = false) // forwards all incomming messages via abstracted on_message function
            {
                if(bwait)
                    m_qMessagesIn.wait();
                size_t nMessageCount = 0;
                while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
                {
                    auto msg = m_qMessagesIn.pop_front();
                    on_message(msg.remote, msg.msg);
                    nMessageCount++;
                }
            }

        protected:
            virtual bool on_client_connect(std::shared_ptr<Connection<T>> client) // gets called when new client connects
            {
                return false;
            }
            virtual void on_client_disconnect(std::shared_ptr<Connection<T>> client) // gets called when client disconnects
            {
            }
            virtual void on_message(std::shared_ptr<Connection<T>> client, Message<T> &msg) // gets called when message arrives
            {
            }

        private:
            asio::io_context m_AsioContext;
            std::thread m_ContextThread;
            asio::ip::tcp::acceptor m_AsioAcceptor;

            TSQueue<OwnedMessage<T>> m_qMessagesIn;

            std::deque<std::shared_ptr<Connection<T>>> m_deqConnections;

            uint32_t m_nIDCounter = 10000;
        };
    }
}