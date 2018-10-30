#pragma once

#include <../GPSCoords.hpp>

namespace pms
{
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

    public:
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
    };
}
