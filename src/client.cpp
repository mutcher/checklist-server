#include <vector>
#include "inc/client.h"
#include "inc/network_packet.h"
#include <iostream>
#include <sstream>

client::client(client_socket& clientSocket, list_processor& processor, log_manager* log)
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
    network_packet packet;
    m_socket >> packet;
    if (packet.is_valid &&
        m_socket.good() &&
        packet.opcode == packet_opcodes::OP_LOGIN)
    {
        std::u16string loginPassword = packet_data_encoders::decode(packet.data);
        std::u16string::value_type exp[8] = {'t', 'e', 's', 't', ':', '1', '2', '3'};
        std::u16string tst(exp, 8);

        packet.opcode = packet_opcodes::OP_LOGIN;
        packet.subcode = 0x00;
        ret = loginPassword == tst;
        if (ret)
        {
            packet.subcode = static_cast<uchar_t>(m_socket.getID()); //we need some value which not equals to zero
        }
        m_socket << packet;
    }
    return ret;
}

void client::processClientRequests()
{
    network_packet packet;
    while(true)
    {
        m_socket >> packet;
        if (!packet.is_valid || !m_socket.good())
        {
            break;
        }

        switch (packet.opcode)
        {
            case packet_opcodes::OP_LIST_ADD:
                addListItem(packet);
                break;
            case packet_opcodes::OP_LIST_GET:
                getList(packet);
                break;
            case packet_opcodes::OP_LIST_DELETE:
                deleteListItem(packet);
                break;
        case packet_opcodes::OP_LIST_SET:
            //NOT IMPLEMENTED
            break;
        case packet_opcodes::OP_LOGIN: //User already logged in. So this is strange behavior
        default:
            break;
        }
    }
}

void client::getList(network_packet packet)
{
    uchar_t listID = packet.subcode;
    packet.data.clear();
    
    auto list = m_processor.getList(listID);
    {
        std::stringstream ss;
        ss << "[" << m_socket.getID() << "]: getList<id=" << (int)listID << " list_empty=" << (list.empty()?'Y':'N') << ">";
        m_log->log(ss.str());
    }

    if (list.empty())
    {
        packet.subcode = 0x00;
        m_socket << packet;
    }
    else
    {
        for(size_t i = list.size(); i > 0; --i)
        {
            packet.subcode = static_cast<uchar_t>(i);
            packet.data = packet_data_encoders::encode(list[i-1]);
            m_socket << packet;
        }
    }
}

void client::addListItem(network_packet packet)
{
    uchar_t& listID = packet.subcode;
    std::u16string itemName = packet_data_encoders::decode(packet.data);
    packet.data.clear();
    
    {
        std::stringstream ss;
        ss << "[" << m_socket.getID() << "]: addListItem<listID=" << (int)listID << ">";
        m_log->log(ss.str());
    }

    if (listID == 0)
    {
        listID = m_processor.createList(itemName);
    }
    else
    {
        bool isAdded = m_processor.addListItem(listID, itemName);
        if (!isAdded)
        {
            packet.subcode = 0x00;
        }
    }

    m_socket << packet;
}

void client::deleteListItem(network_packet packet)
{
    uchar_t listID = packet.subcode;
    std::u16string listItemName = packet_data_encoders::decode(packet.data);
    packet.data.clear();

    {
        std::stringstream ss;
        ss << "[" << m_socket.getID() << "]: deleteListItem<listID=" << (int)listID << ">";
        m_log->log(ss.str());
    }

    if (listID == 0)
    {
        bool isDeleted = m_processor.deleteList(listItemName);
        packet.subcode = (isDeleted? 0x01 : 0x00);
    }
    else
    {
        bool isDeleted = m_processor.deleteListItem(listID, listItemName);
        packet.subcode = (isDeleted? listID : 0x00);
    }

    m_socket << packet;
}