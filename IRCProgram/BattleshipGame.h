 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipGame.h 
Purpose:       interface definition for BattleshipGame
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:08 PM
- End Header --------------------------------------------------------*/


#pragma once

#include "BattleshipCommon.h"
#include "BattleshipBoard.h"

// a class composed of two boards and game information
class BattleshipGame
{
public:
    // constructors
    BattleshipGame();
    BattleshipGame(GameID gameid, PlayerID player1, PlayerID player2);

    // board getter
    BattleshipBoard& GetBoard(unsigned id);
    BattleshipBoard& GetBoard(const std::string& player);

    // print board info
    void Print(std::ostream& ost, unsigned player_id) const;

    // manipulate attacking_player
    void SwapAttackingPlayer();
    void SetAttackingPlayer(const std::string& player);
    const std::string& GetAttackingPlayer();

    // manipulate ready state
    bool GetReady(const std::string& player_id) const;
    bool CanBeSetToReady(const std::string& player_id);
    void SetReady(const std::string& player_id);
    bool IsReady() const;

    // helper to get other player's name or player's index
    PlayerID GetOtherPlayer(const std::string& player) const;;
    unsigned PlayerNameToIndex(const std::string& player) const;
    const std::string& GetPlayer(unsigned id);

    // test if player in game
    bool HasPlayer(const std::string& player);

    // test if a player can attack in this game
    bool CanAttack(const std::string& player) const;

    void SetWinner(const std::string& winner);

    void Hibernate();
    bool IsHibernating();

private:
    bool is_hibernating = false; 
    GameID gameid;
    PlayerID attacking_player;
    PlayerID winner;
    std::array<PlayerID,2> players;
    std::array<BattleshipBoard, 2> boards;
    std::array<bool, 2> ready;
};


