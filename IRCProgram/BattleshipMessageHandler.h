 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipMessageHandler.h 
Purpose:       interface definition for BattleshipMessageHandler
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:08 PM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

class BattleshipApp;
class IRCConnection;

// a class for handling warboat message
class BattleshipMessageHandler
{
public:
    BattleshipMessageHandler(IRCConnection& irc_connection, BattleshipApp& warboat_app, unsigned num_game_max);

    // handle message given source, request(command), and list of arguments
    bool Handle(const std::string& source, const std::string request, const std::vector<std::string>& vstrings);

    // test if 
    // 1. source == player1 
    // 2. gameID is valid 
    // 3. player1 is in game
    bool VerifyGameAndPlayer(const std::string& source, const std::vector<std::string>& vstrings);

    // test if vstring.szie == target_length
    bool VerifyArgumentLength(const std::string& source, const std::vector<std::string>& vstrings, unsigned target_length);;

    // test if the game is already finished
    bool VerifyHibernate(const std::string& source, const std::string& gameid);

    // handle invite command
    bool HandleInvite(const std::string& source, const std::vector<std::string>& vstrings);

    // handle ship placement command
    bool HandlePlace(const std::string& source, const std::vector<std::string>& vstrings);

    // handle ship ready command
    bool HandleReady(const std::string& source, const std::vector<std::string>& vstrings);

    // handle ship fire command
    bool HandleFire(const std::string& source, const std::vector<std::string>& vstrings);

    // handle ship concede command
    bool HandleConcede(const std::string& source, const std::vector<std::string>& vstrings);

    // handle ship board command
    bool HandleBoard(const std::string& source, const std::vector<std::string>& vstrings);

    // test if someone sends board request too frequently
    bool BoardRequestTooFast(const std::string& source);

private:
    // helper function to determine if string represents valid number
    bool IsNumber(const std::string& str);

private:
    using handler_t = bool(BattleshipMessageHandler::*)(const std::string& source,  const std::vector<std::string>& vstrings);
    static std::unordered_map<std::string, handler_t> s_response_map;

    IRCConnection* _irc;
    BattleshipApp* _warboat_app;

    std::unordered_map<std::string, std::chrono::steady_clock::time_point> board_request_record;
    static const std::chrono::seconds k_min_board_request_interval;

    unsigned _num_game_max;
};



