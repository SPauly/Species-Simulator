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

namespace sim
{
    namespace net
    {
        template <typename T> //forward declare Server_Interface so I can call on_client_validated()
        class Server_Interface;

        template <typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>> // Object to handle connections between server and client -> abstracts all the communication related stuff
        {
        public:
            enum class owner
            {
                server,
                client
            };

        public:
            Connection(owner parent, asio::io_context& context, asio::ip::tcp::socket socket, TSQueue<OwnedMessage<T>> &qMessagesIn) // owner = server | client, context = needs an asio context to run in, takes over the socket and handles connection, port = port at which to connect, Queue of Messages comming in
                : m_context(context),
                  m_socket(std::move(socket)),
                  m_qMessagesIn(qMessagesIn)
            {
                m_nowner = parent;

                //do a handshake with client first
                if(m_nowner == owner::server)
                {
                    m_nHandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
                    m_nHandshakeCheck = mf_scramble(m_nHandshakeOut);

                }
                else
                {
                    m_nHandshakeIn = 0;
                    m_nHandshakeOut = 0;
                }
            }
            virtual ~Connection()
            {
            }

        public:
            void connect_to_server(const asio::ip::tcp::resolver::results_type &endpoint) // function only used by clients to connect to the server provided via an endpoint
            {
                if (m_nowner == owner::client)
                {
                    asio::async_connect(m_socket, endpoint,
                                        [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                                        {
                                            if (!ec)
                                            {
                                                mf_read_validation();// prime the asio context with reading a validation
                                            }
                                        });
                }
            }

            void connect_to_client(sim::net::Server_Interface<T>* server, uint32_t uid = 0) // function only used by the server to connect to a client giving him the provided uid
            {
                if (m_nowner == owner::server)
                {
                    if (m_socket.is_open()) // check if the connection to client has already been established -> done by client
                    {
                        m_id = uid;       // this connection now has a unique id
                        
                        mf_write_validation(); // prime the asio context write a validation to the client
                        mf_read_validation(server); //wait for client response
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

            void send(const Message<T> &msg) // primes the asio context with a new task of writing the provided message
            {
                asio::post(m_context,
                           [this, msg]()
                           {
                               bool b_is_already_writing_messages = !m_qMessagesOut.empty(); // if this flag is set it means the asio context is already writing all the messages contained in m_qMessagesOut
                               m_qMessagesOut.push_back(msg);                                // add to outgoing queue
                               if (!b_is_already_writing_messages)
                               {
                                   mf_write_header();
                               }
                           });
            }

        private:

            void mf_read_validation(sim::net::Server_Interface<T>* server = nullptr) //reads the 8 bytes of validation in 
            {
                asio::async_read(m_socket, asio::buffer(&m_nHandshakeIn, sizeof(uint64_t)),
                    [this, server](std::error_code ec, size_t length)
                    {   
                        if(!ec)
                        {
                            if(m_nowner == owner::server)
                            {
                                if(m_nHandshakeIn == m_nHandshakeCheck)
                                {
                                    std::cout << "Client Validated" << std::endl;
                                    server->on_client_validated(this->shared_from_this()); //call a function (overloadable) from the server_interface on what to do when a client connects

                                    mf_read_header(); //prime the asio context with the usual work
                                }
                                else
                                {
                                    std::cout << "Client Disconnected (Fail Validation)" <<std::endl;
                                    m_socket.close();
                                }

                            }
                            else
                            {
                                m_nHandshakeOut = mf_scramble(m_nHandshakeIn);
                                mf_write_validation();
                            }
                        }
                        else
                        {
                            std::cout << "Client disconnected (ReadValidation)" <<std::endl;
                            m_socket.close();
                        }
                    }); 
            }

            void mf_write_validation() //write validation in m_nHandShakeOut to socket  
            {
                asio::async_write(m_socket, asio::buffer(&m_nHandshakeOut, sizeof(uint64_t)),
                    [this](std::error_code ec, size_t length)
                    {
                        if(!ec)
                        {
                            if(m_nowner == owner::client)
                                mf_read_header();
                        }
                        else
                        {
                            m_socket.close();
                        }
                    });
            }      

            void mf_read_header() // automaticaly calls read body function or calls itself again if there is no body to read
            {
                asio::async_read(m_socket, asio::buffer(&m_tempMessageIn.header, sizeof(MessageHeader<T>)),
                                 [this](std::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         if (m_tempMessageIn.header.size > 0)
                                         {
                                             m_tempMessageIn.body.resize(m_tempMessageIn.header.size); // allocate the space for the body
                                             mf_read_body();                                           // prime context to read the body
                                         }
                                         else
                                         {
                                             mf_add_to_incomming_message_queue(); // message has no body so just add it to the queue already -> calls mf_read_header() again
                                         }
                                     }
                                     else
                                     {
                                         std::cout << "[" << m_id << "] Read Header Fail.\n";
                                         m_socket.close();
                                     }
                                 });
            }

            void mf_read_body() // reads the body of the messages already stored in tempMessage -> only called by mf_read_header()
            {
                asio::async_read(m_socket, asio::buffer(m_tempMessageIn.body.data(), m_tempMessageIn.body.size()),
                                 [this](std::error_code ec, std::size_t lenght)
                                 {
                                     if (!ec)
                                     {
                                         mf_add_to_incomming_message_queue(); // add message to m_qMessagesIn
                                     }
                                     else
                                     {
                                         std::cout << "[" << m_id << "] Read Body Fail.\n";
                                         m_socket.close();
                                     }
                                 });
            }

            void mf_add_to_incomming_message_queue() // adds message in m_tempMessageIn to incomming message stream
            {
                if (m_nowner == owner::server) // if the server owns the connection object make him own the message
                {
                    m_qMessagesIn.push_back({this->shared_from_this(), m_tempMessageIn}); // push back new Owned_Message primed to the server
                }
                else
                {
                    m_qMessagesIn.push_back({nullptr, m_tempMessageIn});
                }

                mf_read_header(); // prime asio context with further reading work
            }

            void mf_write_header() // writes the first header in outgoing message queue to the socket and calls mf_write_body() function -> starts ones the first message is pushed into the queue by send()
            {
                asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(MessageHeader<T>)),
                                  [this](std::error_code ec, std::size_t length)
                                  {
                                      if (!ec)
                                      {
                                          if (m_qMessagesOut.front().body.size() > 0) // see if we have to write a body
                                          {
                                              mf_write_body(); // calls mf_write_header() again if there are more messages so the context has a workload
                                          }
                                          else
                                          {
                                              m_qMessagesOut.pop_front(); // means only consisted of a header so remove the message from the queue
                                              if (!m_qMessagesOut.empty())
                                              {
                                                  mf_write_header(); // write until queue is empty
                                              }
                                          }
                                      }
                                      else
                                      {
                                          std::cout << "[" << m_id << "] Write Header Fail.\n";
                                          m_socket.close();
                                      }
                                  });
            }

            void mf_write_body() // writes the body of the front message in the outgoing queue to the socket -> called by mf_write_header() -> calls mf_write_header again if there are still messages left
            {
                asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
                                  [this](std::error_code ec, std::size_t length)
                                  {
                                      if (!ec)
                                      {
                                          m_qMessagesOut.pop_front(); // message has successfully been transmitted we can remove the message from the queue and work on the next one
                                          if (!m_qMessagesOut.empty())
                                          {
                                              mf_write_header();
                                          }
                                      }
                                      else
                                      {
                                          std::cout << "[" << m_id << "] Write Body Fail.\n";
                                          m_socket.close();
                                      }
                                  });
            }

            uint64_t mf_scramble(uint64_t nInput) //scramble the data nInput -> later must be a different one
            {
                uint64_t out = nInput ^ 0xDEADBEEFC0DECAFE;
                out = (out & 0xF0F0F0F0F0F0F00F) >> 4 | (out & 0xF0F0F0F0F0F0F000) << 4;
                return out ^ 0xC0DEFACE12345678;
            }

        protected:
            asio::io_context &m_context;    // does not own the context since it only runs in it
            asio::ip::tcp::socket m_socket; // socket it handles

            TSQueue<OwnedMessage<T>> &m_qMessagesIn;
            Message<T> m_tempMessageIn;
            TSQueue<Message<T>> m_qMessagesOut;

            owner m_nowner = owner::server;
            uint32_t m_id = 0; // unique user id

            //8 byte variables used for Handshake
            uint64_t m_nHandshakeOut = 0;
            uint64_t m_nHandshakeIn = 0;
            uint64_t m_nHandshakeCheck = 0; 
        };
    }
}
