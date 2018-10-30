#pragma once

#include "../config.hpp"
#include "CashStat.hpp"
#include "Player.hpp"

namespace pms
{
    class PomemeonType
    {
        float pickCost;
        int profit;
        int id;
        string codeName;
        int placeCost;
        float placeProfit;
        float radius;

    public:
        PomemeonType(int id, float costForPicker, int profitForPicker, int costForPlacer, float profitForPlacer, float radius, string name);
        CashStat pick(Player* owner, Player* picker);
        CashStat place(Player* owner);
        int getID();
        float getRadius() {return radius;};
    };
}
