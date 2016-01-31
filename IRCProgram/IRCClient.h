 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCClient.h 
Purpose:       interface definition for IRCClient
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:47 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <utility>
#include <atomic>
#include <vector>

#include "IRCInfo.h"

class IRCConnection;
class ConsoleManager;

/////////////////
// irc client composed of IRCConnection, ConsoleManager and termination toggle
// it uses IRCInfo to correctly setup and link IRCConnection and ConsoleManager
//
// IRCConnection is responsible for handling connection and host message
// ConsoleManager is responsible for handling user input
// termination_toggle is for stopping the user input loop
/////////////////
class IRCClient
{
public:
    // store components
    IRCClient(const IRCInfo&, IRCConnection&, const ConsoleManager&, const bool& termination_toggle);
    ~IRCClient();

    // start the irc connection session
    int Start();
    void SetAutoScript(std::vector<std::string> list_of_command);

private:
    const bool*           _termination_toggle;
    IRCConnection*        _irc;
    const ConsoleManager* _console_manager;

    std::vector<std::string> _commands;

    IRCInfo _info;
};

