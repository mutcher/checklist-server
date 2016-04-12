#pragma once
#include <string>
#include "socket.h"

typedef unsigned char uchar_t;

enum opcode_t : uchar_t
{
    OP_NULL = 0x00,
    OP_LIST_DELETE = 0x08,
    OP_LOGIN = 0x10,
    OP_LIST_ADD = 0x20,
    OP_LIST_GET = 0x40,
    OP_LIST_SET = 0x80
};

class packetBuilder
{
private:
    uchar_t m_opcode;
    uchar_t m_subcode;
    const uchar_t* m_data;
    size_t m_dataLength;

    uchar_t* m_buffer;
    size_t m_bufferLength;

private:
    void deleteBuffer();

public:
    packetBuilder();
    ~packetBuilder();

    void setOpcode(const uchar_t& opcode);
    void setSubcode(const uchar_t& subcode);
    void setData(const uchar_t* data, const size_t& size);

    void build();

    const uchar_t* getBuffer() const;
    const size_t& getBufferSize() const;

    static bool packU16String(const std::u16string& str, uchar_t* data, size_t size);
    static size_t getPackedStringSize(const std::u16string& str);
};

class packetReceiver
{
private:
    uchar_t* m_buffer;
    size_t m_dataSize;

public:
    packetReceiver();
    ~packetReceiver();

    bool receivePacket(socket& socket);
    bool isValidPacket() const;
    const opcode_t& getOpcode() const;
    const uchar_t& getSubcode() const;

    const uchar_t* getData() const;
    const size_t& getDataSize() const;

    static std::u16string getDataAsU16String(const uchar_t* data, const size_t& size);
};

namespace packet_helpers
{
    class loginPacket
    {
    private:
        packetBuilder m_builder;

    public:
        loginPacket();

        //if status equals to zero - login failed
        void setLoginStatus(const uchar_t& status);
        void build();

        const packetBuilder& getBuilder() const;
    };

    class getListPacket
    {
    private:
        std::u16string m_listItem;
        packetBuilder m_builder;

    public:
        getListPacket();

        void setListID(const uchar_t& listID);
        void setListItem(const std::u16string& item);
        void setEmptyList();
        
        void build();
        const packetBuilder& getBuilder() const;
    };
}