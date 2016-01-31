 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipCommon.h 
Purpose:       interface definition for BattleshipCommon
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:08 PM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include <array>
#include <cctype>

using GameID = unsigned;
using PlayerID = std::string;
using ShipID = std::string;
using GridState = char;

namespace
{
    std::string ToUpper(const std::string& str)
    {
        auto returned = str;
        for (auto i = 0U; i < str.length(); ++i)
            returned[i] = (char)std::toupper(str[i]);
        return returned;
    }


    std::string ToLower(const std::string& str)
    {
        auto returned = str;
        for (auto i = 0U; i < str.length(); ++i)
            returned[i] = (char)std::tolower(str[i]);
        return returned;
    }
}


// ship types
enum class ShipType : unsigned
{
    DESTROYER,
    SUBMARINE,
    CRUISER,
    BATTLESHIP,
    CARRIER,
    NUM_SHIP,
    INVALID = NUM_SHIP, 
};
const auto k_num_ship = static_cast<unsigned>(ShipType::NUM_SHIP);

// direction types
enum class Direction
{
    N,
    E,
    S,
    W,
    INVALID
};

// convert ship name to shiptype enum class
ShipType ShipNameToShipType(const std::string& name);

// convert direction name to direction enum class
Direction DirectionNameToDirection(const std::string& name);

// array for easy iteration over direction
const std::array<Direction, 4> k_all_directions =
{
    Direction::N,
    Direction::E,
    Direction::S,
    Direction::W
};

// convert character to ship name
std::string CharToShipName(char c);

// convert shiptype to character
unsigned ShipToChar(ShipType ship);

// convert shiptype to ship length
unsigned ShipToLen(ShipType ship);


