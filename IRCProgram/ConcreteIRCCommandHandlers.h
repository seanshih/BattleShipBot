 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCManager.h 
Purpose:       interface definition for IRCManager
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:11 PM
- End Header --------------------------------------------------------*/


#pragma once

#include "IRCMessageHandler.h"

class IRCCommand; 
class IRCConnection;

// print whatever message handled and return false
class IRCHandlePrint : public IRCMessageHandler
{
    bool operator() (const IRCCommand& cmd) override;
};

// filter and print welcome message
// it handles all message between 375 and 376, and prints only 372
class IRCHandleWelcome: public IRCMessageHandler
{
public:
    IRCHandleWelcome(bool& connection_successful) 
        : connection_successful(&connection_successful)
    {}

    bool operator() (const IRCCommand& cmd) override;

private:
    bool is_enabled = false;
    bool* connection_successful;
};


// handle PING by sending PONG
class IRCHandlePing : public IRCMessageHandler
{
    bool operator() (const IRCCommand& cmd, IRCConnection& irc) override;
};

// handle JOIN and update user list
class JoinHandler : public IRCMessageHandler
{
public:
    bool operator() (const IRCCommand& cmd, IRCConnection& irc) override;
};

// handle PRIVMSG from either channel or user
class IRCHandlePrivMsg : public IRCMessageHandler
{
public:
    bool operator() (const IRCCommand& cmd) override;
};

// handle battleship PRIVMSG from either channel or user
class BattleshipMessageHandler;
class IRCHandleBattleshipPrivMsg : public IRCMessageHandler
{
public:
    IRCHandleBattleshipPrivMsg(BattleshipMessageHandler&);
    bool operator() (const IRCCommand& cmd) override;

private:
    BattleshipMessageHandler* warboat_handler;
};



// handle PART and remove from userlist
class IRCHandlePart : public IRCMessageHandler
{
public:
    bool operator() (const IRCCommand& cmd, IRCConnection& irc) override;
};

// handle QUIT and remove from user list
class IRCHandleQuit : public IRCMessageHandler
{
public:
    bool operator() (const IRCCommand& cmd, IRCConnection& irc) override;
};


// handle user name list
class IRCHandleUserNameList : public IRCMessageHandler
{
public:
    bool operator() (const IRCCommand& cmd, IRCConnection& irc) override;

private:
    bool sequence_begin = false;
};


