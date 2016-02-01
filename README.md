# BattleShipBot

This is a BattleShipBot built on IRC protocol. It includes both the IRC client and the game host bot.

The game can be played by two players after initializing the game with the bot.

Most of the message handling is done by extensible chain of responsibility pattern, which is quite maintainable.

## Program Structure

The program is composed of three parts: low level networking interface, IRC client and Battleship host bot.

The IRCClient is built on top of low level TCP/IP protocol, which is wrapped inside classes in **Socket.h**, **SocketStarter.h**, **Address.h**.

**IRCClient** is composed of *IRCConnection* and *ConsoleManager*.
**IRCConnection** is responsible for login and providing basic communication interface like Pass, Nick, User, Join, Part, Pong, PrivMsg, Quit. When receiving server message, it will be passed to extensible message handling chain to decide its response.
**ConsoleManager** is responsible for parsing the user input and hand it to the message handling chain.

To implement the chain of responsibility, we use the pattern similar to the Factory method, in which we implement the concrete version of them and fill in the chain in the derived class. The implementation of handlers is in **ConcreteIRCCommandHandlers.h** and **ConcreteConsoleCommandHandlers.h**. The handler composition is in **ConcreteIRCClient.h**

The actual Battleship related logic are in **BattleshipApp.h**. The *BattleshipApp* will create new **BattleshipGame** when the invitation from one player is accepted by other player. *The BattleshipGame* provides interface for game manipulation, which will then manipulate the board of the game.

## Mitigated Risks

**Spoofing**
Player's identity is verified for each command, so one cannot perform actions like observing the board or playing the game if he or she is not in the game.

**Tampering**
The player's input value is checked for its range and validity to make all modifications are legal. As a result, the player cannot cheat by giving incorrect input. This also prevents elevation of privilege or information disclosure due to buffer overflow.

**Repudiation**
Not applicable

**Information Disclosure**
All messages are sent through PrivMsg to prevent information disclosure, and the bot only recognizes private messages.

**Denial of Service**
The bot has internal delay to regulate its own number of sent messages to prevent being kicked out by the server. Also player cannot ask for more board layout during cooldown period. There is also a limit on maximum number of total games that could be opened by the bot. Finally, invitations are ignored when not both players are present.

**Elevation of Privilege**
Not applicable

## Others
Check out my other [portfolios](http://seanshih.com)! 


