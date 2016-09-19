#include <cstring>
#include <sstream>
#include "inc/packet.h"
#include "inc/u16basic.h"

const uchar_t g_packet_signture[3] = { 0x05, 0x05, 0x03 };
const size_t g_max_packet_size = 256; //packet signature, opcode, subcode, data
const size_t g_min_packet_size = sizeof(g_packet_signture) + 2*sizeof(uchar_t); //packet signature, opcode, subcode

packet_helpers::loginPacket::loginPacket()
    :m_builder()
{
    m_builder.setOpcode(opcode_t::OP_LOGIN);
}

void packet_helpers::loginPacket::setLoginStatus(const uchar_t& status)
{
    m_builder.setSubcode(status);
}

void packet_helpers::loginPacket::build()
{
    m_builder.build();
}

const packetBuilder& packet_helpers::loginPacket::getBuilder() const
{
    return m_builder;
}

packet_helpers::getListPacket::getListPacket()
    :m_listItem(), m_builder()
{
    m_builder.setOpcode(opcode_t::OP_LIST_GET);
}

void packet_helpers::getListPacket::setListID(const uchar_t& listID)
{
    m_builder.setSubcode(listID);
}

void packet_helpers::getListPacket::setListItem(const std::u16string& item)
{
    m_listItem = item;
}

void packet_helpers::getListPacket::setEmptyList()
{
    m_builder.setSubcode(0);
    m_listItem = std::u16string();
}

void packet_helpers::getListPacket::build()
{
    if (!m_listItem.empty())
    {
        size_t dataSize = packetBuilder::getPackedStringSize(m_listItem);
        uchar_t* data = new uchar_t[dataSize];
        packetBuilder::packU16String(m_listItem, data, dataSize);
        m_builder.setData(data, dataSize);
        m_builder.build();
        delete[] data;
    }
    else
    {
        m_builder.setData(nullptr, 0);
        m_builder.build();
    }
}

const packetBuilder& packet_helpers::getListPacket::getBuilder() const
{
    return m_builder;
}

packetReceiver::packetReceiver()
    :m_buffer(new uchar_t[g_max_packet_size]), m_dataSize(0)
{
    std::memset(m_buffer, 0, sizeof(uchar_t) * g_max_packet_size);
}

packetReceiver::~packetReceiver()
{
    delete[] m_buffer;
}

bool packetReceiver::receivePacket(socket& socket)
{
    bool ret = false;
    std::memset(m_buffer, 0, sizeof(uchar_t) * g_max_packet_size);
    int count = socket.receive(reinterpret_cast<char*>(m_buffer), 0, g_min_packet_size);
    if (count == g_min_packet_size)
    {
        if (m_buffer[2] == 0xFF)
        {
            m_buffer[2] = 0x03;
            count += socket.receive(reinterpret_cast<char*>(m_buffer), count, 1);
            m_dataSize = m_buffer[5];
            count += socket.receive(reinterpret_cast<char*>(m_buffer), 6, m_dataSize);
        }

        ret = true;
    }
    return ret;
}

bool packetReceiver::isValidPacket() const
{
    return std::memcmp(m_buffer, g_packet_signture, sizeof(g_packet_signture)) == 0;
}

const opcode_t& packetReceiver::getOpcode() const
{
    size_t opcodePos = sizeof(g_packet_signture); //next after packet signature
    return reinterpret_cast<opcode_t&>(m_buffer[opcodePos]);
}

const uchar_t& packetReceiver::getSubcode() const
{
    size_t subcodePos = sizeof(g_packet_signture) + 1; //next after packet opcode
    return m_buffer[subcodePos];
}

const uchar_t* packetReceiver::getData() const
{
    uchar_t* data = nullptr;
    if (m_dataSize > 0)
    {
        size_t dataPos = g_min_packet_size + 1;
        data = &m_buffer[dataPos];
    }
    return data;
}

const size_t& packetReceiver::getDataSize() const
{
    return m_dataSize;
}

std::u16string packetReceiver::getDataAsU16String(const uchar_t* data, const size_t& size)
{
    std::string bytes(reinterpret_cast<const char*>(data), size);
    return bytesToU16string(bytes);
}

void packetBuilder::deleteBuffer()
{
    if (m_buffer != nullptr)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_bufferLength = 0;
    }
}

packetBuilder::packetBuilder()
    :m_opcode(0x00), m_subcode(0x00), m_data(nullptr), m_dataLength(0), m_buffer(nullptr), m_bufferLength(0)
{
}

packetBuilder::~packetBuilder()
{
    deleteBuffer();
}

void packetBuilder::setOpcode(const uchar_t& opcode)
{
    m_opcode = opcode;
}

void packetBuilder::setSubcode(const uchar_t& subcode)
{
    m_subcode = subcode;
}

void packetBuilder::setData(const uchar_t* data, const size_t& size)
{
    m_data = data;
    m_dataLength = size;
}

void packetBuilder::build()
{
    deleteBuffer();
    m_bufferLength = g_min_packet_size;
    if (m_data != nullptr)
    {
        m_bufferLength += m_dataLength;
    }
    m_buffer = new uchar_t[m_bufferLength];
    size_t pos = sizeof(g_packet_signture);
    std::memcpy(m_buffer, g_packet_signture, pos);
    m_buffer[pos++] = m_opcode;
    m_buffer[pos++] = m_subcode;
    if (m_data != nullptr)
    {
        m_buffer[2] = 0xFF; //this flag meant that data section is not empty
        std::memcpy(&m_buffer[pos], m_data, m_dataLength);
    }
}

const uchar_t* packetBuilder::getBuffer() const
{
    return m_buffer;
}

const size_t& packetBuilder::getBufferSize() const
{
    return m_bufferLength;
}

bool packetBuilder::packU16String(const std::u16string& str, uchar_t* data, size_t size)
{
    bool bret = false;
    if (size >= getPackedStringSize(str))
    {
        data[0] = static_cast<uchar_t>(str.size() * sizeof(u16type));
        auto arr = u16stringToBytes(str);
        std::memcpy(&data[1], arr.data(), data[0]);
    }
    return bret;
}

size_t packetBuilder::getPackedStringSize(const std::u16string& str)
{
    return sizeof(uchar_t) + sizeof(u16type) * str.size();
}
