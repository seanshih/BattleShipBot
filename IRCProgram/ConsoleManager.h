 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConsoleManager.h 
Purpose:       interface definition for ConsoleManager
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:11 PM
- End Header --------------------------------------------------------*/


#pragma once
#include <string>
#include <memory>
#include <vector>

class IConsoleCommandHandler;

/////////////
// a class to handle console command, 
// it is composed of concrete implementation of IConsoleCommandHandler
//
// it uses the chain of responsibility pattern
//
// if handler return true, 
// it indicates message is handled, and the following handlers will not receiving the message.
/////////////

class ConsoleManager
{
public:
    template<typename Handler_t, typename ...Args>
    void AddHandler(Args&&...);
    void AddHandler(std::unique_ptr<IConsoleCommandHandler> handler);

    void Handle(const std::string& input) const;
private:
    std::vector<std::unique_ptr<IConsoleCommandHandler>> _handlers;
};

#include "ConsoleManager_Impl.h"