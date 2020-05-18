# battleship

A C++ implementation of the game _Battleship_, which can be played by 2 players over LAN. 
Networking is done using the C++ Boost Asio library, and the game is implemented using OOP.

## Legend
Symbol | Meaning
-------| -------------
O      | Ship
H      | Hit
X      | Miss
.      | Empty

## Game Format

* Grid Size is fixed to 10x10
* Each player has 2 boards
    * **target:** grid that shows where player has shot, along with hits and misses)
    * **shipyard:** grid which contains the player's ships, along with opponent's hits and misses
* Grid Cells are number from _AA_ to _II_ (row, column)
* Player 1 - server
* Player 2 - client

## Instructions

* By default, Player 2 will try to connect to a server created on the same machine (localhost)
* To play across machines, make this change in client.cpp: "127.0.0.1" -> IPAddress of Player 1's machine
* Use _make_ to link header files, compile and create executables
* Start server before running client

## Dependencies

* C++ 11
* Boost.Asio - for networking
