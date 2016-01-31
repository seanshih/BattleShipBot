 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConsoleCommand.cpp 
Purpose:       implementation for ConsoleCommand
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:43 AM
- End Header --------------------------------------------------------*/


#include <regex>

#include "ConsoleCommand.h"

const std::string& ConsoleCommand::GetContent() const
{
    return _content;
}

const std::string& ConsoleCommand::GetCommand() const
{
    return _command;
}

bool ConsoleCommand::HasCommand()
{
    return _command.size() != 0;
}

ConsoleCommand::ConsoleCommand(const std::string& str)
{
    std::regex e("^(?:/(\\S*)\\s*)?(.*)$");
    std::smatch sm;
    std::regex_match(str.cbegin(), str.cend(), sm, e);
    _command = sm[1];
    _content = sm[2];
}
