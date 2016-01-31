 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     SocketStarter.h 
Purpose:       interface definition for SocketStarter
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:12 PM
- End Header --------------------------------------------------------*/


#pragma once

#pragma comment(lib, "ws2_32.lib")

// this class will start wsa when constructed, and stop wsa when destructed
class SocketStarter
{
public:
    SocketStarter();
    ~SocketStarter();
};


