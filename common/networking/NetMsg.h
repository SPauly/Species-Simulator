#pragma once
#include "CommonNetInclude.h"

namespace sim // global namespace for species simulator
{
    namespace net // namespace used to abstract all the network level
    {
        template <typename T> // T will be the custom message type the user can define
        struct MessageHeader  // Message Header contains the size of the message and the type of message send specified by the custom message type T
        {
            T id{};
            uint32_t size = 0;
        };

        template <typename T>
        struct Message // the actual message that gets send
        {
            MessageHeader<T> header{}; // Contains the type of message and it's size
            std::vector<uint8_t> body; // Contains the actual data of the message in 8 bit format -> serialized

            size_t size() const // returns the actual size of the entire message
            {
                return sizeof(MessageHeader<T>) + body.size();
            }

            friend std::ostream &operator<<(std::ostream &os, const Message<T> msg) // overload << operator info about message into output stream for std::cout<<msg;
            {
                os << "Message ID: " << (int)msg.header.id << " Message Size " << msg.size();
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
        };

        // forward declaration of Connection object to create a Message that is owned by connection
        template <typename T>
        class Connection;

        template <typename T>
        struct OwnedMessage // Message that is owned by some connection
        {
            std::shared_ptr<Connection<T>> owner = nullptr;
            Message<T> msg{};

            // overload all the necessary operators
            friend std::ostream &operator<<(std::ostream &os, const OwnedMessage<T> msg)
            {
                os << msg.msg;
                return os;
            }

            template <typename DataType>
            friend OwnedMessage<T> &operator<<(OwnedMessage<T> &msg, const DataType &data)
            {
                msg.msg << data;
                return msg;
            }

            template <typename DataType>
            friend OwnedMessage<T> &operator>>(OwnedMessage<T> &msg, const DataType &data)
            {
                msg.msg >> data;
                return msg;
            }
        };

    }
}