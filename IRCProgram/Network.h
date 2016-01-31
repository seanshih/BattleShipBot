 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     Network.h 
Purpose:       interface definition for Network
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:48 AM
- End Header --------------------------------------------------------*/


#pragma once

// a collection of network related headers

#include <WinSock2.h>

#include "SocketStarter.h"
#include "Socket.h"
#include "Address.h"
#include "ICommand.h"

#pragma comment(lib,"Ws2_32.lib")