 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IConsoleCommandHandler.h 
Purpose:       interface definition for IConsoleCommandHandler
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:46 AM
- End Header --------------------------------------------------------*/


#pragma once

class ConsoleCommand;

// interface for console command handler
class IConsoleCommandHandler
{
public:
    // if return true, it means message handled
    virtual bool operator()(const ConsoleCommand&){ return false; }
    virtual ~IConsoleCommandHandler(){};
};


