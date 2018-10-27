#pragma once

#include "config.hpp"

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
        PMSServer();
        ~PMSServer();

        void start();
        void loop();
        void networkLoop();
        void close();
        static PMSServer* getInstance();
        void registerTypes();
        void parseCommand(Client* sender, string command);
        bool processCommands(Client* sender, string command, string* argv, int argc);
        void disconnect(TcpSocket* sck);
        Player* findPlayerByID(int userId);
        vector<Player*>* getPlayerList();
    };
}
