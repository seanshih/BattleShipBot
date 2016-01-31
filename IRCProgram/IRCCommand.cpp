 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCCommand.cpp 
Purpose:       implementation for IRCCommand
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:44 AM
- End Header --------------------------------------------------------*/


#include <regex>
#include "IRCCommand.h"

const std::string& IRCCommand::GetParameters() const
{
    return _parameters;
}

const std::string& IRCCommand::GetCommand() const
{
    return _command;
}

const std::string& IRCCommand::GetPrefix() const
{
    return _prefix;
}

const char* IRCCommand::GetData() const
{
    return _data.data();
}

int IRCCommand::GetLength() const
{
    return _data.size();
}

IRCCommand::operator bool() const
{
    return _data.size() != 0;
}

std::string IRCCommand::GetTrailing() const
{
    // find the " :" pattern, if not found, return empty string
    // otherwise return trailing data after the colon

    std::string::size_type colon_pos = _parameters.length();
    do
        colon_pos = _parameters.rfind(':', colon_pos - 1);
    while
        (
            colon_pos != std::string::npos
            && colon_pos != 0
            && _parameters[colon_pos - 1] != ' '
        );
    

    std::string returned;
    if (colon_pos != std::string::npos)
        returned = std::string(_parameters.begin() + colon_pos + 1, _parameters.end());

    return returned;
}

std::string IRCCommand::GetSource() const
{
    auto colon_pos = _prefix.find('!');
    std::string returned;
    if (colon_pos != std::string::npos)
        returned = std::string(_prefix.begin(), _prefix.begin() + colon_pos);
    return returned;
}

void IRCCommand::RemovePrefix()
{
    // remove prefix from data
    if (_prefix.size() > 0)
    {
        _prefix.clear();
        UpdateData();
    }
}

void IRCCommand::ChangeCommand(const std::string& new_command)
{
    _command = new_command;
    UpdateData();
}

void IRCCommand::UpdateData()
{
    _data.clear();
    if (_prefix.size() > 0)
        _data = ":" + _prefix + " ";
    _data += _command + " " + _parameters + "\r\n";
}

IRCCommand::IRCCommand(const std::string& command, const std::string& parameters) :IRCCommand("", command, parameters)
{

}

IRCCommand::IRCCommand(const std::string& prefix, const std::string& command, const std::string& parameters) : _prefix(prefix),
_command(command),
_parameters(parameters)
{
    UpdateData();
}

IRCCommand::IRCCommand(const std::string& full_line)
{
    // parse the raw command into prefix, command and parameters
    std::regex e("^\\s*(?::(\\S+)\\s+)?(\\S+)\\s+(.*)$");
    std::smatch sm;
    std::regex_match(full_line.cbegin(), full_line.cend(), sm, e);
    _prefix = sm[1];
    _command = sm[2];
    _parameters = sm[3];
    UpdateData();
}

IRCCommand::IRCCommand()
{

}

const std::string& IRCCommand::GetStringData() const
{
    return _data;
}
