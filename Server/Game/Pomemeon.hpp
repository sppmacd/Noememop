#pragma once

namespace pms
{
    class Pomemeon
    {
        int pickTimer;
        int id;
        string name;
        PomemonType* type;
        string description;
        GPSCoords coordinates;
        string textureFile;
        Player* owner;

    public:
        Pomemeon(int pId, PomemeonType* pType, GPSCoords coords, Player* own);
        void setData(string pName, string pDesc, string texture);
        CashStat pick(Player* picker);
        CashStat place(Player* owner);
    };
}
