#pragma once

#include "config.hpp"

#include "Network/Command.hpp"

namespace pms
{
    class Player;
    class Pomemeon;
    class PomemeonType;
    class HistoryObject;
    class Client;

    class PMSServer
    {
        vector<Player*> players;
        vector<Pomemeon*> pomemeons;
        vector<PomemeonType*> pomemeonTypeRegistry;
        vector<HistoryObject*> history;

        bool running;

        SocketSelector socketSelector;
        TcpListener serverSocket;
        vector<Client*> clients;

    public:
        int tick;

        PMSServer();
        ~PMSServer();

        void start();
        void loop();
        void networkLoop();
        void close();
        static PMSServer* getInstance();
        void registerTypes();
        void send(Client* recv, string command);
        void sendToAll(string command);
        void parseCommand(Client* sender, string command);
        bool processCommands(Client* sender, Command command);
        void disconnect(TcpSocket* sck, string reason);
        void sendCommand(Command command, Client* client);

        void savePlayer(int id);
        void savePomemeon(int id);

        Player* findPlayerByID(int userId);
        Pomemeon* findPomemeonByID(int id);
        PomemeonType* findTypeByID(int id);
        vector<Player*>* getPlayerList();
    };
}
