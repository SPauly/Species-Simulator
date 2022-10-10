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

namespace sim // global namespace for species simulator
{
    namespace net // namespace used to abstract all the network level
    {
        template <typename T> // T will be the custom message type the user can define
        struct MessageHeader  // Message Header contains the size of the message and the type of message send specified by the custom message type T
        {
            T id{};
            uint32_t receiver = 0;
            uint16_t request_id = 0;
            uint32_t size = 0;
        };

        template <typename T>
        struct Message // the actual message that gets send
        {
            MessageHeader<T> header{}; // Contains the type of message and it's size
            std::vector<uint8_t> body; // Contains the actual data of the message in 8 bit format -> serialized

            size_t size() const // returns the actual size of the entire message
            {
                return body.size();
            }

            void clear()
            {
                body.clear();
                header.size = 0;
                return;
            }

            friend std::ostream &operator<<(std::ostream &os, const Message<T> msg) // overload << operator info about message into output stream for std::cout<<msg;
            {
                os << "Message ID: " << int(msg.header.id) << " Message Size " << msg.header.size;
                return os;
            }

            template <typename DataType>
            friend Message<T> &operator<<(Message<T> &msg, const DataType &data) // overload operator so that we can simply push data into a message like so: msg << data1 << data2;
            {
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be handled by Message Object"); // check if data is too complex to be serialized

                size_t current_body_size = msg.body.size();

                msg.body.resize(current_body_size + sizeof(DataType));
                std::memcpy(msg.body.data() + current_body_size, &data, sizeof(DataType)); // copy all the data from data into the end of body

                msg.header.size = msg.size(); // update size of message

                return msg;
            }

            template <typename ComplexDataType>
            Message<T> &push_back_complex(Message<T> &msg, const ComplexDataType *data, const size_t size_of_data)
            {
                size_t current_body_size = msg.body.size();

                //compute amount of bytes to reserve for data
                size_t bytes = sizeof(ComplexDataType) * size_of_data;

                msg.body.resize(current_body_size + bytes);
                std::memcpy(msg.body.data() + current_body_size, data, bytes);

                msg.header.size = msg.size();

                return msg;
            }

            template <typename DataType>
            friend Message<T> &operator>>(Message<T> &msg, DataType &data) // overload >> operator to get Data out of Message Type like so: msg >> data2 >> data1;
            {
                static_assert(std::is_standard_layout<DataType>::value, "Datatype is too complex for message to be pushed into");

                size_t cached_location = msg.body.size() - sizeof(DataType); // location from where to start copying

                std::memcpy(&data, msg.body.data() + cached_location, sizeof(DataType)); // copy data into data

                msg.body.resize(cached_location); // resize vector

                msg.header.size = msg.size(); // update size in header

                return msg;
            }

            template <typename ComplexDataType>
            Message<T> &pull_complex(Message<T> &msg, ComplexDataType *data, const size_t size_of_data)
            {
                size_t cached_location = msg.body.size() - sizeof(ComplexDataType) * size_of_data;

                std::memcpy(data, msg.body.data() + cached_location, sizeof(ComplexDataType) * size_of_data);

                msg.body.resize(cached_location);

                msg.header.size = msg.size();

                return msg;
            }
        };

        // forward declaration of Connection object to create a Message that is owned by connection
        template <typename T>
        class Connection;

        template <typename T>
        struct OwnedMessage // Message that is owned by some connection
        {
            std::shared_ptr<Connection<T>> remote = nullptr;
            Message<T> msg{};

            // overload all the necessary operators
            friend std::ostream &operator<<(std::ostream &os, const OwnedMessage<T> msg)
            {
                os << msg.msg;
                return os;
            }
            
        };

    }
}