 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     Address.h 
Purpose:       interface definition for Address
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:45 AM
- End Header --------------------------------------------------------*/


#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <WinSock2.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

// address class
class Address 
{
public:

    // address constructor
    // it expects an ip string and a port value
    // if none given, it will have default address, which is null
    Address();
    Address(const char* ip, int port);
    ~Address();;

    // address comparison
    bool operator==(const Address& rhs_in) const;
    bool operator!=(const Address& rhs_in) const;

    // size of the address data
    int GetSize() const;

    // can be casted to pointer to address
    operator PSOCKADDR_IN () const;
    operator PSOCKADDR () const;

private:
    // internet socket
    SOCKADDR_IN _addr;
};



