#pragma once
#include <string>

#if defined(_MSC_VER) && (_MSC_VER == 1900)
typedef unsigned short u16type;
#else
typedef std::u16string::value_type u16type;
#endif

std::string u16stringToBytes(const std::u16string& str);
std::u16string bytesToU16string(const std::string& str);
