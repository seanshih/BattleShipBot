 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipGame.cpp 
Purpose:       implementation for BattleshipGame
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:06 PM
- End Header --------------------------------------------------------*/


#include <cassert>

#include "BattleshipGame.h"
#include "BattleshipBoard.h"

bool BattleshipGame::CanAttack(const std::string& player) const
{
    return IsReady() && player == attacking_player;
}

const std::string& BattleshipGame::GetPlayer(unsigned id)
{
    return players[id];
}

unsigned BattleshipGame::PlayerNameToIndex(const std::string& player) const
{
    if (player == players[0]) return 0U;
    if (player == players[1]) return 1U;
    return ~0U;
}

bool BattleshipGame::HasPlayer(const std::string& player)
{
    return player == players[0] || player == players[1];
}

PlayerID BattleshipGame::GetOtherPlayer(const std::string& player) const
{
    if (player == players[0]) return players[1];
    else if (player == players[1]) return players[0];
    else return "";
}

bool BattleshipGame::IsReady() const
{
    return ready[0] && ready[1];
}

void BattleshipGame::SetReady(const std::string& player_id)
{
    if (player_id == players[0]) ready[0] = true;
    if (player_id == players[1]) ready[1] = true;
}

bool BattleshipGame::CanBeSetToReady(const std::string& player_id)
{
    return !GetReady(player_id)
        && GetBoard(player_id).IsShipFullyPlaced();
}

bool BattleshipGame::GetReady(const std::string& player_id) const
{
    if (player_id == players[0]) return ready[0];
    if (player_id == players[1]) return ready[1];

    assert(0);
    return false;
}

const std::string& BattleshipGame::GetAttackingPlayer()
{
    return attacking_player;
}

void BattleshipGame::SetAttackingPlayer(const std::string& player)
{
    attacking_player = player;
}

void BattleshipGame::SwapAttackingPlayer()
{
    if (attacking_player == players[0]) attacking_player = players[1];
    else attacking_player = players[0];
}

BattleshipBoard& BattleshipGame::GetBoard(const std::string& player)
{
    return GetBoard(PlayerNameToIndex(player));
}

BattleshipBoard& BattleshipGame::GetBoard(unsigned id)
{
    return boards[id];
}

BattleshipGame::BattleshipGame(GameID gameid, PlayerID player1, PlayerID player2) :gameid(gameid)
{
    players[0] = player1;
    players[1] = player2;
    ready[0] = ready[1] = false;
}

BattleshipGame::BattleshipGame()
{

}

void BattleshipGame::Print(std::ostream& ost, unsigned player_id) const
{
    ost << "Board for player " << players[player_id] << " in game " << gameid;
    ost << std::endl;

    if (is_hibernating)
        ost << "Won by " + winner;
    else if (ready[0] && ! ready[1])
        ost << "Waiting for " + players[1] + " to place ships";
    else if (!ready[0] && ready[1])
        ost << "Waiting for " + players[0] + " to place ships";
    else if (!ready[0] && !ready[1])
        ost << "Waiting for both players to place ships";
    else
        ost << "Waiting for " + attacking_player + " to fire";

    ost << std::endl;
    BattleshipBoard::PrintCommonHeader(ost);

    const auto& enemy_board = boards[1-player_id];
    const auto& player_board = boards[player_id];
    for (auto i = 0U; i < player_board.GetBoardSize(); ++i)
    {
        enemy_board.PrintHitInfo(ost, i);
        ost << " ";
        player_board.PrintOverlayRow(ost, i);
        ost << std::endl;
    }
}

void BattleshipGame::Hibernate()
{
    is_hibernating = true;
}

bool BattleshipGame::IsHibernating()
{
    return is_hibernating;
}

void BattleshipGame::SetWinner(const std::string& winner_in)
{
    winner = winner_in;
}

