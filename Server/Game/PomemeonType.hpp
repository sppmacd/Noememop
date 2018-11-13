#pragma once

#include "../config.hpp"
#include "CashStat.hpp"
#include "Player.hpp"

namespace pms
{
    class PomemeonType
    {
        float pickCost;
        int minProfit;
        int maxProfit
        int id;
        string codeName;
        int placeCost;
        float placeProfit;
        float radius;

    public:
        PomemeonType(int id, int minCash, int maxCash, float radius, string name);
        CashStat pick(Player* owner, Player* picker, int prof);
        CashStat place(Player* owner, int prof);
        int getID();
        float getRadius() {return radius;};
    };
}
