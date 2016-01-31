 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConcreteIRCClient.cpp 
Purpose:       implementation for ConcreteIRCClient
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:42 AM
- End Header --------------------------------------------------------*/


#include <iostream>
#include <atomic>
#include "IRCclient.h"
#include "IRCConnection.h"
#include "ConsoleManager.h"
#include "ConcreteIRCClient.h"
#include "ConcreteConsoleCommandHandlers.h"
#include "ConcreteIRCCommandHandlers.h"
#include "BattleshipApp.h"
#include "BattleshipMessageHandler.h"

namespace
{
    // a concrete struct to compose IRCConnection's handlers
    struct ConcreteBattleshipBot : public IRCConnection
    {
        ConcreteBattleshipBot(unsigned num_max_game)
            :warboat_handler(*this, warboat_app, num_max_game)
        {
            AddHandler<IRCHandleWelcome>(connection_successful);
            AddHandler<IRCHandlePing>();
            AddHandler<IRCHandleUserNameList>();
            AddHandler<JoinHandler>();
            AddHandler<IRCHandlePart>();
            AddHandler<IRCHandleQuit>();
            AddHandler<IRCHandleBattleshipPrivMsg>(warboat_handler);
            AddHandler<IRCHandlePrivMsg>();
        }

        virtual bool ConnectionComplete(){ return connection_successful; }

        bool connection_successful = false;
        BattleshipApp warboat_app;
        BattleshipMessageHandler warboat_handler;
    };

    struct ConcreteConsoleManager : public ConsoleManager
    {
        ConcreteConsoleManager(IRCConnection& irc, bool& termination_toggle)
        {
            AddHandler<ConsoleHandleQuit>(irc, termination_toggle);
            AddHandler<ConsoleHandleTell>(irc);
            AddHandler<ConsoleHandleText>(irc);
            AddHandler<ConsoleHandleJoin>(irc);
            AddHandler<ConsoleHandleUsers>(irc);
            AddHandler<ConsoleHandlePart>(irc);
            AddHandler<ConsoleHandleInvalid>();
        }
    };
}


ConcreteIRCClient::ConcreteIRCClient(IRCInfo info)
    : _info(info)
{
}

ConcreteIRCClient::~ConcreteIRCClient()
{
}

int ConcreteIRCClient::Start()
{

    // concrete irc connection instance to handle connection and host message
    bool connection_successful = false;

    auto k_num_max_game = 1U;
    ConcreteBattleshipBot irc_connection(k_num_max_game);
    irc_connection.SetVerbose(_info.is_verbose, &std::cout);

    // termination toggle for console handlers to toggle
    bool termination_toggle = false;

    // concrete console manager instance to handle user input
    ConcreteConsoleManager console_manager(irc_connection, termination_toggle);

    // compose the client and start the session
    IRCClient irc_client(_info, irc_connection, console_manager, termination_toggle);

    irc_client.SetAutoScript({
        "/join #Battleship",
    });

    return irc_client.Start();

}


