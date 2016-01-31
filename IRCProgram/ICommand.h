 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     SocketInterface.h 
Purpose:       interface definition for SocketInterface
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_1
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Wed, Feb 11, 2015  8:58:12 PM
- End Header --------------------------------------------------------*/


#pragma once

// command interface
// whatever class implemented this interface can be sent directly by socket's send method
class ICommand
{
public:
    virtual int GetLength() const = 0;
    virtual const char* GetData() const= 0;
};

