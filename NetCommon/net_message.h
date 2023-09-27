#pragma once
#include "net_common.h"

namespace net
{
    template <typename T>
    struct message_header
    {
        T id{};
        uint32_t size =0;
    };
    template <typename T>
    struct message
    {
        // Header & Body vector
        message_header<T> header{};
        std::vector<uint8_t> body;

        // returns size of entire message packet in bytes
        size_t size() const
        {
            return body.size();
        }

        // Override for std::cout compatibility - produces friendly description of message
        friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
        {
            os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
            return os;
        }

        template<typename DataType>
        friend message<T>& operator << (message<T>& msg, const DataType& data)
        {
            static_assert(std::is_standard_layout_v<DataType>, "Data is too complex to be pushed into vector");
            size_t i = msg.body.size();
            msg.body.resize(msg.body.size() + sizeof(DataType));
            std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
            msg.header.size = msg.size();

            return msg;
        }

        template<typename DataType>
        friend message<T>& operator >> (message<T>& msg, DataType& data)
        {
            static_assert(std::is_standard_layout_v<DataType>, "Data is too complex to be pulled from vector");

            size_t i = msg.body.size() - sizeof(DataType);
            std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

            msg.body.resize(i);
            msg.header.size = msg.size();

            return msg;
        }
    };
    template <typename T>
    class connection;
    template <typename T>
    struct owned_message
    {
        std::shared_ptr<connection<T>> remote = nullptr;
        message<T> msg;

        friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg)
        {
            os << msg.msg;
            return os;
        }
    };
}