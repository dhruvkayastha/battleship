#pragma once

#include <bits/stdc++.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::string;
using std::flush;
using std::endl;
using std::vector;



class Game
{
protected:
    string game_name;
    int num_players;
    int turn;
    int player_no;
public:
    void display();
    void setup(int player_no);

    void save(string filename);
    void load(string filename);
    int getTurn();
};

class Board
{
public:
    int BOARD_SIZE;
    vector<string> targets;
    vector<string> shipyard;
    // friend void BattleshipGame::save(string filename);
    // friend void BattleshipGame::load(string filename);

    Board(int size);
    void display(string type);
    void placeShip(int length);
    vector<string> getTargets();
    vector<string> getShipyard();
    void load(std::ifstream &gameFile, int size);
};

class BattleshipGame : public Game
{
    int BOARD_SIZE;
    // vector<Board> boards;
    Board board;

    // int player_no;

    vector<int> remaining;
    vector<int> num_moves;

public:
    // BattleshipGame();
    BattleshipGame(int size);

    void setup(int player_no_);

    void display();

    bool playMove(int x, int y);
    bool playMove(char x, char y);

    bool canAttack(int x, int y);
    bool defend(int x, int y);
    bool attack(int x, int y, bool hit);

    bool isOver();

    void showStats();
    int getWinner();
    void save(string filename);
    void load(string filename, int player_no);
};

