#pragma once

class Player;
class Pomemeon;
class PomemeonType;
class HistoryObject;

namespace pms
{
    class PMSServer
    {
        vector<Player*> players;
        vector<Pomemeon*> pomemeons;
        vector<PomemeonType*> pomemeonTypeRegistry;
        vector<HistoryObject*> history;

        bool running;

    public:
        void start();
        void loop();
        void networkLoop();
    };
}
