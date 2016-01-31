 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipBoard.h 
Purpose:       interface definition for BattleshipBoard
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:08 PM
- End Header --------------------------------------------------------*/


#pragma once

#include "BattleshipCommon.h"
#include "GridCoord.h"

// label for grid in the board
const char k_empty_gridstate = ' ';
const char k_empty_hitstate = ' ';
const char k_sink_hitstate = 'S';
const char k_miss_hitstate = '.';
const char k_damaged_hitstate = 'D';


class BattleshipBoard
{
public:
    BattleshipBoard();
    BattleshipBoard(const BattleshipBoard& rhs);

    // return value for TestHit
    enum class HitResult
    {
        Hit,
        Miss,
        Sink,
        Invalid,
    };

    // argument for getting different kinds of boards
    enum class BoardType : unsigned
    {
        Ship,
        Hit,
    };

    static const unsigned k_board_size = 10;
    using board_t = std::array < std::array<char, k_board_size>, k_board_size > ;

    // test if index is out of bound
    bool IsValidLine(int r);

    // test if coord is in board boundary
    bool IsInBound(GridCoord coord);

    // test if grid at coord is available for placing ship
    bool IsAvailableGrid(GridCoord coord);

    // grid/hit state manipulator
    void SetGridState(GridCoord coord, GridState state);
    void SetHitState(GridCoord coord, GridState state);
    GridState GetHitState(GridCoord coord);
    GridState GetGridState(GridCoord coord);

    // ship placed state setter and tester
    bool IsShipPlaced(ShipType ship);
    void SetShipPlaced(ShipType ship, bool is_ship_placed);

    // placing ship functions. Use TryPlaceShip for safety.
    // TryPlaceShip = IsValidPlacement + PlaceShip
    bool TryPlaceShip(GridCoord coord, ShipType ship, Direction direction);
    bool IsValidPlacement(GridCoord coord, ShipType ship, Direction direction);
    void PlaceShip(GridCoord coord, ShipType ship, Direction direction);

    // firing functions. Use TryHit for safety.
    // TryHit = IsValidHit + Hit
    HitResult TryHit(GridCoord coord);
    HitResult Hit(GridCoord coord);
    bool IsValidHit(GridCoord coord);

    // test and set if a ship is sunk
    void SetShipToSink(GridCoord coord);
    bool IsShipSunk(GridCoord coord);
    bool IsShipFullyDamaged(GridCoord coord);

    // if all five ships are already on the board
    bool IsShipFullyPlaced();

    // reset the board state
    void ClearBoard();

    // print common header, including column index row and status
    static void PrintCommonHeader(std::ostream& ost);
    static void PrintColNum(std::ostream& ost);

    // get board data directly
    board_t& GetBoard(BoardType board_type);
    const board_t& GetBoard(BoardType board_type) const;

    // test if all ships of this board's owner are sunk
    bool IsPlayerDead();

    // print both the hit board and grid board
    void Print(std::ostream& ost) const;

    // print specific row of a board
    void PrintRow(std::ostream& ost, BoardType type, unsigned row_num) const;

    // print hit information and grid information, as required in assignment
    // lower case means ship intact, while upper case means ship damaged
    void PrintHitInfo(std::ostream& ost, unsigned row_num) const;
    void PrintOverlayRow(std::ostream& ost, unsigned row_num) const;

    static unsigned GetBoardSize();

private:
    board_t board;
    board_t hit_board;
    std::array<board_t*,2> boards;
    std::array<bool, static_cast<unsigned>(ShipType::NUM_SHIP)> ship_placed;
    unsigned num_boat_alive;
};

// overload to print the board with ostream
std::ostream& operator<<(std::ostream& ost, const BattleshipBoard& board);


