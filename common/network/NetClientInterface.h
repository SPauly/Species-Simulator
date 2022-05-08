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

            void update(size_t nMaxMessages = -1, bool bwait = false) // forwards all incomming messages via abstracted on_message function
            {
                if(bwait)
                    m_qMessagesIn.wait();
                size_t nMessageCount = 0;
                while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
                {
                    auto msg = m_qMessagesIn.pop_front();
                    on_message(msg.msg);
                    nMessageCount++;
                }
            }
        
        protected:
            virtual void on_message(Message<T> &msg)
            {};

        protected:
            asio::io_context m_AsioContext;
            std::thread m_ContextThread;

            std::unique_ptr<Connection<T>> m_uptrConnection;

        private:
            TSQueue<OwnedMessage<T>> m_qMessagesIn;
        };
    }
}