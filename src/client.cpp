#include <vector>
#include "inc/client.h"
#include "inc/packet.h"
#include <iostream>
#include <sstream>

client::client(socket& clientSocket, list_processor& processor, log_manager* log)
    :m_socket(clientSocket), m_processor(processor), m_log(log)
{
}

client::~client()
{
    m_socket.close();
}

bool client::init()
{
    bool ret = false;
    packetReceiver prec;
    if (prec.receivePacket(m_socket) && prec.isValidPacket() && prec.getOpcode() == opcode_t::OP_LOGIN)
    {
        std::u16string loginPassword = packetReceiver::getDataAsU16String(prec.getData(), prec.getDataSize());
        packet_helpers::loginPacket packetHelper;
        std::u16string::value_type exp[8] = {'t', 'e', 's', 't', ':', '1', '2', '3'};
        std::u16string tst(exp, 8);
        if (loginPassword == tst)
        {
            ret = true;
            packetHelper.setLoginStatus(static_cast<uchar_t>(m_socket.getID())); //we need some value which not equals to zero
        }
        else
        {
            ret = false;
            packetHelper.setLoginStatus(0); // login failed
        }
        packetHelper.build();
        const packetBuilder& builder = packetHelper.getBuilder();

        m_socket.send(reinterpret_cast<const char*>(builder.getBuffer()), builder.getBufferSize());
    }
    return ret;
}

void client::processClientRequests()
{
    packetReceiver prec;
    while(prec.receivePacket(m_socket))
    {
        if (!prec.isValidPacket())
        {
            break;
        }

        switch (prec.getOpcode())
        {
        case opcode_t::OP_LIST_ADD:
            {
                const uchar_t& listID = prec.getSubcode();
                std::u16string itemName = prec.getDataAsU16String(prec.getData(), prec.getDataSize());
                addListItem(listID, itemName);
            }
            break;
        case opcode_t::OP_LIST_GET:
            {
                const uchar_t& listID = prec.getSubcode();
                getList(listID);
            }
            break;
        case opcode_t::OP_LIST_DELETE:
            {
                const uchar_t& listID = prec.getSubcode();
                std::u16string listItemName = prec.getDataAsU16String(prec.getData(), prec.getDataSize());
                deleteListItem(listID, listItemName);
            }
            break;
        case opcode_t::OP_LIST_SET:
            break;
        case opcode_t::OP_LOGIN: //User already logged in. So this is strange behavior
        default:
            break;
        }
    }
}

void client::getList(const uchar_t& listID)
{
    auto list = m_processor.getList(listID);
    {
        std::stringstream ss;
        ss << "[" << m_socket.getID() << "]: getList<id=" << (int)listID << " list_empty=" << (list.empty()?'Y':'N') << ">";
        m_log->log(ss.str());
    }

    packet_helpers::getListPacket packetHelper;
    if (list.empty())
    {
        packetHelper.setEmptyList();
        packetHelper.build();
        const auto& builder = packetHelper.getBuilder();
        m_socket.send(reinterpret_cast<const char*>(builder.getBuffer()), builder.getBufferSize());
    }
    else
    {
        for(size_t i = list.size(); i > 0; i--)
        {
            packetHelper.setListID(static_cast<uchar_t>(i));
            packetHelper.setListItem(list[i-1]);
            packetHelper.build();
            const auto& builder = packetHelper.getBuilder();
            m_socket.send(reinterpret_cast<const char*>(builder.getBuffer()), builder.getBufferSize());
        }
    }
}

void client::addListItem(const uchar_t& listID, const std::u16string& itemName)
{
    packetBuilder builder;
    builder.setOpcode(opcode_t::OP_LIST_ADD);

    {
        std::stringstream ss;
        ss << "[" << m_socket.getID() << "]: addListItem<listID=" << (int)listID << ">";
        m_log->log(ss.str());
    }

    if (listID == 0)
    {
        uchar_t id = m_processor.createList(itemName);
        builder.setSubcode(id);
    }
    else
    {
        bool isAdded = m_processor.addListItem(listID, itemName);
        builder.setSubcode(isAdded? listID : 0x00);
    }

    builder.build();
    m_socket.send(reinterpret_cast<const char*>(builder.getBuffer()), builder.getBufferSize());
}

void client::deleteListItem(const uchar_t& listID, const std::u16string& itemName)
{
    packetBuilder builder;
    builder.setOpcode(opcode_t::OP_LIST_DELETE);

    {
        std::stringstream ss;
        ss << "[" << m_socket.getID() << "]: deleteListItem<listID=" << (int)listID << ">";
        m_log->log(ss.str());
    }

    if (listID == 0)
    {
        bool isDeleted = m_processor.deleteList(itemName);
        builder.setSubcode(isDeleted? 0x01 : 0x00);
    }
    else
    {
        bool isDeleted = m_processor.deleteListItem(listID, itemName);
        builder.setSubcode(isDeleted? listID : 0x00);
    }

    builder.build();
    m_socket.send(reinterpret_cast<const char*>(builder.getBuffer()), builder.getBufferSize());
}