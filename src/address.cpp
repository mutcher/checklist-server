#include "inc/address.h"
#include <sstream>

address::address()
   :m_address(0), m_port(0)
{
}

address::address(const uchar_t & a, const uchar_t & b, const uchar_t & c, const uchar_t & d, const uint16_t & port)
   :m_address(0), m_port(port)
{
   m_address = (a << 24) | (b << 16) | (c << 8) | d;
}

address::address(const uint32_t & addr, const uint16_t & port)
   :m_address(addr), m_port(port)
{
}

const uint32_t & address::getAddress() const
{
   return m_address;
}

const uint16_t & address::getPort() const
{
   return m_port;
}

std::string address::toString(const bool & includePort) const
{
   std::stringstream ss;
   uint16_t d = m_address >> 24;
   uint16_t c = (m_address - (d << 24)) >> 16;
   uint16_t b = (m_address - (d << 24) - (c << 16)) >> 8;
   uint16_t a = m_address - (d << 24) - (c << 16) - (b << 8);
   ss << a << '.' << b << '.' << c << '.' << d;
   return ss.str();
}
