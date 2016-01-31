 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCManager.cpp 
Purpose:       implementation for IRCManager
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:10 PM
- End Header --------------------------------------------------------*/


#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include "IRCConnection.h"
#include "IRCMessageHandler.h"
#include "IRCCommand.h"
#include "ConcreteIRCCommandHandlers.h"
#include "BattleshipMessageHandler.h"


bool IRCHandlePrint::operator()(const IRCCommand& cmd)
{
    std::cout << "---- " << cmd << "\n";
    return false;
}

bool IRCHandleWelcome::operator()(const IRCCommand& cmd)
{
    // when 375 encountered, enable this handler until 376 encountered
    // if not enabled, this operator is no-op
    // otherwise, prints only 372 message

    auto is_handled = false;

    if (!is_enabled && cmd.GetCommand() == "375")
    {
        is_enabled = true;
        is_handled = true;
    }

    if (is_enabled && cmd.GetCommand() == "376")
    {
        *connection_successful = true;
        is_enabled = false;
        is_handled = true;
    }

    if (is_enabled && cmd.GetCommand() == "372")
    {
        std::cout << cmd.GetTrailing() << "\n";
        is_handled = true;
    }

    return is_handled;
}

bool IRCHandlePing::operator()(const IRCCommand& cmd, IRCConnection& irc)
{
    if (cmd.GetCommand() == "PING")
    {
        IRCCommand command = cmd;
        command.ChangeCommand("PONG");
        irc.Send(command);
        return true;
    }

    return false;
}

bool JoinHandler::operator()(const IRCCommand& cmd, IRCConnection& irc)
{
    if (cmd.GetCommand() == "JOIN")
    {
        irc.AddUser(cmd.GetSource());
        return true;
    }
    return false;
}

bool IRCHandlePrivMsg::operator()(const IRCCommand& cmd)
{
    if (cmd.GetCommand() == "PRIVMSG")
    {
        // if from channel, use broadcast format
        // otherwise, use whisper format

        if (cmd.GetParameters()[0] == '#')
            std::cout << "[" << cmd.GetSource() << "] " << cmd.GetTrailing() << "\n";
        else
            std::cout << cmd.GetSource() << " whispers to you: " << cmd.GetTrailing() << "\n";

        return true;
    }
    return false;
}

bool IRCHandleBattleshipPrivMsg::operator()(const IRCCommand& cmd)
{
    if (cmd.GetCommand() == "PRIVMSG" && cmd.GetTrailing()[0] == '>')
    {
        // if from channel, ignored
        // otherwise, use whisper format
        auto command = cmd.GetTrailing();
        command = command.substr(1);

        std::stringstream ss(command);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(std::istream_iterator<std::string>(ss), end);
        if (vstrings.size() > 0U)
        {
            auto request = vstrings[0];
            vstrings.erase(vstrings.begin());

            warboat_handler->Handle(cmd.GetSource(), request, vstrings);
        }
    }
    return false;
}

IRCHandleBattleshipPrivMsg::IRCHandleBattleshipPrivMsg(BattleshipMessageHandler& warboat_handler)
    :warboat_handler(&warboat_handler)
{
}


bool IRCHandleQuit::operator()(const IRCCommand& cmd, IRCConnection& irc)
{
    if (cmd.GetCommand() == "QUIT")
    {
        irc.RemoveUser(cmd.GetSource());
        return true;
    }
    return false;
}

bool IRCHandleUserNameList::operator()(const IRCCommand& cmd, IRCConnection& irc)
{
    // if first 353 encountered, set sequence_begin to true until 366 encountered
    // when sequence_begin true and 353 received, parse and store user names

    bool is_handled = false;
    if (cmd.GetCommand() == "353")
    {
        // when first 353 encountered, clear user list
        if (!sequence_begin)
        {
            sequence_begin = true;
            irc.ClearUsers();
        }

        auto sstr = std::istringstream(cmd.GetTrailing());
        std::string user;
        while (sstr >> user)
        {
            // remove previlege indicator
            if (user[0] == '@' || user[0] == '!')
                user = user.substr(1);
            irc.AddUser(user);
        }
        is_handled = true;
    }
    else if (!sequence_begin && cmd.GetCommand() == "366")
    {
        sequence_begin = false;
        is_handled = true;
    }

    return is_handled;
}

bool IRCHandlePart::operator()(const IRCCommand& cmd, IRCConnection& irc)
{
    if (cmd.GetCommand() == "PART")
    {
        irc.RemoveUser(cmd.GetSource());
        return true;
    }
    return false;
}

