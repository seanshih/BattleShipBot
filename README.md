# BattleShipBot

This is a BattleShipBot built on IRC protocol. It includes both the IRC client and the game host bot.

The game can be played by two players after initialize the game with the bot.

Most of the message handling is done by extensible chain of responsibility pattern and is quite maintainable.

## Program Structure

The program is composed of three parts: low level networking interface, IRC client and Battleship host bot.

The IRCClient is built on top of low level TCP/IP protocol, which is wrapped inside classes in **Socket.h**, **SocketStarter.h**, **Address.h**.

**IRCClient** is composed of *IRCConnection* and *ConsoleManager*.
**IRCConnection** is responsible for login and providing basic communication interface like Pass, Nick, User, Join, Part, Pong, PrivMsg, Quit. When receiving server message, it will be passed to extensible message handling chain to decide its response.
**ConsoleManager** is responsible for parsing the user input and hand it to the message handling chain.

To implement the chian of responsibility, we use the pattern similar to Factory method, in which we implement the concrete version of them and fill in the chain in the derived class. The implementations of handlers are in **ConcreteIRCCommandHandlers.h** and **ConcreteConsoleCommandHandlers.h**. The handler composition is in **ConcreteIRCClient.h**

The actual Battleship related logic are in **BattleshipApp.h**. The *BattleshipApp* will create new **BattleshipGame** when the invitation from one player is accepted by other player. *The BattleshipGame* provides interface for game manipulation, which will then manipulate the board of the game.

## Mitigated Risks

1. The player cannot pretend to be other player


## Others
Check out my other [portfolios](http://seanshih.com)! 


