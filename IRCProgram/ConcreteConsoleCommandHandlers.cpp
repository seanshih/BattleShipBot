 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConcreteConsoleCommandHandlers.cpp 
Purpose:       implementation for ConcreteConsoleCommandHandlers
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:42 AM
- End Header --------------------------------------------------------*/


#include <iostream>

#include "IRCConnection.h"
#include "ConsoleCommand.h"
#include "ConcreteConsoleCommandHandlers.h"

IRCConnection& ConsoleCommandHandlerIRC::GetIRC()
{
    return *irc;
}

bool ConsoleCommandHandlerIRC::Handle(const std::string& content)
{
    return false;
}

bool ConsoleCommandHandlerIRC::operator()(const ConsoleCommand& command)
{
    if (pattern == command.GetCommand())
        return Handle(command.GetContent());
    return false;
}

ConsoleCommandHandlerIRC::~ConsoleCommandHandlerIRC()
{

}

ConsoleCommandHandlerIRC::ConsoleCommandHandlerIRC(IRCConnection& irc, const std::string& pattern) : irc(&irc), pattern(pattern)
{

}

bool ConsoleHandleQuit::Handle(const std::string& message)
{
    GetIRC().Quit(message);
    *_program_terminated = true;
    return true;
}

ConsoleHandleQuit::ConsoleHandleQuit(IRCConnection& irc, bool& program_terminated) 
    : ConsoleCommandHandlerIRC(irc, "quit"), 
      _program_terminated(&program_terminated)
{
}

bool ConsoleHandleJoin::Handle(const std::string& channel_in)
{
    if (channel_in.size() > 0)
    {
        const std::string& channel = (channel_in[0] == '#') ? channel_in : ('#' + channel_in);
        GetIRC().Join(channel);
    }
    return true;
}

ConsoleHandleJoin::ConsoleHandleJoin(IRCConnection& irc) 
    : ConsoleCommandHandlerIRC(irc, "join")
{

}

bool ConsoleHandleTell::Handle(const std::string& message)
{
    auto username_tail = message.find_first_of(' ');
    if (username_tail == std::string::npos)
        return true;

    std::string username(message.begin(), message.begin() + username_tail);
    std::string text(message.begin() + username_tail + 1, message.end());

    GetIRC().PrivMsg(username, text);
    std::cout << "You whisper to " << username << ": " << text << "\n";

    return true;
}

ConsoleHandleTell::ConsoleHandleTell(IRCConnection& irc) 
    : ConsoleCommandHandlerIRC(irc, "tell")
{

}

bool ConsoleHandleText::Handle(const std::string& message)
{
    if (GetIRC().IsInChannel())
    {
        GetIRC().PrivMsg("", message);
        std::cout << "[you] " << message << "\n";
    }
    return true;
}

ConsoleHandleText::ConsoleHandleText(IRCConnection& irc) 
    : ConsoleCommandHandlerIRC(irc, "")
{

}

bool ConsoleHandleUsers::Handle(const std::string& message)
{
    if (GetIRC().GetCurrentChannel().size() == 0)
        std::cout << "Not in a channel!\n";
    else
    {
        std::cout << "Users in channel " << GetIRC().GetCurrentChannel() << ": [you]";
        auto count = GetIRC().GetUsers().size();
        std::cout << ((count == 0U) ? "\n" : ", ");

        for (const auto& user : GetIRC().GetUsers())
        {
            --count;
            const auto tail = (count > 0U ? ", " : "\n");
            std::cout << user << tail;
        }
    }
    return true;
}

ConsoleHandleUsers::ConsoleHandleUsers(IRCConnection& irc) 
    : ConsoleCommandHandlerIRC(irc, "users")
{

}

bool ConsoleHandlePart::Handle(const std::string& channel)
{
    GetIRC().Part();
    return true;
}

ConsoleHandlePart::ConsoleHandlePart(IRCConnection& irc) 
    : ConsoleCommandHandlerIRC(irc, "part")
{

}

