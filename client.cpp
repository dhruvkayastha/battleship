#include "battleship.h"
#include <boost/asio.hpp>

#define PLAYER 1
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
    data.pop_back();
    return data;
}

void send(tcp::socket &socket, const string& _data)
{
    const string data = _data + "\n";
    // cout << "Sending: " << data << endl;
    boost::asio::write(socket, boost::asio::buffer(data));
}

int main() 
{  
    boost::asio::io_service io_service;  
    tcp::socket socket(io_service);  
    socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), PORT));

    int pid = getpid();

    string connect_msg = "Client (pid: " + std::to_string(pid) + ") connected";

    send(socket, connect_msg);

    cout << "Waiting for server to start game..." << endl;

    BattleshipGame game = BattleshipGame(10);

    string reply = read(socket);
    string saveFile;
    if(reply == "load")
    {
        saveFile = read(socket);
        game.load(saveFile, PLAYER);
    }
    else
    {
        game.display();
        game.setup(PLAYER);
    }

    send(socket, "done");
    reply = read(socket);
    if(reply != "done")
    {
        cout << "Synchronization error" << endl;
        exit(1);
    }


    game.display();
    while(!game.isOver())
    {
        cout << "Waiting for opponent to play" << endl;
        string x_str = read(socket);
        string y_str = read(socket);

        int x = std::stoi(x_str);
        int y = std::stoi(y_str);

        string status;
        if(game.defend(x, y))
            status = "miss";
        else
            status = "hit";

        game.display();
        
        send(socket, status);
        if(game.isOver())
            break;
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
        x = cx - 'A';
        y = cy - 'A';
        
        send(socket, std::to_string(x));
        send(socket, std::to_string(y));

        status = read(socket);
        if(status == "hit")
            game.attack(x, y, true);
        else
            game.attack(x, y, false);
        game.display();
    }
    game.showStats();
    socket.close();
    return 0;  
}  