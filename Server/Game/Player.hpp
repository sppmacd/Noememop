#pragma once

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

    public:
        Player(int id);
        bool tryAddCash(int count);
        bool tryRemoveCash(int count);
        bool tryCollectReward();
        void addPoints(int count);
        bool tryRemovePoints(int count);
        void ensureUpdated();
        void setReward();
        void login();
        void getUserID();
    };
}
