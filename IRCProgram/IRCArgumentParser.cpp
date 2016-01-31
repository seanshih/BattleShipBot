 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCArgumentParser.cpp 
Purpose:       implementation for IRCArgumentParser
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:43 AM
- End Header --------------------------------------------------------*/


#include "IRCArgumentParser.h"

void IRCArgumentParser::ParseHostName(const _TCHAR* content)
{
    // if port not given, use default port, otherwise use given port
    const auto colon_ptr = _tcsrchr(content, _T(':'));
    if (colon_ptr != nullptr)
    {
        const auto colon_pos = colon_ptr - content;
        _info.host_ip = std::string(content, content + colon_pos);
        _info.port = _tstoi(content + colon_pos + 1);
    }
    else
    {
        _info.host_ip = std::string(content, content + _tcslen(content));
        _info.port = k_default_irc_port;
    }

}

void IRCArgumentParser::NameCopy(const _TCHAR* content, std::string& target)
{
    target = std::string(content, content + _tcslen(content));
}

IRCArgumentParser::~IRCArgumentParser()
{

}

IRCArgumentParser::IRCArgumentParser(int argc, const _TCHAR* const* argv)
    :_argc(argc), _argv(argv)
{
    _info.port = k_default_irc_port;
}

IRCInfo IRCArgumentParser::GetInfo()
{
    // parse info once and cache it
    if (!_is_parsed)
    {
        FillInfo();
        _is_parsed = true;
    }

    return _info;
}

void IRCArgumentParser::FillInfo()
{
    // fill in info based on option
    _info.is_verbose = false;
    for (auto iarg = 1; iarg < _argc; iarg += 2)
    {
        const _TCHAR* content = nullptr;
        if (iarg + 1 < _argc)
            content = _argv[iarg + 1];

        switch (_argv[iarg][1])
        {
        case _T('h'):  // host ip:port
            ParseHostName(content); 
            break;
        case _T('u'):  // username and nickname
            NameCopy(content, _info.username); 
            NameCopy(content, _info.nickname); 
            break;
        case _T('n'):  // real name
            NameCopy(content, _info.realname); 
            break;
        case _T('p'):  // password
            NameCopy(content, _info.password); 
            break;
        case _T('v'): // verbose
            iarg -= 1;
            _info.is_verbose = true;
            break;
        }
    }
}
