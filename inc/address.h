#pragma once
#include <cstdint>
#include <string>

class address
{
private:
   uint32_t m_address;
   uint16_t m_port;

public:
   typedef unsigned char uchar_t;

   address();
   address(const uchar_t& a, const uchar_t& b, const uchar_t& c, const uchar_t& d, const uint16_t& port);
   address(const uint32_t& addr, const uint16_t& port);

   const uint32_t& getAddress() const;
   const uint16_t& getPort() const;
   std::string toString(const bool& includePort = false) const;
};
