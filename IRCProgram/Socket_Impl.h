 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     Socket_Impl.h 
Purpose:       interface definition for SocketStarter_Impl
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:12 PM
- End Header --------------------------------------------------------*/


#pragma once

template<int N>
int Socket::Send(const std::array<char, N>& buffer, const Address& dest)
{
    return Send(buffer.data(), N, dest);
}

template<int N>
int Socket::Send(const char (&buffer)[N], const Address& dest)
{
    return Send(&buffer[0], N, dest);
}

template <int N>
int Socket::Receive(std::array<char, N>& buffer)
{
    return Receive(buffer.data(), N);
}

template <int N>
int Socket::Receive(char (&buffer)[N])
{
    return Receive(&buffer[0], N);
}

