 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     GridCoord.h 
Purpose:       interface definition for GridCoord
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:07 PM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include <array>
#include <cctype>

#include "BattleshipCommon.h"


struct GridCoord
{
public:
    int r;
    int c;
    bool is_valid;

public:
    GridCoord(int r, int c, bool is_valid);
    GridCoord(int r, int c);
    std::string ToString();


public:
    GridCoord operator+(GridCoord rhs) const;
    GridCoord operator*(int len) const;
    GridCoord& operator+=(const GridCoord& rhs);
    bool operator!=(const GridCoord& rhs);
    bool operator==(const GridCoord& rhs);

    static GridCoord FromDirection(Direction dir);
    static bool IsRowChar(char c);
    static bool IsColStr(const std::string& str);
    static GridCoord FromString(const std::string& str);

    bool IsInvalid();
};

struct IterGridCoord
{
    IterGridCoord(GridCoord start, Direction dir, unsigned len);

    struct Iterator
    {
        Iterator operator++(int);
        Iterator& operator++();
        bool operator!=(const Iterator& rhs);
        bool operator==(const Iterator& rhs);
        GridCoord operator*();

        GridCoord current;
        GridCoord step;
    };

    Iterator begin();
    Iterator end();

    GridCoord start;
    GridCoord dest;
    GridCoord dir;
};


