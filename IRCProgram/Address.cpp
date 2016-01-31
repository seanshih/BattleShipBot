 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     Address.cpp 
Purpose:       implementation for Address
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:42 AM
- End Header --------------------------------------------------------*/


#include <cstring>
#include "Address.h"

Address::operator PSOCKADDR() const
{
    return (PSOCKADDR)&_addr;
}

Address::operator PSOCKADDR_IN() const
{
    return (PSOCKADDR_IN)&_addr;
}

int Address::GetSize() const
{
    return (int)sizeof(SOCKADDR_IN);
}

bool Address::operator!=(const Address& rhs_in) const
{
    return !(*this == rhs_in);
}

bool Address::operator==(const Address& rhs_in) const
{
    PSOCKADDR lhs = (PSOCKADDR)*this;
    PSOCKADDR rhs = (PSOCKADDR)rhs_in;
    return lhs->sa_family == rhs->sa_family
        && std::strncmp(lhs->sa_data, rhs->sa_data, sizeof(lhs->sa_data)) == 0;
}

Address::~Address()
{

}

Address::Address(const char* ip, int port)
{
    // initialize address data member
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.S_un.S_addr = (ip == nullptr) ?
    INADDR_ANY : inet_addr(ip);
}

Address::Address()
{
    std::memset(&_addr, 0, sizeof(_addr));
}


