#ifndef NETWORK_H_9192016
#define NETWORK_H_9192016
#pragma once

#include <vector>
#include "basic_types.h"
#include "u16basic.h"

enum class packet_opcodes : uchar_t
{
    OP_NULL = 0x00,
    OP_LIST_DELETE = 0x08,
    OP_LOGIN = 0x10,
    OP_LIST_ADD = 0x20,
    OP_LIST_GET = 0x40,
    OP_LIST_SET = 0x80
};

struct network_packet
{
    packet_opcodes opcode;
    uchar_t subcode;
    std::vector<uchar_t> data;

    // is_valid variable should be used only during receiving packet
    bool is_valid;

    network_packet()
        :opcode(packet_opcodes::OP_NULL), data(), is_valid(true)
    {
    }
};

namespace packet_data_encoders
{
    // encode u16string to data for network_packet
    std::vector<uchar_t> encode(const std::u16string& str);

    // decode network_packet data to u16string
    std::u16string decode(const std::vector<uchar_t>& data);
}

#endif //NETWORK_H_9192016
