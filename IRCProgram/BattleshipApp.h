 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipApp.cpp 
Purpose:       implementation for BattleshipApp
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Apr 10, 2015  3:20:44 AM
- End Header --------------------------------------------------------*/

#pragma once

#include <string>
#include <array>
#include <cassert>
#include <iomanip>
#include <random>
#include <unordered_map>
#include <limits>
#include <iostream>
#include <cctype>
#include <sstream>
#include <thread>
#include <chrono>
#include <unordered_set>

#include "BattleshipCommon.h"
#include "BattleshipGame.h"

#include "IRCConnection.h"
#include "GridCoord.h"


class BattleshipApp
{
private:
    static std::default_random_engine rand_eng;
    static std::uniform_int_distribution<unsigned> distri;

public:
    static const unsigned k_gameid_not_found = ~0U;

public:
    BattleshipApp();

    GameID GetValidID();

    void ClearGameExists(PlayerID player1, PlayerID player2);

    void SetGameExists(PlayerID player1, PlayerID player2, GameID id);

    GameID GetGameIDFromName(PlayerID player1, PlayerID player2);

    GameID CreateNewGame(PlayerID player1, PlayerID player2);

    unsigned GetNumGame();

    void HibernateGame(GameID gameid);
    void HibernateGame(const std::string& id);

    bool IsGameIDValid(GameID id);
    bool IsGameIDValid(const std::string& id);

    bool IsGameReady(const std::string& id);
    bool IsGameReady(GameID id);

    BattleshipGame& GetGame(GameID gameid);
    BattleshipGame& GetGame(const std::string& gameid);

private:
    std::unordered_map<std::string, GameID> game_exists;
    std::unordered_map<GameID, BattleshipGame> game_map;
};

