#include "Pomemeon.hpp"
#include "PomemeonType.hpp"

namespace pms
{
    Pomemeon::Pomemeon(int pId, PomemeonType* pType, GPSCoords coords, Player* own)
    : id(pId), type(pType), coordinates(coords), owner(own)
    {

    }

    void Pomemeon::setData(string pName, string pDesc, string texture)
    {
        this->name = pName;
        this->description = pDesc;
        this->textureFile = texture;
    }

    CashStat Pomemeon::pick(Player* picker)
    {
        return this->type->pick(this->owner, picker);
    }

    CashStat Pomemeon::place(Player* owner)
    {
        return this->type->place(owner);
    }

    void Pomemeon::update(Time elapsed)
    {
        this->pickTimer += elapsed.asSeconds();
    }
}
