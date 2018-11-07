#pragma once

#include "../GPSCoords.hpp"
#include "../Network/Command.hpp"
#include "../Util/DataFile.hpp"

namespace pms
{
    class PomemeonType;

    class Player
    {
        int cashCount;
        bool isDailyRewardCollected;
        int id;
        float totalPoints;
        float currentPoints;
        int xp;
        int leaderboardPlace;
        bool needUpdate;
        int logCount;
        int level;
        GPSCoords lastPos;
        Clock lastPosUpdateTime;

    public:
        bool freePomemeonPlaced;
        int pickCount;
        Clock tickTimer;

        Player(int id);
        Player(DataNode node);
        bool tryAddCash(int count);
        bool tryRemoveCash(int count);
        bool tryCollectReward();
        void addPoints(float count);
        bool tryRemovePoints(float count);
        void addXP(int count);
        void ensureUpdated();
        void setReward();
        void login();
        int getUserID();
        bool updateCoords(GPSCoords coords);
        GPSCoords& getLastCoords() {return lastPos;}
        bool isPomemeonUnlocked(PomemeonType* type);
        Command getCommand();
        DataNode getNode();
    };
}
