 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     Socket.h 
Purpose:       interface definition for Socket
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:49 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include <array>
#include "ICommand.h"
#include "Address.h"

// socket class
class Socket 
{
public:
    const static auto k_success = 0;
    const static auto k_failure = -1;

public:
    // constructors
    // if not protocol given, tcp protocol will be used by default
    // socket will be acquired during constructor call
    Socket();
    Socket(int protocol); 
    Socket(Socket&& socket);
    Socket(Socket&) = delete;

    ~Socket();

    // is this socket released
    void CheckValid();

    // send char array buffer or command
    template<int N> int Send(const char (&buffer)[N], const Address& dest);
    template<int N> int Send(const std::array<char,N>& buffer, const Address& dest);
                    int Send(const char* buffer, int bytes, const Address& dest);
                    int Send(const std::string& str, const Address& dest);
                    int Send(const ICommand& command, const Address& dest);

    // receive data to char array buffer
    // these functions are blocking, in the main program they are called in a different thread               
    template <int N> int Receive(char (&buffer)[N]);
    template <int N> int Receive(std::array<char, N>& buffer);
                     int Receive(char* buffer, int maxBytes);
                     int Receive(char* buffer, int maxBytes, const Address& filterAddress);

    // bind address to this socket
    int Bind(const Address& addr);

    // connect to another address
    int Connect(const Address& addr);

    // release this socket, this will be called automatically when Socket destructed
    // but it could be manually released
    void Release();

private:
    // if this socket is already released
    bool _invalidated;
    SOCKET _socket;
};

#include "Socket_Impl.h"
