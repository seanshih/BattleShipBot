 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCClient.cpp 
Purpose:       implementation for IRCClient
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:43 AM
- End Header --------------------------------------------------------*/


#include "IRCClient.h"
#include "tstream.h"
#include "IRCConnection.h"

#include <atomic>
#include "ConsoleManager.h"
#include "ConcreteIRCCommandHandlers.h"
#include "ConcreteConsoleCommandHandlers.h"

IRCClient::IRCClient
(
    const IRCInfo& info, 
    IRCConnection& irc, 
    const ConsoleManager& console_manager, 
    const bool& termination_toggle
)
    :_info(info), 
     _irc(&irc), 
     _console_manager(&console_manager), 
     _termination_toggle(&termination_toggle)
{
}

IRCClient::~IRCClient()
{
}

void IRCClient::SetAutoScript(std::vector<std::string> list_of_command)
{
    _commands = list_of_command;
}

int IRCClient::Start()
{
    int result = 0;

    // start connection
    if (_info.IsDefaultPort())
        result = _irc->Connect(_info.host_ip);
    else
        result = _irc->Connect(_info.host_ip, _info.port);

    if (result != 0) return result;

    // login with irc info
    _irc->Login(_info.username, _info.password, _info.nickname, _info.realname);

    // start receiving session
    _irc->StartReceiving();

    while (!_irc->ConnectionComplete())
        continue;

    for (auto& command : _commands)
        _console_manager->Handle(command);

    // start user input loop until termination_toggle true
    do
    {
        std::string result;
        std::getline(std::cin, result);
        if (result.size() > 0U)
            _console_manager->Handle(result);
    } while (!*_termination_toggle);

    // stop the receiving thread
    _irc->StopReceiving();
    return result;
}


