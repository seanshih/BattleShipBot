 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConsoleCommand.h 
Purpose:       interface definition for ConsoleCommand
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:46 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>

///////////////
// a simple class to parse console input into Command and Content part
// e.g. /join RoomName 
// join will be the command
// and RoomName will be the content
//
// if slash not found, HasCommand() will return false 
///////////////

class ConsoleCommand
{
public:
    ConsoleCommand(const std::string& str);

    // is command given
    bool HasCommand();

    const std::string& GetCommand() const;
    const std::string& GetContent() const;
private:
    std::string _command; 
    std::string _content;
};



