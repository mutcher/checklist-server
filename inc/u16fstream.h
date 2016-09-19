#pragma once
#include <fstream>
#include "u16basic.h"


typedef std::basic_ofstream<u16type> basic_u16ofstream;
typedef std::basic_ifstream<u16type> basic_u16ifstream;

class u16ofstream : public basic_u16ofstream
{
public:
    u16ofstream(const char* filename)
        :basic_u16ofstream(filename, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary)
    {
    }

    void writeRecord(const std::pair<unsigned char, std::u16string>& item)
    {
        auto& stream = (*this);
        stream.put(item.first);
        auto bytes = u16stringToBytes(item.second);
        for(const auto& b : bytes)
        {
            stream.put(b);
        }
        stream.put(0x0D);
        stream.flush();
    }
};

class u16ifstream : public basic_u16ifstream
{
public:
    u16ifstream(const char* filename)
        :basic_u16ifstream(filename, std::ios_base::in | std::ios_base::binary)
    {
    }

    bool readRecord(std::pair<unsigned char, std::u16string>& item)
    {
        bool ret = false;
        item.first = 0;
        item.second.clear();
        
        std::string bytes;
        bool isIndexRead = false;
        while((*this))
        {
            const char c = static_cast<const char>(this->get());
            if (c == 0x0D)
            {
                break;
            }
            if (!isIndexRead)
            {
                item.first = static_cast<unsigned char>(c);
                isIndexRead = true;
                continue;
            }
            bytes += c;
        }
        if (!bytes.empty())
        {
            item.second = bytesToU16string(bytes);
            ret = true;
        }
        return ret;
    }
};
