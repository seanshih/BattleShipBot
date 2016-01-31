 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCArgumentParser.h 
Purpose:       interface definition for mygetopt
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:11 PM
- End Header --------------------------------------------------------*/


#include <tchar.h>
#include <string>

#include "IRCInfo.h"

// a struct for parsing irc client's command line arguments
struct IRCArgumentParser
{
    IRCArgumentParser(int argc, const _TCHAR* const* argv);
    ~IRCArgumentParser();;

public:
    IRCInfo GetInfo();

private:
    // copy from content to target
    void NameCopy(const _TCHAR* content, std::string& target);

    // parsed the host:port string
    void ParseHostName(const _TCHAR* content);

    // fill information to struct for output
    void FillInfo();

private:
    static const int k_default_irc_port = 6667;

private:
    int _argc;
    const _TCHAR* const* _argv;

    // if the command line argument has been parsed
    bool _is_parsed = false;

    IRCInfo _info;
};

