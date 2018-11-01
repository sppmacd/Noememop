#include "PMSServer.hpp"
#include "Game/PomemeonType.hpp"
#include "Network/Client.hpp"
#include "Game/Player.hpp"
#include "Game/Pomemeon.hpp"
#include "History/HistoryObject.hpp"
#include "Game/CashStat.hpp"

namespace pms
{
    PMSServer::PMSServer()
    {
        log(Error, "Wypierdaj!");
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
        static Clock tickClock;
        this->networkLoop();

        for(Player* player: players)
        {
            player->ensureUpdated();
        }
        Time elapsed = tickClock.restart();
        for(Pomemeon* pomemeon: pomemeons)
        {
            pomemeon->update(elapsed);
        }
    }

    void PMSServer::networkLoop()
    {
        if(this->socketSelector.wait(seconds(2.f)))
        {
            if(this->socketSelector.isReady(this->serverSocket)) //Someone tries to connect to server
            {
                TcpSocket* socket2 = new TcpSocket;
                if(this->serverSocket.accept(*socket2) != Socket::Done)
                {
                    delete socket2;
                }
                else
                {
                    // Someone connected
                    Client* client = new Client(0,socket2);
                    clients.push_back(client);
                    this->socketSelector.add(*socket2);
                    log(Debug, "IP: " + socket2->getRemoteAddress().toString() + ":" + to_string(socket2->getRemotePort()) + " connected");
                }
            }

            if(!this->clients.empty())
            {
                for(Client* client: this->clients)
                {
                    if(this->socketSelector.isReady(*client->socket)) //Something was sent to server
                    {
                        char data[1024] = {0};
                        size_t received;
                        if(client->socket->receive((void*)&data, 1024LL, received) == Socket::Done)
                        {
                            string command((char*)data);

                            log(Debug, "Client: (" + to_string(client->userID) + ") " + client->socket->getRemoteAddress().toString() + string(" sent command: '") + command + "'");

                            this->parseCommand(client, command);
                        }
                        else
                        {
                            send(client, "pms:disconnect\1ERR_DISCONNECTED");
                            disconnect(client->socket);
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
            if((*it)->socket == sck)
            {
                this->clients.erase(it);
                break;
            }
        }

        log(Info, "IP: " + sck->getRemoteAddress().toString() + ":" + to_string(sck->getRemotePort()) + " has disconnected");
        this->socketSelector.remove(*sck);
        delete sck;
    }

    void PMSServer::parseCommand(Client* sender, string command)
    {
        string cmd;
        vector<string> args;
        int lastp = 0;

        for(unsigned int i = 0; i < command.size()+1; i++)
        {
            if(command[i] == 1 || command[i] == '\0' || i == command.size())
            {
                if(lastp == 0)
                    cmd = command.substr(lastp, i-lastp);
                else
                    args.push_back(command.substr(lastp, i-lastp));

                lastp = i+1;
            }
        }

        log(Info, "Server command: '" + cmd + "'");
        for(string& str: args)
            log(Debug, "Server argument: '" + str + "'");

        if(!this->processCommands(sender, cmd, args.data(), args.size()))
        {
            log(Error, "Invalid command: " + command);
        }
    }

    void PMSServer::send(Client* recv, string command)
    {
        recv->socket->send(command.c_str(), command.size());
    }

    void PMSServer::sendToAll(string command)
    {
        for(Client* recv: clients)
            recv->socket->send(command.c_str(), command.size());
    }

    bool PMSServer::processCommands(Client* sender, string command, string* argv, int argc)
    {
        if(command == "pmc:setuserid")
        {
            if(argc == 1)
            {
                int uid = stoi(argv[0]);
                if(uid == 0) //the player is a new player!
                {
                    Player* player = new Player(this->players.size() + 1);
                    player->login();
                    player->setReward();
                    sender->userID = player->getUserID();
                    players.push_back(player);
                    send(sender, "pms:userid " + to_string(player->getUserID()));
                    log(Info, "A new player " + to_string(player->getUserID()) + " has logged in to server");
                    return true;
                }
                else
                {
                    sender->userID = uid;
                    Player* player = this->findPlayerByID(uid);

                    if(player != NULL)
                    {
                        player->login();
                        player->setReward();
                        log(Debug, "A player " + to_string(player->getUserID()) + " has logged in to server");
                    }
                    else
                    {
                        log(Error, "The player with id " + to_string(uid) + " was not found!");
                        send(sender, "pms:disconnect\1ERR_INVALID_USER_ID");
                        disconnect(sender->socket); //cheats
                    }

                    return true;
                }
            }
        }
        else if(command == "pmc:requestpomemeons") //pms:requestpomemeons <posNS> <posEW> <radiusKMS>
        {
            if(argc == 3)
            {
                double posNS = stod(argv[0]);
                double posEW = stod(argv[1]);
                double rad = stod(argv[2]);
                GPSCoords playerCoords(posNS,posEW);

                for(Pomemeon* pomemeon: pomemeons)
                {
                    if(pomemeon->getCoordinates().distance(playerCoords) < rad)
                        send(sender, pomemeon->getCommand());
                }

                return true;
            }
        }
        else if(command == "pmc:pick") //pmc:pick <id>
        {
            if(argc == 1)
            {
                Pomemeon* pomemeon = findPomemeonByID(stoi(argv[0]));
                Player* picker = findPlayerByID(sender->userID);

                if(pomemeon == NULL)
                {
                    log(Error, "The Pomemeon with id " + to_string(stoi(argv[0])) + " was not found!");
                    send(sender, "pms:disconnect\1ERR_INVALID_OBJECT_ID");
                    disconnect(sender->socket); //cheats
                }

                double dist = pomemeon->getCoordinates().distance(picker->getLastCoords());

                if(dist < pomemeon->getType()->getRadius() && picker != pomemeon->getOwner())
                {
                    CashStat stat = pomemeon->pick(picker);
                    send(sender, "pms:cashstat\1"+to_string(stat));

                    //TODO add history object
                }
            }
        }
        else if(command == "pmc:place") //pmc:place <coordsNS> <coordsEW> <type>
        {
            if(argc == 3)
            {
                double posNS = stod(argv[0]);
                double posEW = stod(argv[1]);
                Player* owner = findPlayerByID(sender->userID);
                GPSCoords coords(posNS,posEW);

                Pomemeon* pomemeon = new Pomemeon(pomemeons.size() + 1, findTypeByID(stoi(argv[3])), coords, owner);
                double dist = pomemeon->getCoordinates().distance(owner->getLastCoords());

                if(dist < pomemeon->getType()->getRadius() && owner->isPomemeonUnlocked(pomemeon->getType()))
                {
                    CashStat stat = pomemeon->place(owner);
                    if(stat == Success)
                    {
                        pomemeons.push_back(pomemeon);
                        send(sender, "pms:requestpmdata\1"+to_string(pomemeon->getID()));
                    }
                    send(sender, "pms:cashstat\1"+to_string(stat));

                    //TODO add history object
                }
            }
        }
        else if(command == "pmc:setpmdata")  //pmc:setpmdata <id> <name> <desc> <textureData>
        {
            if(argc == 4)
            {
                Pomemeon* pomemeon = findPomemeonByID(stoi(argv[0]));
                Player* setter = findPlayerByID(sender->userID);
                if(pomemeon->getOwner() == setter)
                {
                    pomemeon->setData(argv[1],argv[2],argv[3]); //TODO texture!!!
                }
            }
        }

        return false;
    }

    Pomemeon* PMSServer::findPomemeonByID(int id)
    {
        for(Pomemeon* pomemeon: pomemeons)

            if(pomemeon->getID() == id)
                return pomemeon;
        return NULL;
    }

    PomemeonType* PMSServer::findTypeByID(int id)
    {
        for(PomemeonType* pomemeon: pomemeonTypeRegistry)

            if(pomemeon->getID() == id)
                return pomemeon;
        return pomemeonTypeRegistry[0];

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
        this->pomemeonTypeRegistry.push_back(new PomemeonType(0, 0.2f, 10, 1000, 1.f, 0.01f, "small"));
        this->pomemeonTypeRegistry.push_back(new PomemeonType(1, 0.2f, 19, 2000, 2.f, 0.02f, "medium"));
        this->pomemeonTypeRegistry.push_back(new PomemeonType(2, 0.4f, 80, 10000, 4.f, 0.02f, "big"));
    }
}
