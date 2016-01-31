 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCInfo.h 
Purpose:       interface definition for IRCInfo
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:48 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>

// A structure to store required irc arguments
struct IRCInfo
{
    bool  is_verbose;
    int   port; 
    std::string host_ip; 
    std::string nickname; 
    std::string username; 
    std::string realname; 
    std::string password; 

    // methods to set and get if using default port
    void UseDefaultPort(){ port = k_use_default_port; }
    bool IsDefaultPort(){ return port == k_use_default_port; }

private:
    const static auto k_use_default_port = -1;
};

