#include "PMSServer.hpp"
#include "Game/PomemeonType.hpp"

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

        log(Debug, "Creating SocketSelector...");
        this->socketSelector

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
