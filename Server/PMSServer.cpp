#include "PMSServer.hpp"
#include "Game/PomemeonType.hpp"
#include "Network/Client.hpp"
#include "Game/Player.hpp"
#include "Game/Pomemeon.hpp"
#include "History/HistoryObject.hpp"

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
        for(Client* client: clients)
            delete client;
    }

    void PMSServer::loop()
    {
        this->networkLoop();

        for(Player* player: players)
        {
            player->ensureUpdated();
        }
    }

    void PMSServer::networkLoop()
    {
        log(Info, "Started network loop...");
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
                    Client* client = new Client(0,socket2);
                    clients.push_back(client);
                    this->socketSelector.add(*socket2);
                }
            }

            if(!this->clients.empty())
            {
                for(Client* client: this->clients)
                {
                    if(this->socketSelector.isReady(*client->socket)) //Something was sent to server
                    {
                        char data[128];
                        size_t received;
                        if(client->socket->receive((void*)&data, 128LL, received) == Socket::Done)
                        {
                            string command((char*)data);

                            this->parseCommand(client, command);
                        }
                        else
                        {
                            disconnect(client->socket);
                        }
                    }
                }
            }
        }
    }

    #include <SFML/Network.hpp>
    using namespace sf;

    void PMSServer::disconnect(TcpSocket* sck)
    {
        for(auto it = this->clients.begin(); it != this->clients.end(); it++)
        {
            if((*it)->socket == sck)
            {
                this->clients.erase(it);
                break;
            }
        }

        log(Info, "IP: " + sck->getRemoteAddress().toString() + ":" + to_string(sck->getRemotePort()) + " disconnected");
        this->socketSelector.remove(*sck);
        delete sck;
    }

    void PMSServer::parseCommand(Client* sender, string command)
    {
        string cmd;
        vector<string> args;
        int lastp = 0;

        for(unsigned int i = 0; i < command.size(); i++)
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

        this->processCommands(sender, cmd, args.data(), args.size());
    }

    bool PMSServer::processCommands(Client* sender, string command, string* argv, int argc)
    {
        if(command == "pms:setuserid")
        {
            if(argc == 1)
            {
                size_t s;
                int uid = stoi(argv[0], &s, 10);
                if(uid == 0) //the player is a new player!
                {
                    Player* player = new Player(this->players.size());
                    player->login();
                    player->setReward();
                    sender->userID = player->getUserID();
                    players.push_back(player);
                    log(Info, "A new player " + to_string(player->getUserID()) + " was logged in to server");
                    return true;
                }
                else
                {
                    sender->userID = uid;
                    Player* player = this->findPlayerByID(uid);
                    player->setReward();
                    player->login();
                    log(Debug, "A player " + to_string(player->getUserID()) + " was logged in to server");

                    return true;
                }
            }
        }

        return false;
    }

    vector<Player*>* PMSServer::getPlayerList()
    {
        return &players;
    }

    Player* PMSServer::findPlayerByID(int userId)
    {
        for(unsigned int i = 0; i < players.size(); i++)
            if(players[i]->getUserID() == userId)
                return players[i];
        return NULL;
    }

    void PMSServer::close()
    {
        this->running = false;
    }

    void PMSServer::start()
    {
        log(Warning, "");
        log(Warning, "-----------------------------------");
        log(Warning, "--       Pomemeon Server         --");
        log(Warning, "--      Version: Alpha 1.0       --");
        log(Warning, "--    Pomemeon Team (C) 2018     --");
        log(Warning, "-----------------------------------");
        log(Warning, "");

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
        this->pomemeonTypeRegistry.push_back(new PomemeonType(0, 1.f, 10, 1000, 2.f, "small"));
        this->pomemeonTypeRegistry.push_back(new PomemeonType(1, 1.f, 19, 2000, 2.f, "medium"));
        this->pomemeonTypeRegistry.push_back(new PomemeonType(2, 2.f, 80, 10000, 4.f, "big"));
    }
}
