#include "battleship.h"
#include <boost/asio.hpp>

#define PLAYER 0
#define PORT 2020

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::flush;

string read(tcp::socket& socket)
{
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\n");
    string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}

void send(tcp::socket &socket, const string& _data)
{
    const string data = _data + "\n";
    boost::asio::write(socket, boost::asio::buffer(data));
}

int main() 
{  
    boost::asio::io_service io_service;  
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), PORT));  
    tcp::socket socket(io_service);  
    acceptor.accept(socket);  
    
    string data = read(socket);
    data.pop_back();

    cout << data << endl;

    cout << "Welcome to Battleship!" << endl;
    cout << "1: New game\n2: Load game\n3: Exit" << endl;
    cout << "Choose option: ";
    int type;
    cin >> type;
    BattleshipGame game = BattleshipGame(10);
    if(type == 1)
    {
        send(socket, "new");
        game.display();
        game.setup(PLAYER);
    }
    else if(type == 2)
    {
        cout << "Enter save filename: ";
        string saveFile;
        cin >> saveFile;
        game.load(saveFile, PLAYER);
        send(socket, "load");
        send(socket, saveFile);
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
    send(socket, "done");
    string reply = read(socket);
    if(reply != "done")
    {
        cout << "Synchronization error" << endl;
        exit(1);
    }
    
    game.display();
    while(!game.isOver())
    {
        //attack
        cout << "Enter target coordinate (or enter q to quit): " << endl;
        char cx;
        char cy;
        cin.clear();
        cin >> cx;
        if(cx == 'q')
        {
            break;
        }
        cin >> cy;
        int x = cx - 'A', y = cy - 'A';
        
        send(socket, std::to_string(x));
        send(socket, std::to_string(y));

        string status = read(socket);
        if(status == "hit")
            game.attack(x, y, true);
        else
            game.attack(x, y, false);
        game.display();
        // defend

        cout << "Waiting for opponent to play" << endl;

        string x_str = read(socket);
        string y_str = read(socket);

        x = std::stoi(x_str);
        y = std::stoi(y_str);

        if(game.defend(x, y))
            status = "hit";
        else
            status = "miss";

        send(socket, status);

        // cout << flush;
        game.display();
    }
    game.showStats();
    
    return 0;  
}  