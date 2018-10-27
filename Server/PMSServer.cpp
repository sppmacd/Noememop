#include "PMSServer.hpp"
#include "Game/PomemeonType.hpp"
#include "Network/Client.hpp"
#include "Game/Player.hpp"

#include <SFML/Network.hpp>
using namespace sf;

namespace pms
{
    PMSServer::PMSServer()
    {

    }

    PMSServer::~PMSServer()
    {
        for(Player* player: players)
            delete player;
        for(Pomemeon* pomemeon: pomemeons)
            delete pomemeon;
        for(PomemeonType* pomemeontype: pomemeonTypeRegistry)
            delete pomemeontype;
        for(HistoryObject* historyobject: history)
            delete historyobject;
    }

    void PMSServer::loop()
    {
        this->networkLoop();
    }

    void PMSServer::networkLoop()
    {
        if(this->socketSelector.wait(seconds(1.f)))
        {
            if(this->socketSelector.isReady(this->serverSocket)) //Someone tries to connect to server
            {
                TcpSocket* socket2 = new TcpSocket;
                if(!this->serverSocket.accept(*socket2))
                {
                    delete socket2;
                }
                else
                {
                    // Someone connected
                    clients.push_back(Client(0,socket2));
                    this->socketSelector.add(*socket2);
                }
            }

            if(!this->clients.empty())
            {
                for(Client* client: this->clients)
                {
                    if(this->socketSelector.isReady(*client->socket)) //Something was sent to server
                    {
                        void* data;
                        int received;
                        if(client->socket->receive(data, 128, received) == Socket::Done)
                        {
                            string command(data);

                            this->parseCommand(client->socket, command);
                        }
                        else
                        {
                            disconnect(socket);
                        }
                    }
                }
            }
        }
    }

    void PMSServer::disconnect(TcpSocket* sck)
    {
        for(auto it = this->clients.begin(); it != this->clients.end(); it++)
        {
            if(it->socket == sck)
            {
                this->clients.erase(it);
                break;
            }
        }

        log(Info, "IP: " + to_string(sck->getRemoteAddress()) + ":" + to_string(sck->getRemotePort()) + " disconnected");
        this->socketSelector.remove(*sck);
        delete sck;
    }

    void PMSServer::parseCommand(Client* sender, string command)
    {
        string cmd;
        vector<string> args;
        int lastp = 0;

        for(int i = 0; i < command.size(); i++)
        {
            if(command[i] == ' ' || command[i] == '\0')
            {
                if(lastp == 0)
                    cmd = command.substr(lastp, i-lastp-1);
                else
                    args.push_back(command.substr(lastp, i-lastp-1));

                lastp = i;
            }
        }

        this->processCommands(sender, cmd, args.data(); args.size());
    }

    bool PMSServer::processCommands(Client* sender, string command, string* argv, int argc)
    {
        switch(command)
        {
            case "pms:setuserid":
            {
                if(argc == 1)
                {
                    int uid = stoi(argv);
                    if(uid == 0) //the player is a new player!
                    {
                        Player* player = new Player(this->players.size());
                        player->setReward();
                        sender->userID = player->getUserID();

                        return true;
                    }
                    else
                    {
                        sender->userID = uid;
                        this->findPlayerByID(uid)->setReward();
                        return true;
                    }
                }
                break;
            }
            case "pmc"
        }

        return false;
    }

    Player* PMSServer::findPlayerByID(int userId)
    {
        for(int i = 0; i < players.size(); i++)
            if(players[i]->getUserID() == userID)
                return players[i];
        return NULL;
    }

    void PMSServer::close()
    {
        this->running = false;
    }

    void PMSServer::start()
    {
        log(Info, "Starting dedicated Pomemeon server...");
        this->running = true;
        this->registerTypes();

        log(Debug, "Setting up Network Core...");
        this->serverSocket.listen(12346);
        this->socketSelector.add(this->serverSocket);

        while(this->running)
        {
            this->loop();
        }

        log(Info, "Closing server...");
    }

    PMSServer* PMSServer::getInstance()
    {
        static PMSServer server;
        return &server;
    }

    void PMSServer::registerTypes()
    {
        log(Debug, "Registering Pomemeon types...");
        this->pomemeonTypeRegistry.push_back(new PomemeonType(0, 1, 10, 1000, "small"));
        this->pomemeonTypeRegistry.push_back(new PomemeonType(1, 1, 19, 2000, "medium"));
        this->pomemeonTypeRegistry.push_back(new PomemeonType(2, 2, 80, 10000, "big"));
    }
}
