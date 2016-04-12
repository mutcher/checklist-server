#pragma once
#include <string>
#include <fstream>
#include <codecvt>
#include <locale>

typedef std::u16string::value_type u16type;
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
        std::wstring_convert<std::codecvt_utf16<u16type>, u16type> conv;
        auto bytes = conv.to_bytes(item.second);
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
            std::wstring_convert<std::codecvt_utf16<u16type>, u16type> conv;
            item.second = conv.from_bytes(bytes);
            ret = true;
        }
        return ret;
    }
};
