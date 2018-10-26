#pragma once

namespace pms
{
    class Player
    {
        int cashCount;
        bool isDailyRewardCollected;
        int id;
        int totalPoints;
        int currentPoints;
        int maxPoints;
        int leaderboardPlace;
        bool needUpdate;

    public:
        Player(int id);
        bool tryAddCash(int count);
        bool tryRemoveCash(int count);
        bool tryCollectReward();
        void addPoints(int count);
        bool tryRemovePoints(int count);
        void ensureUpdated();
        void setReward();
    };
}
