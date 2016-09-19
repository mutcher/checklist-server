#include "inc/network_packet.h"

namespace packet_data_encoders
{
    std::vector<uchar_t> encode(const std::u16string& str)
    {
        std::vector<uchar_t> data;
        std::string tmp = u16stringToBytes(str);
        data.resize(tmp.size());
        std::copy(tmp.cbegin(), tmp.cend(), data.begin());

        return data;
    }

    std::u16string decode(const std::vector<uchar_t>& data)
    {
        std::string bytes(reinterpret_cast<const char*>(data.data()), data.size());
        return bytesToU16string(bytes);
    }
}