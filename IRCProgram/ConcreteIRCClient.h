 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConcreteIRCClient.h 
Purpose:       interface definition for ConcreteIRCClient
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:45 AM
- End Header --------------------------------------------------------*/


#include "IRCinfo.h"

// a concrete class to compose irc client based on given info
class ConcreteIRCClient
{
public:

    // store irc info
    ConcreteIRCClient(IRCInfo info);
    ~ConcreteIRCClient();

    // start connection, receiving and sending message
    int Start();

private:
    IRCInfo _info;
};

