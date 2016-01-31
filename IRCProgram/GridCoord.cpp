 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     GridCoord.cpp 
Purpose:       implementation for GridCoord
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:05 PM
- End Header --------------------------------------------------------*/


#include <cassert>

#include "GridCoord.h"
#include "BattleshipCommon.h"

GridCoord GridCoord::FromDirection(Direction dir)
{
    switch (dir)
    {
    case Direction::W: return GridCoord{ 0, -1 };
    case Direction::S: return GridCoord{ 1, 0 };
    case Direction::N: return GridCoord{ -1, 0 };
    case Direction::E: return GridCoord{ 0, 1 };
    }

    assert(0);
    return GridCoord{ 0, 0 };
}

bool GridCoord::operator==(const GridCoord& rhs)
{
    return (r == rhs.r) && (c == rhs.c);
}

bool GridCoord::operator!=(const GridCoord& rhs)
{
    return !(*this == rhs);
}

GridCoord& GridCoord::operator+=(const GridCoord& rhs)
{
    r += rhs.r; c += rhs.c; return *this;
}

GridCoord GridCoord::operator*(int len) const
{
    return GridCoord{ r * len, c * len };
}

GridCoord GridCoord::operator+(GridCoord rhs) const
{
    return GridCoord{ r + rhs.r, c + rhs.c };
}

bool GridCoord::IsInvalid()
{
    return !is_valid;
}

GridCoord GridCoord::FromString(const std::string& str)
{
    if (str.size() > 3U || str.size() <= 1U) { return{ 0, 0, false }; }
    if (!IsRowChar(str[0]))                { return{ 0, 0, false }; }
    if (!IsColStr(str.substr(1)))          { return{ 0, 0, false }; }

    int c = std::stoi(str.substr(1)) - 1;
    int r = std::toupper(str[0]) - 'A';
    return{ r, c, true };
}

bool GridCoord::IsColStr(const std::string& str)
{
    if (str.size() == 2U)
        return str[0] == '1' && str[1] == '0';
    else // str.size() == 1U
        return str[0] >= '0' && str[0] <= '9';
}

bool GridCoord::IsRowChar(char c)
{
    return (c >= 'A' && c <= 'A' + 9) || (c >= 'a' && c <= 'a' + 9);
}

std::string GridCoord::ToString()
{
    std::array<char, 4> data;
    data[3] = '\0';
    data[2] = c == 9 ? '0' : '\0';
    data[1] = c == 9 ? '1' : ('1' + c);
    data[0] = 'A' + r;
    return data.data();
}

GridCoord::GridCoord(int r, int c) : GridCoord(r, c, true)
{

}

GridCoord::GridCoord(int r, int c, bool is_valid) : r(r), c(c), is_valid(is_valid)
{

}

IterGridCoord::Iterator IterGridCoord::end()
{
    return{ dest, dir };
}

IterGridCoord::Iterator IterGridCoord::begin()
{
    return{ start, dir };
}

IterGridCoord::IterGridCoord(GridCoord start, Direction dir, unsigned len) :start(start), dest(start + GridCoord::FromDirection(dir) * len), dir(GridCoord::FromDirection(dir))
{

}

GridCoord IterGridCoord::Iterator::operator*()
{
    return current;
}

bool IterGridCoord::Iterator::operator==(const Iterator& rhs)
{
    return current == rhs.current;
}

bool IterGridCoord::Iterator::operator!=(const Iterator& rhs)
{
    return !(*this == rhs);
}

IterGridCoord::Iterator& IterGridCoord::Iterator::operator++()
{
    current += step; return *this;
}

IterGridCoord::Iterator IterGridCoord::Iterator::operator++(int)
{
    return Iterator{ current + step, step };
}


