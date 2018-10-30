#pragma once

#include "../config.hpp"
#include "CashStat.hpp"
#include "../GPSCoords.hpp"
#include "PomemeonType.hpp"

namespace pms
{
    class Pomemeon
    {
        int pickTimer;
        int id;
        string name;
        PomemeonType* type;
        string description;
        GPSCoords coordinates;
        string textureFile;
        Player* owner;

    public:
        Pomemeon(int pId, PomemeonType* pType, GPSCoords coords, Player* own);
        void setData(string pName, string pDesc, string texture);
        CashStat pick(Player* picker);
        CashStat place(Player* owner);
        void update(Time elapsed);
        GPSCoords getCoordinates();
        string getCommand();
    };
}
