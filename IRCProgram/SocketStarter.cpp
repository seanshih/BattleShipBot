 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction of this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     SocketStarter.cpp 
Purpose:       implementation for SocketStarter
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:11 PM
- End Header --------------------------------------------------------*/


#include <vector>
#include <memory>
#include <cstring>
#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <WinSock2.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

#include "SocketStarter.h"

#pragma 

// see header files for more usage instruction

SocketStarter::SocketStarter()
{
    WSAStartup(MAKEWORD(2, 2), &(WSADATA()));
}

SocketStarter::~SocketStarter()
{
    WSACleanup();
}

