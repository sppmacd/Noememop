#include "PomemeonType.hpp"
#include "Player.hpp"

namespace pms
{
    PomemeonType::PomemeonType(int id, float costForPicker, int profitForPicker, int costForPlacer, float profitForPlacer, string name)
    {
        this->id = id;
        this->pickCost = costForPicker;
        this->profit = profitForPicker;
        this->placeCost = costForPlacer;
        this->placeProfit = profitForPlacer;
    }

    CashStat PomemeonType::pick(Player* owner, Player* picker)
    {
        bool tooenough = false;
        tooenough = !picker->tryRemovePoints(this->pickCost);
        bool toomany = false;
        if(!tooenough)
        {
            toomany = !picker->tryAddCash(this->profit);

            if(!toomany)
            {
                owner->addPoints(this->placeProfit);
                return Success;
            }
            else
            {
                return TooManyCoins;
            }
        }
        else
        {
            return NotEnoughCoins;
        }
    }
    
    int PomemeonType::getID()
    {
        return id;
    }

    CashStat PomemeonType::place(Player* owner)
    {
        bool tooenough = false;
        tooenough = !owner->tryRemoveCash(this->placeCost);

        if(!tooenough)
            return Success;
        else
            return NotEnoughCoins;
    }
}
