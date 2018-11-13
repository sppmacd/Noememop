#include "PomemeonType.hpp"
#include "Player.hpp"

namespace pms
{
    PomemeonType::PomemeonType(int id, int minCash, int maxCash, float radius, string name)
    {
        this->codeName = name;
        this->radius = radius;
        this->id = id;
        this->minProfit = minCash;
        this->maxProfit = maxCash;
    }

    CashStat PomemeonType::pick(Player* owner, Player* picker, int prof)
    {
        int profit = prof;
        float profPicker = pow(profit,1.1)/10;
        float costPicker = profPicker / 10;

        bool tooenough = false;
        tooenough = !picker->tryRemovePoints(costPicker);
        bool toomany = false;
        if(!tooenough)
        {
            toomany = !picker->tryAddCash(profit);

            if(!toomany)
            {
                owner->addPoints(profPicker);
                owner->addXP(profPicker);
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

    CashStat PomemeonType::place(Player* owner, int prof)
    {
        int profit = prof;

        //log(Info, "The owner command is " + owner->getCommand());
        if(owner->freePomemeonPlaced || this->getID() != 0)
        {
            bool tooenough = false;
            tooenough = !owner->tryRemoveCash(this->placeCost);

            if(!tooenough)
            {
                log(Info, "User " + to_string(owner->getUserID()) + " tried to place PomemeonType " + to_string(this->getID()) + ": Success");
                return Success;
            }
            else
            {
                log(Warning, "User " + to_string(owner->getUserID()) + " tried to place PomemeonType " + to_string(this->getID()) + ": NotEnoughCoins");
                return NotEnoughCoins;
            }
        }
        else
        {
            log(Info, "User " + to_string(owner->getUserID()) + " tried to place free Pomemeon: Success");
            owner->freePomemeonPlaced = true;
            return Success;
        }
    }
}
