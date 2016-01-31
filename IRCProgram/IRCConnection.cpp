 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     IRCConnection.cpp 
Purpose:       implementation for IRCConnection
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:44 AM
- End Header --------------------------------------------------------*/


#include "IRCConnection.h"
#include "IRCCommand.h"
#include "IRCMessageHandler.h"
#include <numeric>
#include <iostream>
#include <thread>
#include <chrono>

void IRCConnection::RemoveUser(const std::string& user)
{
    auto iter = _users.find(user);
    if (iter != _users.end())
        _users.erase(iter);
}

void IRCConnection::AddUser(const std::string& user)
{
    if (user != _nickname)
        _users.insert(user);
}

void IRCConnection::ClearUsers()
{
    _users.clear();
}

const std::set<std::string>& IRCConnection::GetUsers() const
{
    return _users;
}

const std::string& IRCConnection::GetCurrentChannel()
{
    return _current_channel;
}

void IRCConnection::Send(const IRCCommand& command)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    TransmissionRegulator();

    _socket.Send(command, _addr);
    if (_is_verbose)
        (*_ostr) << "      <== " << command << "\n";
}

void IRCConnection::Send(const std::string& command, const std::string& parameters)
{
    Send(IRCCommand{ command, parameters });
}

void IRCConnection::Quit(const std::string& quit_message)
{
    Send("QUIT", ":" + quit_message);
}

void IRCConnection::User(const std::string& username, const std::string& realname)
{
    Send("USER", username + " 0 * :" + realname);
}

void IRCConnection::Nick(const std::string& nickname)
{
    Send("NICK", nickname);
}

void IRCConnection::Pass(const std::string& passwd)
{
    Send("PASS", passwd);
}

void IRCConnection::Join(const std::string& channel)
{
    if (_current_channel != channel)
    {
        Part();
        _current_channel = channel;
        Send("JOIN", channel);
    }
}

void IRCConnection::Part()
{
    if (IsInChannel())
    {
        Send("PART", _current_channel);
        _current_channel.clear();
        ClearUsers();
    }
}

void IRCConnection::Pong(const std::string& message)
{
    Send("PONG", ": " + message);
}


void IRCConnection::StartReceiving()
{
    // create thread to constantly receive data
    _receiving_thread = std::thread([this](){
        std::array < char, 1024 > buffer;
        std::string str;
        while (!_thread_should_stop)
        {
            // receive data to buffer
            auto nbyte = _socket.Receive(buffer);
            if (nbyte < 0)
                break;

            // head and tail of received data
            auto head = buffer.begin();
            auto tail = buffer.begin() + nbyte;

            // loop to handle message whenever \r encountered
            for (;;)
            {
                // if /r not found, lines not completed, continue to receive message
                auto return_place = std::find(head, tail, '\r');
                if (return_place == tail)
                    break;

                // if /r found, concatenate info to str, and send str to handlers
                str.append(head, return_place);
                HandleMessage(str);

                // clear str after message handled
                str.clear();

                // skip \r\n
                head = return_place + 2;
            }

            // append last chunk of data to str, and continue to receive message to complete the command
            if (head != tail)
                str.append(head, tail);
        };
    });

}

void IRCConnection::AddHandler(std::unique_ptr<IRCMessageHandler> handler)
{
    _handlers.emplace_back(std::move(handler));
}

// handle message with chain of responsibility
void IRCConnection::HandleMessage(const std::string& str)
{
    IRCCommand command(str);

    if (_is_verbose)
        (*_ostr) << "      ==> " << command << "\n";

    for (auto& handler : _handlers)
    {
        auto is_handled = (*handler)(command, *this);
        if (is_handled)
            break;
    }
}

// login with username, password, nickname and realname
void IRCConnection::Login(const std::string& username, const std::string& password, const std::string& nickname, const std::string& realname)
{
    _username = username;
    _nickname = nickname;
    _realname = realname;

    Pass(password);
    Nick(_nickname);
    User(_username, _realname);
}


const std::string& IRCConnection::GetNickname()
{
    return _nickname;
}


// connect with default port
int IRCConnection::Connect(const std::string& ip)
{
    return Connect(ip, k_default_irc_port);
}

// connect with given port
int IRCConnection::Connect(const std::string& ip, int port)
{
    _addr = Address{ ip.data(), port };
    return _socket.Connect(_addr);
}

// when program ends, stop connection if thread is still running
IRCConnection::~IRCConnection()
{
    if (_receiving_thread.joinable())
    {
        Quit("");
        StopReceiving();
    }
}

IRCConnection::IRCConnection()
{
    std::fill(time_arr.begin(), time_arr.end(), std::chrono::milliseconds(std::numeric_limits<unsigned>::max()));
}

void IRCConnection::PrivMsg(const std::string& other_user, const std::string& message)
{
    if (message.empty())
        return;

    std::string sent_message;
    if (other_user.empty())
        sent_message = _current_channel + " :" + message;
    else
        sent_message = other_user + " :" + message;

    Send(IRCCommand("PRIVMSG", sent_message));
}

void IRCConnection::SetVerbose(bool is_verbose, std::ostream* ostr)
{
    _is_verbose = is_verbose;
    _ostr = is_verbose ? ostr : nullptr;
}

void IRCConnection::SetVerbose(bool is_verbose)
{
    SetVerbose(is_verbose, &std::cout);
}

bool IRCConnection::IsInChannel()
{
    return _current_channel.size() > 0U;
}

void IRCConnection::StopReceiving()
{
    if (_receiving_thread.joinable())
    {
        _thread_should_stop = true;
        _receiving_thread.join();
    }
}

bool IRCConnection::IsUserInRoom(const std::string& user) const
{
    return _users.find(user) != _users.end()
        || user == _nickname;
}

void IRCConnection::TransmissionRegulator()
{
    auto now_time = std::chrono::steady_clock::now();
    time_arr[time_idx] = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - time_pt);

    time_pt = now_time;
    time_idx = (time_idx + 1U) % k_max_send_per_second;

    auto sum_time = std::accumulate(time_arr.begin(), time_arr.end(), std::chrono::milliseconds(0));
    if (sum_time < std::chrono::seconds(1))
        std::this_thread::sleep_for(std::chrono::seconds(1) - sum_time);
}

std::array<IRCConnection::duration_t, IRCConnection::k_max_send_per_second> IRCConnection::time_arr;

