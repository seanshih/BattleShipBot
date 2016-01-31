 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConcreteConsoleCommandHandlers.h 
Purpose:       interface definition for ConcreteConsoleCommandHandlers
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:45 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include "ConsoleCommand.h"
#include "IConsoleCommandHandler.h"


class IRCConnection;

// a base class for IRC related command handler
class ConsoleCommandHandlerIRC : public IConsoleCommandHandler
{
public:
    ConsoleCommandHandlerIRC(IRCConnection& irc, const std::string& pattern);
    virtual ~ConsoleCommandHandlerIRC();;

    bool operator()(const ConsoleCommand& command);

    virtual bool Handle(const std::string& content);

    IRCConnection& GetIRC();;

private:
    IRCConnection* irc;
    std::string pattern;
};

///////////////////////
// the following classes are concrete implementation of ConsoleCommandHandlerIRC
///////////////////////

// handle /quit
class ConsoleHandleQuit : public ConsoleCommandHandlerIRC
{
public:
    ConsoleHandleQuit(IRCConnection& irc, bool& program_terminated);

    bool Handle(const std::string&) override;

private:
    bool* _program_terminated;
};

// handle /join
class ConsoleHandleJoin : public ConsoleCommandHandlerIRC
{
public:
    ConsoleHandleJoin(IRCConnection& irc);

    bool Handle(const std::string& channel) override;
};

// handle invalid message
class ConsoleHandleInvalid : public IConsoleCommandHandler
{
public:
    virtual bool operator()(const ConsoleCommand& cmd)
    { 
      std::cout << '/' << cmd.GetCommand() << " is not a valid command" << std::endl;
      std::cout << "This irc client only supports /tell, /users, /part, /join and /quit" << std::endl;
      return true; 
    }
};


// handle /part
class ConsoleHandlePart : public ConsoleCommandHandlerIRC
{
public:
    ConsoleHandlePart(IRCConnection& irc);

    bool Handle(const std::string& channel) override;
};

// handle /users
class ConsoleHandleUsers : public ConsoleCommandHandlerIRC
{
public:
    ConsoleHandleUsers(IRCConnection& irc);
    bool Handle(const std::string& message) override;
};

// handle text message
class ConsoleHandleText : public ConsoleCommandHandlerIRC
{
public:
    ConsoleHandleText(IRCConnection& irc);
    bool Handle(const std::string& message) override;
};

// handle /tell
class ConsoleHandleTell : public ConsoleCommandHandlerIRC
{
public:
    ConsoleHandleTell(IRCConnection& irc);

    bool Handle(const std::string& message) override;
};



