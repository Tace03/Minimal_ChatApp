#pragma once

#include "net_common.h"

namespace tace{
namespace net{

/*

Message Header:
- The first part to be sent in a message
- Manage the types (ID) and the size of the message

*/
    template <typename T>
    struct message_header{
        T id{};
        uint32_t size = 0;
    };

/*

Message:
- A basic unit to transfer data
- Body to be extracted and feed to the communication

*/

    template <typename T>
    struct message{
        message_header<T> header{};
        std::vector<uint8_t> body;

        size_t size() const
        {
            return sizeof(message_header<T>) + body.size();
        }
    

    // Custom inputting method for our message
    friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
    {
        os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
        return os;
    }


    template <typename DataType>
    friend message<T>& operator << (message<T> &msg, const DataType& data)
    {
        // Check that the data type can be pushed into the vector
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        // Cache current size of the vector, which is where to insert the data
        size_t i = msg.body.size();
        
        //Resize the vector by the size of the data
        msg.body.resize(msg.body.size() + sizeof(DataType));

        // Physically copy the data into the newly allocated space
        std::memcpy(msg.body.data() + i , &data, sizeof(DataType));

        // Recalculate message size
        msg.header.size = msg.size();

        return msg;
    }

    template <typename DataType>
    friend message<T>& operator >> (message<T> &msg,DataType& data)
    {
        // Check that if the data type is ok to be popped out of vector
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be popped out of vector");

        // Cache the location of the popped data
        size_t i = msg.body.size() - sizeof(DataType);

        // Physically write the intended pop part to the designated
        std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

        // Reduce the size of the vector
        msg.body.resize(i);

        return msg;
    }

    };
}
}