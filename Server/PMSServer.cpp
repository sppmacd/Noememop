#include "PMSServer.hpp"
#include "Game/PomemeonType.hpp"
#include "Network/Client.hpp"
#include "Game/Player.hpp"
#include "Game/Pomemeon.hpp"
#include "History/HistoryObject.hpp"
#include "Game/CashStat.hpp"
#include "Util/DataFile.hpp"
#include "main.h"
#include <cstring>

namespace pms
{
    PMSServer::PMSServer()
    {
        //load server

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
        tick++;
        static Clock tickClock;
        this->networkLoop();

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

                        Socket::Status status = client->socket->receive((void*)&data, 1024LL, received);

                        string command((char*)data);
                        log(Debug, "Client: (" + to_string(client->userID) + ") " + client->socket->getRemoteAddress().toString() + string(" sent command: '") + command + "'");

                        if(status == Socket::Done)
                        {
                            this->parseCommand(client, command);
                        }
                        else
                        {
                            this->disconnect(client->socket, "ERR_DISCONNECTED_" + to_string(status) + ": " + string(strerror(errno)));
                        }
                    }
                }
            }
        }
    }

    void PMSServer::disconnect(TcpSocket* sck, string reason)
    {
        for(auto it = this->clients.begin(); it != this->clients.end(); it++)
        {
            if((*it)->socket == sck)
            {
                send(*it, "pms:disconnect\1"+reason);
                this->clients.erase(it);
                break;
            }
        }

        log(Info, "IP: " + sck->getRemoteAddress().toString() + ":" + to_string(sck->getRemotePort()) + " has disconnected. Reason: " + reason);

        this->socketSelector.remove(*sck);
        delete sck;
    }

    void PMSServer::parseCommand(Client* sender, string command)
    {
        Command cmd(command);

        if(!this->processCommands(sender, cmd))
        {
            log(Error, "Invalid command: " + command);
            sendCommand(Command(pms::SCmdErr, {"Invalid command!"}), sender);
        }
    }

    void PMSServer::send(Client* recv, string command)
    {
        recv->socket->send(string(command + "\n\0").c_str(), command.size());
    }

    void PMSServer::sendCommand(Command command, Client* client)
    {
        if(client != NULL)
            this->send(client, command.toString());
        else
            this->sendToAll(command.toString());
    }

    void PMSServer::sendToAll(string command)
    {
        for(Client* recv: clients)
            recv->socket->send(string(command + "\n\0").c_str(), command.size());
    }

    void PMSServer::savePlayer(int id)
    {
        log(Debug, "Saving player: " + to_string(id));
        auto save = [this,id]()
        {
            DataFile file(DTPlayer);
            Player* player = findPlayerByID(id);
            if(player != NULL)
            {
                file.setNode(player->getNode(), id-1);
                file.saveSize(players.size());
            }
        };

        Thread thread(save);
        thread.launch();
    }

    void PMSServer::savePomemeon(int id)
    {
        log(Debug, "Saving pomemeon: " + to_string(id));
        auto save = [this,id]()
        {
            DataFile file(DTPomemeon);
            Pomemeon* pomemeon = findPomemeonByID(id);
            if(pomemeon != NULL)
            {
                file.setNode(pomemeon->getNode(), id-1);
                file.saveSize(pomemeons.size());
            }
        };
        Thread thread(save);
        thread.launch();
    }

    bool PMSServer::processCommands(Client* sender, Command cmd)
    {
        string errMsg;
        string command = cmd.command;
        string* argv = cmd.args.data();
        int argc = cmd.args.size();

        if(command == "pmc:setuserid")
        {
            if(argc == 1)
            {
                int uid = stoi(argv[0]);
                if(uid == 0) //the player is a new player!
                {
                    Player* player = new Player(DataFile(DTPlayer).getSize() + 1);
                    player->login();
                    player->setReward();
                    sender->userID = player->getUserID();
                    players.push_back(player);
                    sendCommand(Command(SCmdUserID, {to_string(player->getUserID())}), sender);
                    log(Info, "A new player " + to_string(player->getUserID()) + " has logged in to server");
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
                        disconnect(sender->socket, "ERR_INVALID_USER_ID"); //cheats
                    }
                }

                savePlayer(sender->userID);
                return true;
                // TODO save player - other thread
            }
            errMsg="Invalid syntax!";
        }
        else if(command == "pmc:requestpomemeons") //pmc:requestpomemeons <posNS> <posEW> <radiusKMS>
        {
            if(argc == 3)
            {
                double posNS = stod(argv[0]);
                double posEW = stod(argv[1]);
                double rad = stod(argv[2]);
                GPSCoords playerCoords(posNS,posEW);

                int c = DataFile(DTPomemeon).getSize();
                for(int i = 1; i <= c; i++)
                {
                    Pomemeon* pomemeon = findPomemeonByID(i);
                    if(pomemeon->getCoordinates().distance(playerCoords) < rad)
                        sendCommand(pomemeon->getCommand(), sender);
                }

                return true;
            }
            errMsg="Invalid syntax!";
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
                    disconnect(sender->socket, "ERR_INVALID_OBJECT_ID"); //cheats
                }

                double dist = pomemeon->getCoordinates().distance(picker->getLastCoords());

                if(dist < pomemeon->getType()->getRadius() && picker != pomemeon->getOwner())
                {
                    CashStat stat = pomemeon->pick(picker);
                    sendCommand(Command(SCmdCashStat, {to_string(stat)}), sender);

                    //TODO add history object
                    savePlayer(sender->userID);
                    savePomemeon(pomemeon->getID());
                    //TODO save player and pomemeon - other thread
                }
                else
                {
                    send(sender, "pms:err\1Cannot Pick Pomemeon");
                }
                return true;
            }
            errMsg="Invalid syntax!";
        }
        else if(command == "pmc:place") //pmc:place <coordsNS> <coordsEW> <type>
        {
            if(argc == 3)
            {
                double posNS = stod(argv[0]);
                double posEW = stod(argv[1]);
                Player* owner = findPlayerByID(sender->userID);
                GPSCoords coords(posNS,posEW);

                Pomemeon* pomemeon = new Pomemeon(DataFile(DTPomemeon).getSize() + 1, findTypeByID(stoi(argv[2])), coords, owner);
                double dist = pomemeon->getCoordinates().distance(owner->getLastCoords());

                bool dst = dist < pomemeon->getType()->getRadius();
                bool unlock = owner->isPomemeonUnlocked(pomemeon->getType());

                if(dst && unlock)
                {
                    CashStat stat = pomemeon->place(owner);
                    if(stat == Success)
                    {
                        pomemeons.push_back(pomemeon);
                        sendCommand(Command(SCmdRequestPMData, {to_string(pomemeon->getID())}), sender);
                        savePomemeon(pomemeon->getID());
                    }
                    sendCommand(Command(SCmdCashStat, {to_string(stat)}), sender);

                    //TODO add history object
                    //TODO save pomemeon and player - other thread
                    savePlayer(sender->userID);
                }
                else
                {
                    if(!dst) sendCommand(Command(SCmdErr, {"Cannot Place Pomemeon: invalid distance: " + to_string(dist) + " > " + to_string(pomemeon->getType()->getRadius())}), sender);
                    if(!unlock) sendCommand(Command(SCmdErr, {"Cannot Place Pomemeon: this type is not unlocked: " + to_string(pomemeon->getType()->getID())}), sender);
                }
                return true;
            }
            errMsg="Invalid syntax!";
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
                //TODO save pomemeon - other thread
                savePomemeon(pomemeon->getID());

                return true;
            }
            errMsg="Invalid syntax!";
        }
        else if(command == "pmc:setuserpos") //pmc:setuserpos <posNS> <posEW>
        {
            if(argc == 2)
            {
                Player* player = findPlayerByID(sender->userID);
                player->updateCoords(GPSCoords(stod(argv[0]), stod(argv[1])));

                //TODO save player - other thread
                savePlayer(sender->userID);

                return true;
            }
            errMsg="Invalid syntax!";
        }
        else if(command == "pmc:requestuserdata") //pmc:requestuserdata <userID>
        {
            if(argc == 1)
            {
                Player* player = findPlayerByID(stoi(argv[0]));
                player->ensureUpdated();
                send(sender, player->getCommand().toString());
                return true;
            }
            errMsg="Invalid syntax!";
        }
        else if(command == "pmc:stopserver" && sender->userID <= 3) //only admin
        {
            this->running = false;
            return true;
        }
        else
            errMsg="Invalid syntax!";

        sendCommand(Command(SCmdErr, {errMsg}), sender);

        return false;
    }

    Pomemeon* PMSServer::findPomemeonByID(int id)
    {
        for(Pomemeon* pomemeon: pomemeons)
            if(pomemeon->getID() == id)
                return pomemeon;

        DataFile file(DTPomemeon);
        DataNode node = file.getNode(id-1);
        if(node.args.empty())
            return NULL;
        else
        {
            log(Debug, "Loaded pomemeon: " + to_string(id));
            Pomemeon* pomemeon = new Pomemeon(id, node);
            pomemeons.push_back(pomemeon);
            return pomemeon;
        }
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

        DataFile file(DTPlayer);
        DataNode node = file.getNode(userId-1);
        if(node.args.empty())
            return NULL;
        else
        {
            log(Debug, "Loaded player: " + to_string(userId));
            Player* player = new Player(userId,node);
            players.push_back(player);
            return player;
        }
        return NULL;
    }

    void PMSServer::close()
    {
        this->running = false;
    }

    void PMSServer::start()
    {
        log(ImportantInfo, "");
        log(ImportantInfo, "-----------------------------------");
        log(ImportantInfo, "--       Pomemeon Server         --");
        log(ImportantInfo, "--      Version: Alpha 1.0       --");
        log(ImportantInfo, "--    Pomemeon Team (C) 2018     --");
        log(ImportantInfo, "-----------------------------------");
        log(ImportantInfo, "");

        log(Info, "Printing IP Config:");

        system("ipconfig");

        cout << endl;

        log(Info, "Starting dedicated Pomemeon server...");
        this->running = true;
        this->registerTypes();

        log(Debug, "Setting up Network Core...");
        this->serverSocket.listen(12346);
        this->socketSelector.add(this->serverSocket);

        while(this->running)
        {
            this->loop();
            pms::sendStats(to_string(this->pomemeons.size()) + " " + to_string(this->clients.size()) + " " + to_string(this->players.size()));
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
