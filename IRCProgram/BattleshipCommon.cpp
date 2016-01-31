 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipCommon.cpp 
Purpose:       implementation for BattleshipCommon
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:06 PM
- End Header --------------------------------------------------------*/


#include <cassert>
#include <cctype>
#include <unordered_map>

#include "BattleshipCommon.h"

unsigned ShipToChar(ShipType ship)
{
    switch (ship)
    {
    case ShipType::DESTROYER: return 'd';
    case ShipType::SUBMARINE: return 's';
    case ShipType::CRUISER:   return 'r';
    case ShipType::BATTLESHIP:return 'b';
    case ShipType::CARRIER:   return 'c';
    }

    assert(0);
    return ~0U;
}

unsigned ShipToLen(ShipType ship)
{
    switch (ship)
    {
    case ShipType::DESTROYER: return 2;
    case ShipType::SUBMARINE: return 3;
    case ShipType::CRUISER:   return 3;
    case ShipType::BATTLESHIP:return 4;
    case ShipType::CARRIER:   return 5;
    }

    assert(0);
    return ~0U;
}

ShipType ShipNameToShipType(const std::string& name)
{
    const static std::unordered_map<std::string, ShipType> map =
    {
        { "C",   ShipType::CARRIER },
        { "B",   ShipType::BATTLESHIP },
        { "R",   ShipType::CRUISER },
        { "S",   ShipType::SUBMARINE },
        { "D",   ShipType::DESTROYER },
    };


    auto found = map.find(ToUpper(name));
    if (found == map.end())
        return ShipType::INVALID;
    else
        return found->second;
}

Direction DirectionNameToDirection(const std::string& name)
{
    const static std::unordered_map<std::string, Direction> map =
    {
        { "N", Direction::N },
        { "E", Direction::E },
        { "S", Direction::S },
        { "W", Direction::W },
    };

    auto found = map.find(ToUpper(name));
    if (found == map.end())
        return Direction::INVALID;
    else
        return found->second;
}

std::string CharToShipName(char c)
{
    switch ((char)std::tolower(c))
    {
    case 'd': return "DESTROYER";
    case 's': return "SUBMARINE";
    case 'r': return "CRUISER";
    case 'b': return "BATTLESHIP";
    case 'c': return "CARRIER";
    };

    return std::string();
}



