 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     Socket.cpp 
Purpose:       implementation for Socket
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:44 AM
- End Header --------------------------------------------------------*/


#include <string>
#include "Socket.h"

int Socket::Connect(const Address& addr)
{
    // connect to address, return error code if failed
    CheckValid();
    if (connect(_socket, addr, addr.GetSize()) == SOCKET_ERROR)
        return WSAGetLastError();
    else
        return k_success;
}

int Socket::Bind(const Address& addr)
{
    // bind to address, return error code if failed
    CheckValid();
    if (bind(_socket, addr, addr.GetSize()) == SOCKET_ERROR)
        return WSAGetLastError();
    else
        return k_success;
}

int Socket::Receive(char* buffer, int maxBytes, const Address& filterAddress)
{
    // blocking receive message, with filter address
    // non matching sender address's message will not be received
    auto size = filterAddress.GetSize();
    auto bytes = recvfrom(_socket, buffer, maxBytes, 0, filterAddress, &size);
    if (bytes == SOCKET_ERROR)
        bytes = k_failure;
    return bytes;
}

int Socket::Receive(char* buffer, int maxBytes)
{
    // default receive with no filtering address
    return Receive(buffer, maxBytes, Address());
}

int Socket::Send(const char* buffer, int bytes, const Address& dest)
{
    // send message to destination address
    // return fail code if failed
    CheckValid();

    auto result = sendto(_socket, buffer, bytes, 0, dest, dest.GetSize());
    if (result == SOCKET_ERROR)
        result = k_failure;

    return result;
}

int Socket::Send(const std::string& str, const Address& dest)
{
    // send string to destination address
    return Send(str.c_str(), str.length() + 1, dest);
}

int Socket::Send(const ICommand& command, const Address& dest)
{
    // send command to destination address
    return Send(command.GetData(), command.GetLength(), dest);
}


void Socket::CheckValid()
{
    if (_invalidated)
        throw("Trying to access invalidated socket");
}

Socket::~Socket()
{
    Release();
}

Socket::Socket(Socket&& socket) :_socket(socket._socket), _invalidated(socket._invalidated)
{
    socket._invalidated = true;
}

Socket::Socket(int protocol) : _invalidated(false)
{
    auto type = (protocol == IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM;
    _socket = socket(AF_INET, type, protocol);
}

Socket::Socket() 
    :Socket(IPPROTO_TCP)
{
}

void Socket::Release()
{
    if (!_invalidated)
        closesocket(_socket);
    _invalidated = true;
}


