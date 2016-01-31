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

#include "BattleshipCommon.h"
#include "BattleshipApp.h"


BattleshipGame& BattleshipApp::GetGame(const std::string& gameid)
{
    return GetGame((unsigned)std::stoi(gameid));
}

BattleshipGame& BattleshipApp::GetGame(GameID gameid)
{
    assert(IsGameIDValid(gameid)); return game_map[gameid];
}

bool BattleshipApp::IsGameReady(GameID id)
{
    return GetGame(id).IsReady();
}

bool BattleshipApp::IsGameReady(const std::string& id)
{
    return IsGameReady((unsigned)std::stoi(id));
}

bool BattleshipApp::IsGameIDValid(const std::string& id)
{
    return IsGameIDValid((unsigned)std::stoi(id));
}

bool BattleshipApp::IsGameIDValid(GameID id)
{
    return game_map.find(id) != game_map.end();
}

void BattleshipApp::HibernateGame(const std::string& id)
{
    HibernateGame((unsigned)std::stoi(id));
}

void BattleshipApp::HibernateGame(GameID gameid)
{
    auto& game = game_map.find(gameid)->second;
    game.Hibernate();
    ClearGameExists(game.GetPlayer(0U), game.GetPlayer(1U));
}

unsigned BattleshipApp::GetNumGame()
{
    return game_map.size();
}

GameID BattleshipApp::CreateNewGame(PlayerID player1, PlayerID player2)
{
    auto id = GetValidID();
    game_map.emplace(std::make_pair(id, BattleshipGame(id, player1, player2)));

    SetGameExists(player1, player2, id);
    return id;
}

GameID BattleshipApp::GetGameIDFromName(PlayerID player1, PlayerID player2)
{
    auto iter = game_exists.find(player1 + player2);
    if (iter != game_exists.end())
        return iter->second;
    else
        return k_gameid_not_found;
}

void BattleshipApp::SetGameExists(PlayerID player1, PlayerID player2, GameID id)
{
    game_exists[player1 + player2] = id;
    if (player1 != player2)
        game_exists[player2 + player1] = id;
}

void BattleshipApp::ClearGameExists(PlayerID player1, PlayerID player2)
{
    game_exists.erase(game_exists.find(player1 + player2));
    if (player1 != player2)
        game_exists.erase(game_exists.find(player2 + player1));
}

GameID BattleshipApp::GetValidID()
{
    GameID id = 0U;

    do
        id = distri(rand_eng);
    while (game_map.find(id) != game_map.end());

    return id;
}

BattleshipApp::BattleshipApp()
{

}

std::default_random_engine BattleshipApp::rand_eng;
std::uniform_int_distribution<unsigned> BattleshipApp::distri ( 1000U, 9999U );

