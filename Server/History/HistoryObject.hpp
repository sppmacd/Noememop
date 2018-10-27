#pragma once

namespace pms
{
    class HistoryObject
    {
        Player* player;
        Pomemeon* picked;
        int id;

    public:
        HistoryObject(int id, Player* picker, Pomemeon* obj) {}
    };
}
