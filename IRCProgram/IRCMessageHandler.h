 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCMessageHandler.h 
Purpose:       interface definition for IRCMessageHandler
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:48 AM
- End Header --------------------------------------------------------*/


#pragma once

class IRCCommand;
class IRCConnection;

// interface for irc message handler
class IRCMessageHandler
{
public:
    IRCMessageHandler(){};
    virtual ~IRCMessageHandler(){};

    // if return true, it means message handled
    virtual bool operator() (const IRCCommand&) { return false; };

    // irc connection will be provided if this is overridden
    // otherwise it acts as single argument version operator()
    virtual bool operator() (const IRCCommand& command, IRCConnection&) { return operator()(command); };
};


