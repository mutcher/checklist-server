#include "inc/network_packet.h"

namespace packet_data_encoders
{
    std::vector<uchar_t> encode(const std::u16string& str)
    {
        std::vector<uchar_t> data(sizeof(uchar_t) + sizeof(u16type) * str.size());
        auto pos = data.begin();

        *(pos) = static_cast<uchar_t>(str.size() * sizeof(u16type));
        auto tmp = u16stringToBytes(str);
        std::copy(tmp.cbegin(), tmp.cend(), ++pos);

        return data;
    }

    std::u16string decode(const std::vector<uchar_t>& data)
    {
        std::string bytes(reinterpret_cast<const char*>(&data[1]), data.size() - 1);
        return bytesToU16string(bytes);
    }
}