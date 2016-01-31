 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     main.cpp 
Purpose:       implementation for main
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:10 PM
- End Header --------------------------------------------------------*/

#include "IRCArgumentParser.h"
#include "SocketStarter.h"
#include "ConcreteIRCClient.h"
#include "BattleshipApp.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    // start the socket library
    SocketStarter starter;

    //// start the irc client
    auto irc_info = IRCArgumentParser(argc, argv).GetInfo();
    int result = ConcreteIRCClient(irc_info).Start();
    if (result != 0)
    {
      std::cout << "Connection Failure. Press any Enter to quit." << std::endl;
      std::cin.get();
    }

    return 0;
}