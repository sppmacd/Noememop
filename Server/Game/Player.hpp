#pragma once

#include "../GPSCoords.hpp"

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
        float maxPoints;
        int leaderboardPlace;
        bool needUpdate;
        int logCount;
        int level;
        GPSCoords lastPos;
        Clock lastPosUpdateTime;


    public:
        bool freePomemeonPlaced;
        Clock tickTimer;

        Player(int id);
        bool tryAddCash(int count);
        bool tryRemoveCash(int count);
        bool tryCollectReward();
        void addPoints(float count);
        bool tryRemovePoints(float count);
        void ensureUpdated();
        void setReward();
        void login();
        int getUserID();
        bool updateCoords(GPSCoords coords);
        GPSCoords& getLastCoords() {return lastPos;}
        bool isPomemeonUnlocked(PomemeonType* type);
    };
}
