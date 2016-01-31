 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipBoard.cpp 
Purpose:       implementation for BattleshipBoard
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:06 PM
- End Header --------------------------------------------------------*/


#include <cassert>
#include <iomanip>

#include "BattleshipBoard.h"
#include "BattleshipCommon.h"

bool BattleshipBoard::IsPlayerDead()
{
    return num_boat_alive == 0U;
}

void BattleshipBoard::PrintRow(std::ostream& ost, BoardType type, unsigned row_num) const
{
    auto& board = GetBoard(type);
    auto& row = board[row_num];
    ost << (unsigned char)('A' + row_num) << "|";
    for (auto& grid : row)
        ost << std::setw(2) << grid << '|';
}

void BattleshipBoard::PrintHitInfo(std::ostream& ost, unsigned row_num) const
{
    auto& board = GetBoard(BoardType::Ship);
    auto& hit_board = GetBoard(BoardType::Hit);
    ost << (unsigned char)('A' + row_num) << "|";

    auto& board_row = board[row_num];
    auto& hit_row = hit_board[row_num];
    for (unsigned i = 0U; i < GetBoardSize(); ++i)
    {
        char grid = hit_row[i];
        switch (hit_row[i])
        {
        case k_sink_hitstate:
        case k_damaged_hitstate:
            grid = std::toupper(board_row[i]);
            break;
        }
        ost << std::setw(2) << grid << '|';
    }
}

void BattleshipBoard::PrintOverlayRow(std::ostream& ost, unsigned row_num) const
{
    auto& board = GetBoard(BoardType::Ship);
    auto& hit_board = GetBoard(BoardType::Hit);
    ost << (unsigned char)('A' + row_num) << "|";

    auto& board_row = board[row_num];
    auto& hit_row = hit_board[row_num];
    for (unsigned i = 0U; i < GetBoardSize(); ++i)
    {
        char grid = k_empty_hitstate;
        switch (hit_row[i])
        {
        case k_miss_hitstate:
            grid = k_miss_hitstate;
            break;
        case k_empty_hitstate:
            grid = board_row[i];
            break;
        case k_sink_hitstate:
        case k_damaged_hitstate:
            grid = std::toupper(board_row[i]);
            break;
        }
        ost << std::setw(2) << grid << '|';
    }
}

void BattleshipBoard::Print(std::ostream& ost) const
{
    PrintCommonHeader(ost);
    ost << std::endl;

    for (auto irow = 0U; irow < GetBoardSize(); ++irow)
    {
        PrintRow(ost, BoardType::Ship, irow);
        ost << " ";
        PrintRow(ost, BoardType::Hit, irow);
        ost << std::endl;
    }

}

void BattleshipBoard::ClearBoard()
{
    for (auto p_board : boards)
    {
        auto& board = *p_board;
        for (auto& row : board)
            for (auto& grid : row)
                grid = k_empty_gridstate;
    }

    for (auto& is_ship_placed : ship_placed)
        is_ship_placed = false;
}

void BattleshipBoard::SetShipToSink(GridCoord coord)
{
    auto boat_name = GetGridState(coord);
    assert(boat_name != k_empty_gridstate);

    for (auto dir : k_all_directions)
    {
        auto step = GridCoord::FromDirection(dir);
        for (auto grid = coord;; grid += step)
        {
            if (!IsInBound(grid) || GetGridState(grid) != boat_name)
                break;
            else
                SetHitState(grid, k_sink_hitstate);
        }
    }
}

bool BattleshipBoard::IsShipFullyDamaged(GridCoord coord)
{
    auto boat_name = GetGridState(coord);
    if (boat_name == k_empty_gridstate)
        return false;

    for (auto dir : k_all_directions)
    {
        auto step = GridCoord::FromDirection(dir);
        for (auto grid = coord;; grid += step)
        {
            if ((!IsInBound(grid)) || (GetGridState(grid) != boat_name))
                break;
            else if (GetHitState(grid) != k_damaged_hitstate)
                return false;
        }

    }
    return true;
}

bool BattleshipBoard::IsShipSunk(GridCoord coord)
{
    return GetHitState(coord) == k_sink_hitstate;
}

BattleshipBoard::HitResult BattleshipBoard::TryHit(GridCoord coord)
{
    if (!IsValidHit(coord))
        return HitResult::Invalid;
    else
        return Hit(coord);
}

bool BattleshipBoard::IsValidHit(GridCoord coord)
{
    if (!IsInBound(coord))
        return false;

    if (GetHitState(coord) != k_empty_hitstate)
        return false;

    return true;
}

BattleshipBoard::HitResult BattleshipBoard::Hit(GridCoord coord)
{
    if (GetGridState(coord) == k_empty_gridstate)
    {
        SetHitState(coord, k_miss_hitstate);
        return HitResult::Miss;
    }

    SetHitState(coord, k_damaged_hitstate);
    if (IsShipFullyDamaged(coord))
    {
        SetShipToSink(coord);
        num_boat_alive--;
        return HitResult::Sink;
    }

    return HitResult::Hit;
}

void BattleshipBoard::PlaceShip(GridCoord coord, ShipType ship, Direction direction)
{
    auto len = ShipToLen(ship);
    for (auto& grid : IterGridCoord(coord, direction, len))
        SetGridState(grid, ShipToChar(ship));
    ++num_boat_alive;
}

bool BattleshipBoard::TryPlaceShip(GridCoord coord, ShipType ship, Direction direction)
{
    if (IsShipPlaced(ship))
        return false;

    if (!IsValidPlacement(coord, ship, direction))
        return false;
    else
    {
        PlaceShip(coord, ship, direction);
        SetShipPlaced(ship, true);
        return true;
    }
}

void BattleshipBoard::SetShipPlaced(ShipType ship, bool is_ship_placed)
{
    ship_placed[static_cast<unsigned>(ship)] = is_ship_placed;
}

bool BattleshipBoard::IsShipPlaced(ShipType ship)
{
    return ship_placed[static_cast<unsigned>(ship)];
}

bool BattleshipBoard::IsValidPlacement(GridCoord coord, ShipType ship, Direction direction)
{
    auto len = ShipToLen(ship);
    for (auto& grid : IterGridCoord(coord, direction, len))
        if (!IsAvailableGrid(grid))
            return false;

    return true;
}

bool BattleshipBoard::IsInBound(GridCoord coord)
{
    return (IsValidLine(coord.r) && IsValidLine(coord.c));
}

GridState BattleshipBoard::GetGridState(GridCoord coord)
{
    return board[coord.r][coord.c];
}

GridState BattleshipBoard::GetHitState(GridCoord coord)
{
    return hit_board[coord.r][coord.c];
}

void BattleshipBoard::SetHitState(GridCoord coord, GridState state)
{
    hit_board[coord.r][coord.c] = state;
}

void BattleshipBoard::SetGridState(GridCoord coord, GridState state)
{
    board[coord.r][coord.c] = state;
}

void BattleshipBoard::PrintColNum(std::ostream& ost) 
{
    ost << " |";
    for (auto col = 0U; col < GetBoardSize(); ++col)
        ost << std::setw(2) << col + 1 << std::setw(1) << '|';
}

bool BattleshipBoard::IsAvailableGrid(GridCoord coord)
{
    if (!IsInBound(coord))
        return false;

    if (GetGridState(coord) != k_empty_gridstate)
        return false;

    return true;
}

bool BattleshipBoard::IsValidLine(int r)
{
    return r >= 0 && r <= 9;
}

BattleshipBoard::BattleshipBoard()
    : num_boat_alive(0U)
{
    boards[0] = &board;
    boards[1] = &hit_board;
    ClearBoard();
}

BattleshipBoard::BattleshipBoard(const BattleshipBoard& rhs)
    :board(rhs.board), hit_board(rhs.hit_board), ship_placed(rhs.ship_placed), num_boat_alive(rhs.num_boat_alive)
{
    boards[0] = &board;
    boards[1] = &hit_board;
}


void BattleshipBoard::PrintCommonHeader(std::ostream& ost)
{
    ost << std::setw(33) << std::left << "Shots"
        << std::setw(32) << std::left << "Ships" 
        << std::endl;

    ost << std::right;
    PrintColNum(ost);
    ost << " ";
    PrintColNum(ost);
    ost << std::endl;
}


std::ostream& operator<<(std::ostream& ost, const BattleshipBoard& board)
{
    board.Print(ost);
    return ost;
}


const BattleshipBoard::board_t& BattleshipBoard::GetBoard(BoardType board_type) const
{
    return *boards[static_cast<unsigned>(board_type)];
}

BattleshipBoard::board_t& BattleshipBoard::GetBoard(BoardType board_type)
{
    return *boards[static_cast<unsigned>(board_type)];
}

bool BattleshipBoard::IsShipFullyPlaced()
{
    return std::all_of(ship_placed.begin(), ship_placed.end(), [](bool val){return val; });
}

unsigned BattleshipBoard::GetBoardSize()
{
    return k_board_size;
}
