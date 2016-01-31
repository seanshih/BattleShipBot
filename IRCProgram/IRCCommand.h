 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCCommand.h 
Purpose:       interface definition for IRCCommand
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:47 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include "ICommand.h"

/////////////////
// class IRCCommand is both message and command
// it uses the same logic to 
// 1. parse message from server
// 2. compose message to server
////////////////

class IRCCommand : public ICommand
{
public:
    // construct empty command
    IRCCommand();

    // construct with raw data
    IRCCommand(const std::string& full_line);

    // construct with prefix, command, and following parameters
    IRCCommand(const std::string& prefix, const std::string& command, const std::string& parameters);

    // construct without prefix
    IRCCommand(const std::string& command, const std::string& parameters);


    IRCCommand(IRCCommand&& rhs) = default;
    IRCCommand(const IRCCommand& rhs) = default;
    IRCCommand& operator=(const IRCCommand& rhs) = default;
    IRCCommand& operator=(IRCCommand&& rhs) = default;

    void ChangeCommand(const std::string& new_command);
    void RemovePrefix();

    // get the source from part of prefix between the beginning and the '!'
    std::string GetSource() const;

    // get the string after last " :"
    // if no trailing data, return empty string
    std::string GetTrailing() const;

    // if this is empty command
    explicit operator bool() const;

    // get length of data
    int GetLength() const override;

    // get the complete message composed of prefix, command and parameters
    const char* GetData() const override;

    // get data in string type
    const std::string& GetStringData() const;

    // get prefix part
    const std::string& GetPrefix()const;

    // get command part
    const std::string& GetCommand()const;

    // get parameters, including the trailing part
    const std::string& GetParameters()const;

private:
    // reconstruct the _data member
    void UpdateData();

private:
    std::string _data;       // the raw irc message string
    std::string _prefix;     // prefix part
    std::string _command;    // command part
    std::string _parameters; // parameters
};

// irc command stream output
inline std::ostream& operator << (std::ostream& ostr, const IRCCommand& cmd)
{
    const auto& str = cmd.GetStringData();
    ostr.write(str.c_str(), str.length() - 2);
    return ostr;
}



