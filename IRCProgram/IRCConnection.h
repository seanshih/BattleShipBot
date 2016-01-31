 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCConnection.h 
Purpose:       interface definition for IRCConnection
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:47 AM
- End Header --------------------------------------------------------*/


#pragma once

#include <string>
#include <memory>
#include <set>
#include <thread>
#include <vector>
#include <atomic>

#include "Address.h"
#include "Socket.h"

class IRCCommand;
class IRCMessageHandler;

class IRCConnection
{
public:
    IRCConnection();;
    ~IRCConnection();;

    virtual bool ConnectionComplete(){ return true; }

    //////////////////
    // login and connection
    //////////////////
    int Connect(const std::string& ip, int port);
    int Connect(const std::string& ip);

    void Login(const std::string& username,
        const std::string& password,
        const std::string& nickname,
        const std::string& realname);

    //////////////////
    // start and stop receiving thread
    //////////////////
    void StartReceiving();
    void StopReceiving();

    ////////////////////
    // message handler addition and processing
    // it uses chain of responsibility pattern
    ////////////////////
    void HandleMessage(const std::string& str);

    template <typename Handler_t, typename ...Args>
    void AddHandler(Args&&...);
    void AddHandler(std::unique_ptr<IRCMessageHandler> handler);

    ////////////////////
    // generic methods to send message
    ////////////////////
    void Send(const std::string& command, const std::string& parameters);
    void Send(const IRCCommand& command);


    ////////////////////
    // predefined ways to send message
    ////////////////////

    // sned password
    void Pass(const std::string& passwd);

    // send nickname
    void Nick(const std::string& nickname);

    // send username and real name
    void User(const std::string& username, const std::string& realname);

    // join a channel, will part from current channel first
    void Join(const std::string& channel);

    // part from current channel
    void Part();

    // pong with respect to ping
    void Pong(const std::string& message);

    // send PRIVMSG
    // if message is empty string, this is no-op
    // if other_user is empty string, this becomes channel message
    void PrivMsg(const std::string& other_user, const std::string& message);

    // send quit message
    void Quit(const std::string& quit_message);

    // regulate send speed
    void TransmissionRegulator();

    //////////////
    // get current irc connection info
    //////////////
    const std::string& GetNickname();
    const std::string& GetCurrentChannel();
    const std::set<std::string>& GetUsers() const;

    ///////////////
    // user list manipulation
    ///////////////
    void ClearUsers();
    void AddUser(const std::string& user);
    void RemoveUser(const std::string& user);
    bool IsUserInRoom(const std::string& user)const;

    //////////////
    // verbosity setting
    // if no ostr given, &std::cout is used by default
    //////////////
    void SetVerbose(bool is_verbose, std::ostream* ostr);
    void SetVerbose(bool is_verbose);

    ////////////////
    // if the user is currently in channel
    ///////////////
    bool IsInChannel();

private:
    // dfault irc port number
    const static auto k_default_irc_port = 6667;

private:
    // toggle to safely stop thread
    std::atomic<bool> _thread_should_stop = false;
    std::thread _receiving_thread;

private:
    bool _is_verbose = true;

    // host address and sockets
    Address _addr;
    Socket _socket;

    // info for irc connection
    std::string _username;
    std::string _nickname;
    std::string _realname;
    std::string _current_channel;

    // user list information
    std::set<std::string> _users;

    // stream destination for verbose output
    std::ostream* _ostr = nullptr;

    // regulate transmission speed
    static const unsigned k_max_send_per_second = 5U;
    using duration_t = std::chrono::milliseconds;
    std::chrono::steady_clock::time_point time_pt;
    static std::array<duration_t, k_max_send_per_second> time_arr;
    unsigned time_idx = 0;

    // IRCCommand handlers
    std::vector<std::unique_ptr<IRCMessageHandler>> _handlers;
};

#include "IRCConnection_Impl.h"

