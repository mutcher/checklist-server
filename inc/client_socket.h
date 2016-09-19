#ifndef CLIENT_SOCKET_H_9192016
#define CLIENT_SOCKET_H_9192016
#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include "address.h"
#include "network_packet.h"
#include "platform_socket.h"

namespace socket_system
{
    bool init();
    void shutdown();
}

class client_socket
{
private:
    static uchar_t min_packet_length;
    bool _good;
    

protected:
    uint64_t m_handle;
    address m_addr;

protected:
    uint64_t createSocket(int af, int type, int proto);

public:
    client_socket();
    client_socket(const address& addr);
    client_socket(const uint64_t& handle, const address& addr);

    void close();
    //int receive(char* buff, size_t offset, size_t length);
    //bool send(const std::vector<uchar_t>& packet);

    operator bool() const
    {
        return _good;
    }

    const bool& good() const
    {
        return _good;
    }

    client_socket& operator<<(const network_packet& packet)
    {
        // socket send data only in case if "good" flag is set
        if (_good)
        {
            size_t data_length = client_socket::min_packet_length;
            if (!packet.data.empty())
            {
                // one symbol for data size
                // plus data length
                data_length += packet.data.size() + 1;
            }
            std::vector<uchar_t> data(data_length);
            auto iter = data.begin();
            // packet signature
            *iter     = 0x05;
            *(++iter) = 0x05;
            if (packet.data.empty())
            {
                // this flag indicates that data section is empty
                *(++iter) = 0x03;
            }
            else
            {
                // this flag indicates that data section is not empty
                *(++iter) = 0xFF;
            }
            // packet opcode
            *(++iter) = static_cast<uchar_t>(packet.opcode);
            // packet subcode
            *(++iter) = packet.subcode;
            // data section
            if (!packet.data.empty())
            {
                *(++iter) = static_cast<uchar_t>(packet.data.size());
                std::copy(packet.data.cbegin(), packet.data.cend(), ++iter);
            }
            size_t sent_count = ::send(static_cast<SOCKET>(m_handle),
                                       reinterpret_cast<const char*>(data.data()),
                                       static_cast<int>(data.size()),
                                       0);
            if (sent_count != data_length)
            {
                _good = false;
            }
        }
        return *this;
    }

    client_socket& operator>>(network_packet& packet)
    {
        // socket receive data only in case if "good" flag is set
        if (_good)
        {
            std::vector<uchar_t> tmp(client_socket::min_packet_length);
            int receive_count = ::recv(static_cast<SOCKET>(m_handle),
                                       reinterpret_cast<char*>(tmp.data()),
                                       static_cast<int>(tmp.size()),
                                       0);
            if (receive_count < 0)
            {
                //we have some error so "good" flag should be unset
                _good = false;
            }
            else
            {
                // check is data section is empty
                bool read_data = tmp[2] == 0xFF;
                if (read_data)
                {
                    // data section not empty
                    // so data flag can be unset
                    tmp[2] = 0x03;
                }
                
                // need to validate signature
                for(int i = 0; i < 3; ++i)
                {
                    packet.is_valid = tmp[i] == ((i<2)? 0x05 : 0x03);
                    if (!packet.is_valid)
                    {
                        break;
                    }
                }

                // read data only in case if packet is valid
                if (packet.is_valid)
                {
                    packet.opcode = static_cast<packet_opcodes>(tmp[3]);
                    packet.subcode = tmp[4];
                    if (read_data)
                    {
                        uchar_t data_length = 0;
                        receive_count = ::recv(static_cast<SOCKET>(m_handle),
                                               reinterpret_cast<char*>(&data_length),
                                               sizeof(data_length),
                                               0);
                        if (receive_count > 0)
                        {
                            tmp.resize(data_length);
                            receive_count = ::recv(static_cast<SOCKET>(m_handle),
                                                   reinterpret_cast<char*>(tmp.data()),
                                                   static_cast<int>(tmp.size()),
                                                   0);
                            if (receive_count == data_length)
                            {
                                packet.data = tmp;
                            }
                            else
                            {
                                _good = false;
                            }
                        }
                        else
                        {
                            _good = false;
                        }
                    }
                }
            }
        }
        return *this;
    }

    const address& getAddress() const;
    const uint64_t& getID() const;
};
#endif //CLIENT_SOCKET_H_9192016
