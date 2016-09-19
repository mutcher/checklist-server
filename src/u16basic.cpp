#include <codecvt>
#include <locale>
#include "inc/u16basic.h"

std::string u16stringToBytes(const std::u16string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
   auto p = reinterpret_cast<unsigned short const*>(str.data());
   auto out = std::wstring_convert<std::codecvt_utf16<unsigned short>, unsigned short>().to_bytes(p, p + str.size());
   return out;
#else
   std::wstring_convert<std::codecvt_utf16<u16type>, u16type> conv;
   return conv.to_bytes(str);
#endif
}

std::u16string bytesToU16string(const std::string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
   std::u16string out;
   auto s = std::wstring_convert<std::codecvt_utf16<unsigned short>, unsigned short>().from_bytes(str);
   auto p = reinterpret_cast<wchar_t const*>(s.data());
   out.assign(p, p + s.size());
   return out;
#else
   std::wstring_convert<std::codecvt_utf16<u16type>, u16type> conv;
   return conv.from_bytes(str);
#endif
}