#include "battleship.h"

int Game::getTurn()
{
    return turn;
}

Board::Board(int size = 10)
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

void Board::display(string type = "shipyard")
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

void Board::placeShip(int length)
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
            sleep(2);
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

vector<string> Board::getTargets()
{
    return targets;
}

vector<string> Board::getShipyard()
{
    return shipyard;
}

void Board::load(std::ifstream& gameFile, int size)
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

BattleshipGame::BattleshipGame(int size=10)
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
    board = Board(size);
    BOARD_SIZE = size;
    remaining = vector<int>(2);
    num_moves = vector<int>(2);
}

void BattleshipGame::setup(int player_no)
{
    cout << "Setting up Board for Player " << player_no + 1 << endl;
    // Board* board = board;
    board.placeShip(5);
    board.placeShip(4);
    board.placeShip(4);
    board.placeShip(3);
    board.placeShip(2);
    remaining[player_no] = 18;
    this->player_no = player_no;
}

void BattleshipGame::display()
{
    // int player_no = turn - 1;
    cout << "Player " << player_no + 1<< endl;
    cout << "Target Board" << endl;
    board.display("targets");
    cout << "Shipyard" << endl;
    board.display("shipyard");
}
/*
bool BattleshipGame::playMove(char x, char y)
{
    int xx = x - 'A';
    int yy = y - 'A';
    return playMove(xx, yy);
}

bool BattleshipGame::playMove(int x, int y)
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
        boards[player].targets[x][y] = 'H';
        boards[opponent].shipyard[x][y] = 'H';
        remaining[opponent--];
        display();
        cout << "Hit Target at " << char('A' + x) << char('A' + y) << "!" << endl;
    }
    else 
    {
        boards[player].targets[x][y] = 'X';
        boards[opponent].shipyard[x][y] = 'X';
        display();
        cout << "Missed.." << endl;
    }
    num_moves[player]++;
    turn = opponent + 1;
    return true;
}
*/
bool BattleshipGame::isOver()
{
    if(remaining[0]*remaining[1] == 0)
        return true;
    return false;
}

int BattleshipGame::getWinner()
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

void BattleshipGame::showStats()
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

void BattleshipGame::save(string filename)
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

    auto target1 = board.getTargets();

    for(int i = 0; i < BOARD_SIZE; i++)
        gameFile << target1[i] << endl;

    auto shipyard1 = board.getShipyard();
    for(auto v: shipyard1)
        gameFile << v << endl;

    auto target2 = board.getTargets();

    for(int i = 0; i < BOARD_SIZE; i++)
        gameFile << target2[i] << endl;

    auto shipyard2 = board.getShipyard();
    for(auto v: shipyard2)
        gameFile << v << endl;
    
    gameFile.close();
}

void BattleshipGame::load(string filename, int player_no)
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

        board.load(gameFile, BOARD_SIZE);
        board.load(gameFile, BOARD_SIZE);
        
        gameFile.close();
    }

bool BattleshipGame::defend(int x, int y)
{
    int opponent = 1 - player_no;
    
    bool defended = true;

    if(board.shipyard[x][y] == 'O')
    {
        board.shipyard[x][y] = 'H';
        remaining[player_no]--;
        // display();
        cout << "Opponent hit Target at " << char('A' + x) << char('A' + y) << "!" << endl;
        defended = false;
    }
    else 
    {
        board.shipyard[x][y] = 'X';
        // display();
        cout << "Opponent missed.." << endl;
    }
    num_moves[opponent]++;
    return defended;
}

bool BattleshipGame::canAttack(int x, int y)  
{
    int opponent = 1 - player_no;
    if(x >= BOARD_SIZE || y >= BOARD_SIZE || x < 0 || y < 0)
    {
        cout << "Invalid target coordinate. Try again." << endl;
        return false;
    }
    if(board.targets[x][y] != '.')
    {
        cout << "Target already shot at. Try again." << endl;
        return false;
    }
    return true;
}

bool BattleshipGame::attack(int x, int y, bool hit)
{
    if(hit)
    {
        board.targets[x][y] = 'H';
        cout << "Hit Target at " << char('A' + x) << char('A' + y) << "!" << endl;
        remaining[1-player_no]--;
    }
    else
    {
        board.targets[x][y] = 'X';
        cout << "Shot at " << char('A' + x) << char('A' + y) << "..." << endl;
    }
    num_moves[player_no]++;
    return hit;
}