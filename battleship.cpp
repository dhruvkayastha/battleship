#include <bits/stdc++.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

static volatile int keepRunning = 1;

void handler(int sigNo)
{
    keepRunning = 0;
}

class Game
{
protected:
    string game_name;
    int num_players;
    int turn;
public:
    void display();
    void setup(int player_no);

    void save(string filename);
    void load(string filename);
    int getTurn()
    {
        return turn;
    }
};


/*
    Empty: .
    Hit: H
    Miss: X
    Ship: O
*/

class Board
{
public:
    int BOARD_SIZE;
    vector<string> targets;
    vector<string> shipyard;
    // friend void BattleshipGame::save(string filename);
    // friend void BattleshipGame::load(string filename);

    Board(int size = 10)
    {
        if(size > 26)
        {
            cout << "Max Size is 26x26" << endl;
            cout << "Creating board of size 10x10" << endl;
            size = 10;
        }
        BOARD_SIZE = size;
        targets = vector<string>(size, string(size, '.'));
        shipyard = vector<string>(size, string(size, '.'));
    }

    void display(string type = "shipyard")
    {
        auto to_print = shipyard;
        if(type == "shipyard")
        {

        }
        else if(type == "targets")
        {
            to_print = targets;
        }
        else
        {
            cout << "Invlalid Board.display() argument: type=" << type << endl;
            return;
        }
        
        cout << string(BOARD_SIZE*2 + 5, '-') << endl;
        cout << "  | ";
        for(char c = 'A'; c < 'A' + BOARD_SIZE; c++)
            cout << c << ' ';
        cout << endl;
        cout << string(BOARD_SIZE*2 + 5, '-') << endl;
        for(int i = 0; i<BOARD_SIZE; i++)
        {
            auto row = to_print[i];
            cout << char('A' + i) << " | ";
            for(auto x: row)
                cout << x << ' ';
            cout << endl;
        }
        cout << string(BOARD_SIZE*2 + 5, '-') << endl;

    }

    void placeShip(int length)
    {
        cout << "Placing Ship of length = " << length << endl;
        bool done = false;
        int orientation;
        while(!done)
        {
            cout << "Select Orientation (1: Verical, 2: Horizontal)" << endl;
            cin >> orientation;
            if(orientation != 1 && orientation != 2)
            {
                cout << "Re enter orientation" << endl;
                sleep(5);
            }
            else done = true;
            // sleep(10);

        }
        done = false;
        while(!done)
        {
            cout << flush;
            if(orientation == 1)
                cout << "Enter topmost coordinate" << endl;
            else if(orientation == 2)
                cout << "Enter leftmost coordinate" << endl;
            string xy;
            cin >> xy;
            if(xy.size() != 2)
            {
                cout << "Error\nExample: BD" << endl;
                continue;
            }
            if('A' <= xy[0] <= char('A' + BOARD_SIZE - 1) &&  'A' <= xy[1] <= char('A' + BOARD_SIZE - 1))
            {
                int x = xy[0] - 'A';
                int y = xy[1] - 'A';
                bool canPlace = true;
                if(orientation == 1)
                {
                    if(x + length > BOARD_SIZE)
                    {
                        cout << "Out of bounds\n";
                        continue;
                    }
                    for(int i = x; i<x+length; i++)
                    {
                        if(shipyard[i][y] != '.')
                        {
                            cout << "Can't place here\n";
                            canPlace = false;
                            break;
                            // sleep(1);
                        }
                    }
                    if(canPlace)
                    {
                        for(int i = x; i<x + length; i++)
                            shipyard[i][y] = 'O';
                        done = true;
                    }
                }
                else if(orientation == 2)
                {
                    if(y + length > BOARD_SIZE)
                    {
                        cout << "Out of bounds\n";
                        continue;
                    }
                    for(int j = y; j<y+length; j++)
                    {
                        if(shipyard[x][j] != '.')
                        {
                            cout << "Can't place here\n";
                            canPlace = false;
                            break;
                            // sleep(1);
                        }
                    }
                    if(canPlace)
                    {
                        for(int j = y; j<y + length; j++)
                            shipyard[x][j] = 'O';
                        done = true;
                    }
                }
            }
        }
        display();
    }

    vector<string> getTargets()
    {
        return targets;
    }

    vector<string> getShipyard()
    {
        return shipyard;
    }

    void load(std::ifstream& gameFile, int size)
    {
        BOARD_SIZE = size;
        targets = vector<string>(size, string(size, '.'));
        shipyard = vector<string>(size, string(size, '.'));

        for(int i = 0; i<BOARD_SIZE; i++)
            gameFile >> targets[i];
        
        for(int i = 0; i<BOARD_SIZE; i++)
            gameFile >> shipyard[i];
        
        display();
    }

};

class BattleshipGame : public Game
{
    int BOARD_SIZE;
    vector<Board> boards;

    vector<int> remaining;
    vector<int> num_moves;

public:
    BattleshipGame(int size=10)
    {
        if(size > 26)
        {
            cout << "Max Size is 26x26" << endl;
            cout << "Creating board of size 10x10" << endl;
            size = 10;
        }
        game_name = "Battleship";
        num_players = 2;
        turn = 1;
        for(int i = 0; i<num_players; i++)
        {
            Board new_board = Board(size);
            boards.push_back(new_board);
        }
        BOARD_SIZE = size;
        remaining = vector<int>(2);
        num_moves = vector<int>(2);
    }

    void setup(int player_no)
    {
        cout << "Setting up Board for Player " << player_no << endl;
        Board* board = &boards[player_no-1];
        board->placeShip(5);
        board->placeShip(4);
        board->placeShip(4);
        board->placeShip(3);
        board->placeShip(2);
        remaining[player_no-1] = 18;
    }

    void display()
    {
        int player_no = turn - 1;
        cout << "Player " << turn << endl;
        cout << "Target Board" << endl;
        boards[player_no].display("targets");
        cout << "Shipyard" << endl;
        boards[player_no].display("shipyard");
    }

    bool playMove(int x, int y)
    {
        int player = turn - 1;
        int opponent = 1 - player;
        if(x >= BOARD_SIZE || y >= BOARD_SIZE || x < 0 || y < 0)
        {
            cout << "Invalid target coordinate. Try again." << endl;
            return false;
        }
        if(boards[player].targets[x][y] != '.')
        {
            cout << "Target already shot at. Try again." << endl;
            return false;
        }
        if(boards[opponent].shipyard[x][y] == 'O')
        {
            cout << "Hit!" << endl;
            boards[player].targets[x][y] = 'H';
            boards[opponent].shipyard[x][y] = 'H';
            remaining[opponent--];
        }
        else 
        {
            boards[player].targets[x][y] = 'X';
            boards[opponent].shipyard[x][y] = 'X';
            cout << "Missed.." << endl;
        }
        num_moves[player]++;
        turn = opponent + 1;
        return true;
    }

    bool isOver()
    {
        if(remaining[0]*remaining[1] == 0)
            return true;
        return false;
    }

    int getWinner()
    {
        if(isOver())
        {
            if(remaining[0] == 0)
                return 1;
            else return 2;
        }
        else
        {
            return -1;
        }
        
    }

    void showStats()
    {
        int winner = getWinner();
        if(winner != -1)
            cout << "Game over! Player " << winner << " won" << endl;
        
        cout << "Player 1 Stats:" << endl;
        cout << "\tTargets Hit - " << 18 - remaining[1] << endl;
        cout << "\tShots fired - " << num_moves[0] << endl;
        
        cout << "Player 2 Stats:" << endl;
        cout << "\tTargets Hit - " << 18 - remaining[0] << endl;
        cout << "\tShots fired - " << num_moves[1] << endl;
    }

    void save(string filename)
    {
        std::ofstream gameFile(filename);
        if(!gameFile)
        {
            cout << "Game could not be saved" << endl;
            return;
        }
        gameFile << "GameSave" << endl << game_name << endl;
        gameFile << num_players << endl;
        gameFile << turn << endl;
        gameFile << BOARD_SIZE << endl;

        for(int i = 0; i<num_players; i++)
        {
            gameFile << num_moves[i] << ' ' << remaining[i] << endl;
        }
        // vector<int> remaining;
        // vector<int> num_moves;

        auto target1 = boards[0].getTargets();

        for(int i = 0; i < BOARD_SIZE; i++)
            gameFile << target1[i] << endl;

        auto shipyard1 = boards[0].getShipyard();
        for(auto v: shipyard1)
            gameFile << v << endl;

        auto target2 = boards[1].getTargets();

        for(int i = 0; i < BOARD_SIZE; i++)
            gameFile << target2[i] << endl;

        auto shipyard2 = boards[1].getShipyard();
        for(auto v: shipyard2)
            gameFile << v << endl;
        
        gameFile.close();
    }

    void load(string filename)
    {
        std::ifstream gameFile(filename);
        if(!gameFile)
        {
            cout << "Game could not be loaded. Check filename" << endl;
            exit(1);
            return;
        }
        string temp;
        gameFile >> temp;
        if(temp != "GameSave")
        {
            cout << "error in save file" << endl;
            return;
        }
        gameFile >> game_name; 
        gameFile >> num_players;
        gameFile >> turn;
        gameFile >> BOARD_SIZE;

        for(int i = 0; i<num_players; i++)
        {
            gameFile >> num_moves[i];
            gameFile >> remaining[i];
        }

        boards[0].load(gameFile, BOARD_SIZE);
        boards[1].load(gameFile, BOARD_SIZE);
        
        gameFile.close();
    }
};


int main()
{
    cout << "Welcome to Battleship!" << endl;
    cout << "1: New game\n2: Load game\n3: Exit" << endl;
    cout << "Choose option: ";
    int type;
    cin >> type;
    BattleshipGame game = BattleshipGame();
    if(type == 1)
    {
        game.setup(1);
        game.setup(2);
    }
    else if(type == 2)
    {
        cout << "Enter save filename: ";
        string saveFile;
        cin >> saveFile;
        game.load(saveFile);
    }
    else if(type == 3)
    {
        exit(1);
    }
    else
    {
        cout << "Invalid input" << endl;
        exit(1);
    }
    
    signal(SIGINT, handler);

    while(!game.isOver() && keepRunning)    // add condition of how many targets left
    {
        int player_no = game.getTurn();
        cout << "Player " << player_no << "'s turn" << endl;
        game.display();
        cout << "Enter target coordinate (or enter q to quit): " << endl;
        char cx;
        char cy;
        cin.clear();
        cin >> cx;
        if(cx == 'q')
        {
            keepRunning = 0;
            break;
        }
        cin >> cy;
        int x = cx - 'A', y = cy - 'A';
        game.playMove(x, y);
        cout << flush;
    }
    if(!keepRunning)
    {
        cout << "Save game? [y/n]: ";
        char save;
        cin >> save;
        if(save == 'y')
        {
            cout << "Enter save file name: ";
            string file;
            cin >> file;
            game.save(file);
            cout << "Game saved" << endl;
        }
    }
    game.showStats();



}