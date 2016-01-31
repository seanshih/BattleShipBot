 /* Start Header -------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     BattleshipMessageHandler.cpp 
Purpose:       implementation for BattleshipMessageHandler
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_4
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sat, Apr 11, 2015 11:10:06 PM
- End Header --------------------------------------------------------*/


#include "BattleshipApp.h"
#include "BattleshipMessageHandler.h"


bool BattleshipMessageHandler::IsNumber(const std::string& str)
{
    return !str.empty()
        && std::find_if(str.begin(), str.end(), [](char c) {
        return !std::isdigit(c);
    }) == str.end();
}

bool BattleshipMessageHandler::HandleBoard(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (!VerifyArgumentLength(source, vstrings, 2U)) return false;
    if (!VerifyGameAndPlayer(source, vstrings)) return false;
    if (BoardRequestTooFast(source))
    {
        _irc->PrivMsg(source, "Only one board request allowed every " + std::to_string(k_min_board_request_interval.count()) + " seconds.");
        return false;
    }

    const auto& game = _warboat_app->GetGame(vstrings[0]);
    const auto& player = vstrings[1];
    std::stringstream ss;
    game.Print(ss, game.PlayerNameToIndex(player));

    if (_irc->IsUserInRoom(player))
    {
        std::string message;
        while (std::getline(ss, message))
            _irc->PrivMsg(player, message);
    }
    return true;
}

bool BattleshipMessageHandler::HandleConcede(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (!VerifyArgumentLength(source, vstrings, 2U)) return false;
    if (!VerifyGameAndPlayer(source, vstrings)) return false;
    if (!VerifyHibernate(source, vstrings[0])) return false;


    const auto& player1 = vstrings[1];
    const auto& player2 = _warboat_app->GetGame(vstrings[0]).GetOtherPlayer(player1);
    const std::string& message =
        player1
        + " has resigned. "
        + player2
        + " wins game "
        + vstrings[0]
        + ".";

    _warboat_app->GetGame(vstrings[0]).SetWinner(player2);
    _warboat_app->HibernateGame(vstrings[0]);

    if (_irc->IsUserInRoom(player1)) _irc->PrivMsg(player1, message);
    if (_irc->IsUserInRoom(player2)) _irc->PrivMsg(player2, message);

    return true;
}

bool BattleshipMessageHandler::HandleFire(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (!VerifyArgumentLength(source, vstrings, 3U)) return false;
    if (!VerifyGameAndPlayer(source, vstrings)) return false;
    if (!VerifyHibernate(source, vstrings[0])) return false;

    const auto& game_id = vstrings[0];
    auto& game = _warboat_app->GetGame(game_id);

    // only fire if game is ready
    if (!game.IsReady())
    {
        _irc->PrivMsg(vstrings[1], "Game is not ready yet. Cannot fire.");
        return false;
    }

    const auto& player1 = vstrings[1];
    const auto& player2 = game.GetOtherPlayer(vstrings[1]);

    // only fire if it is player1's turn
    if (!game.CanAttack(player1))
    {
        _irc->PrivMsg(player1, "Not your turn to fire. Wait for the other player's action.");
        return false;
    }
    else
    {
        // only fire if the coordinate is in correct format
        auto grid_coord = GridCoord::FromString(vstrings[2]);
        if (grid_coord.IsInvalid())
        {
            _irc->PrivMsg(player1, "Invalid coordinate format.");
            return false;
        }

        // only fire if the coordinate is in available
        auto& enemy_board = game.GetBoard(player2);
        auto result = enemy_board.TryHit(grid_coord);
        if (result == BattleshipBoard::HitResult::Invalid)
        {
            _irc->PrivMsg(player1, "Invalid fire coordinate. Try again.");
            return false;
        }
        else
        {
            // handle miss or hit message
            std::string hit_miss_text;
            if (result == BattleshipBoard::HitResult::Miss)
                hit_miss_text = "MISSED";
            else
                hit_miss_text = std::string("HIT ") + (char)std::toupper(enemy_board.GetGridState(grid_coord));

            const std::string& hit_miss_message =
                "In game " + game_id + ", "
                + player1 + " fired at " + grid_coord.ToString() + " and " + hit_miss_text + ".";

            _irc->PrivMsg(player1, hit_miss_message);
            _irc->PrivMsg(player2, hit_miss_message);

            // handle ship sunk situation
            if (enemy_board.IsShipSunk(grid_coord))
            {
                const std::string& ship_sunk_message =
                    "In game " + game_id + ", "
                    + player1 + " sunk " + player2 + "'s "
                    + (char)std::toupper(enemy_board.GetGridState(grid_coord));

                _irc->PrivMsg(player1, ship_sunk_message);
                _irc->PrivMsg(player2, ship_sunk_message);

                // handle player dead and winning situation
                if (enemy_board.IsPlayerDead())
                {
                    const std::string& death_message =
                        player2 + "'s last ship has been sunk. "
                        + player1 + " wins game " + game_id + ".";

                    _irc->PrivMsg(player1, death_message);
                    _irc->PrivMsg(player2, death_message);

                    _warboat_app->GetGame(game_id).SetWinner(player1);
                    _warboat_app->HibernateGame(game_id);
                    return true;
                }
            }

            // if the game continues, swap players' attacking order and notify them
            game.SwapAttackingPlayer();
            const auto& message_start = "It is your turn in game " + vstrings[0] + ".";
            _irc->PrivMsg(game.GetAttackingPlayer(), message_start);

            return true;
        }
    }

}

bool BattleshipMessageHandler::HandleReady(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (!VerifyArgumentLength(source, vstrings, 2U)) return false;
    if (!VerifyGameAndPlayer(source, vstrings)) return false;
    if (!VerifyHibernate(source, vstrings[0])) return false;

    // only ready when it can be set to ready
    auto& game = _warboat_app->GetGame(vstrings[0]);
    if (!game.CanBeSetToReady(vstrings[1]))
    {
        _irc->PrivMsg(vstrings[1], "Cannot be set to ready. Maybe the ship is not fully placed or you are already ready");
        return false;
    }

    const auto& player1 = vstrings[1];
    const auto& player2 = game.GetOtherPlayer(vstrings[1]);

    game.SetReady(player1);

    // handle ready message for notifying other player
    if (!game.GetReady(player2))
    {
        _irc->PrivMsg(player1, "Waiting for " + player2);
        _irc->PrivMsg(player2, player1 + "is ready in game " + vstrings[0]);
    }
    else
    {
        const auto& message = "Game " + vstrings[0] + " has begun.";
        _irc->PrivMsg(player1, message);
        _irc->PrivMsg(player2, message);

        game.SetAttackingPlayer((std::rand() % 2 == 0) ? player1 : player2);

        const auto& message_start = "It is your turn in game " + vstrings[0] + ".";
        _irc->PrivMsg(game.GetAttackingPlayer(), message_start);
    }

    return true;
}

bool BattleshipMessageHandler::HandlePlace(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (!VerifyArgumentLength(source, vstrings, 5U)) return false;
    if (!VerifyGameAndPlayer(source, vstrings)) return false;
    if (!VerifyHibernate(source, vstrings[0])) return false;

    // check if invalid ship name
    auto ship_type = ShipNameToShipType(vstrings[2]);
    if (ship_type == ShipType::INVALID) 
    { 
        _irc->PrivMsg(vstrings[1], vstrings[2] + " is not a valid ship name.");
        return false; 
    }

    // check if invalid coordinate
    auto coord = GridCoord::FromString(vstrings[3]);
    if (coord.IsInvalid())
    {
        _irc->PrivMsg(vstrings[1], vstrings[3] + " is not a valid coordinate.");
        return false;
    }

    // check if invalid direction
    auto direction = DirectionNameToDirection(vstrings[4]);
    if (direction == Direction::INVALID) 
    {
        _irc->PrivMsg(vstrings[1], vstrings[4] + " is not a valid direction.");
        return false;
    }

    // cannot place after ready
    auto& game = _warboat_app->GetGame(vstrings[0]);
    if (game.GetReady(vstrings[1]))
    {
        _irc->PrivMsg(vstrings[1], "Cannot place ship after ready.");
        return false;
    }

    const auto& player = vstrings[1];

    auto& board = game.GetBoard(player);
    auto result = board.TryPlaceShip(coord, ship_type, direction);
    if (result)
    {
        auto message = "Ship " + vstrings[2] + " placed.";
        _irc->PrivMsg(player, message);
    }
    else
        _irc->PrivMsg(player, "Ship placement coordinate invalid. It overlaps or goes out of bounds. Please try again.");

    return result;
}

bool BattleshipMessageHandler::HandleInvite(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (vstrings.size() != 2U)
    {
        _irc->PrivMsg(source, "Invalid number of arguments.");
        return false;
    }

    if (source != vstrings[0])
    {
        _irc->PrivMsg(source, "Message sender is required to be the first player as well.");
        return false;
    }

    if (!_irc->IsUserInRoom(vstrings[1]))
    {
        _irc->PrivMsg(source, "Invitee is not in the channel.");
        return false;
    }

    if (_warboat_app->GetNumGame() >= _num_game_max)
    {
        _irc->PrivMsg(source, "Maximum number of game reached. Cannot create any new games.");
        return false;
    }

    if (vstrings[0] == vstrings[1])
    {
        _irc->PrivMsg(source, "Player1 and player2 should not have the same nickname.");
        return false;
    }

    auto pre_gameid = _warboat_app->GetGameIDFromName(vstrings[0], vstrings[1]);
    if (pre_gameid != BattleshipApp::k_gameid_not_found)
    {
        _irc->PrivMsg(source, "There is already an active game between the two players. Finish it first before creating a new one.");
        return false;
    }

    auto gameid = _warboat_app->CreateNewGame(vstrings[0], vstrings[1]);
    _irc->PrivMsg(source, "Game " + std::to_string(gameid) + " created");
    return true;
}

bool BattleshipMessageHandler::VerifyGameAndPlayer(const std::string& source, const std::vector<std::string>& vstrings)
{
    if (vstrings.size() < 2U) return false;

    const auto& gameid = vstrings[0];
    const auto& player = vstrings[1];

    if (source != player)
    {
        _irc->PrivMsg(source, "Message denied. The message sender should be the first player as well.");
        return false;
    }

    if (!IsNumber(gameid) // Ensures IsGameIDValid successful
        || !_warboat_app->IsGameIDValid(vstrings[0]) // ensures GetGame successful
        || !_warboat_app->GetGame(vstrings[0]).HasPlayer(vstrings[1]))
    {
        _irc->PrivMsg(source, "Invalid gameID given, the game not created, or the player1 is not in the game");
        return false;
    }
    return true;
}

bool BattleshipMessageHandler::Handle(const std::string& source, const std::string request, const std::vector<std::string>& vstrings)
{
    if (s_response_map.find(request) == s_response_map.end())
        return false;
    else
        return (this->*s_response_map[request])(source, vstrings);
}

BattleshipMessageHandler::BattleshipMessageHandler(IRCConnection& irc_connection, BattleshipApp& warboat_app, unsigned num_game_max) 
    :_irc(&irc_connection), _warboat_app(&warboat_app), _num_game_max(num_game_max)
{

}


std::unordered_map<std::string, BattleshipMessageHandler::handler_t> BattleshipMessageHandler::s_response_map =
{
    { "invite", &BattleshipMessageHandler::HandleInvite },
    { "place",  &BattleshipMessageHandler::HandlePlace },
    { "ready",  &BattleshipMessageHandler::HandleReady },
    { "fire",   &BattleshipMessageHandler::HandleFire },
    { "concede",&BattleshipMessageHandler::HandleConcede },
    { "board",  &BattleshipMessageHandler::HandleBoard },
};

bool BattleshipMessageHandler::VerifyHibernate(const std::string& source, const std::string& gameid)
{
    if (_warboat_app->GetGame(gameid).IsHibernating())
    {
        _irc->PrivMsg(source, "Game " + gameid + " has finished. You can only view the board.");
        return false;
    }
    return true;
}

bool BattleshipMessageHandler::VerifyArgumentLength(const std::string& source, const std::vector<std::string>& vstrings, unsigned target_length)
{
    if (vstrings.size() != target_length)
    {
        _irc->PrivMsg(source, "Invalid number of arguments.");
        return false;
    }

    return true;
}

bool BattleshipMessageHandler::BoardRequestTooFast(const std::string& source)
{
    auto now_t = std::chrono::steady_clock::now();
    auto iter = board_request_record.find(source);
    if (iter == board_request_record.end())
    {
        board_request_record[source] = now_t;
        return false;
    }

    auto pre_t = iter->second;
    if (now_t - pre_t < k_min_board_request_interval)
        return true;
    else
    {
        board_request_record[source] = now_t;
        return false;
    }
}

const std::chrono::seconds BattleshipMessageHandler::k_min_board_request_interval = std::chrono::seconds(6);
