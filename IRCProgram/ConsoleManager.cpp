 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConsoleManager.cpp 
Purpose:       implementation for ConsoleManager
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:43 AM
- End Header --------------------------------------------------------*/


#include "ConsoleManager.h"
#include "ConsoleCommand.h"
#include "IConsoleCommandHandler.h"

void ConsoleManager::Handle(const std::string& input) const
{
    // handler return a bool value to return if message handled
    // if handled, break from the handling chain immediately
    for (auto& handler : _handlers)
    {
        bool is_handled = (*handler)(input);
        if (is_handled)
            break;
    }
}

void ConsoleManager::AddHandler(std::unique_ptr<IConsoleCommandHandler> handler)
{
    _handlers.emplace_back(std::move(handler));
}
