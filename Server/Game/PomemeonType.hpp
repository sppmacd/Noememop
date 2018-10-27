#pragma once

namespace pms
{
    class PomemeonType
    {
        int pickCost;
        int profit;
        int id;
        string codeName;
        int placeCost;

    public:
        PomemeonType(int id, int costForPicker, int profitForPicker, int costForPlacer, float costForPicker, string name);
        CashStat pick(Player* owner, Player* picker);
        CashStat place(Player* owner);
    };
}
