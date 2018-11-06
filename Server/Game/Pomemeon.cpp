#include "Pomemeon.hpp"
#include "PomemeonType.hpp"

namespace pms
{
    Pomemeon::Pomemeon(int pId, PomemeonType* pType, GPSCoords& coords, Player* own)
    : id(pId), type(pType), coordinates(coords), owner(own)
    {
        this->pickTimer = 0;
    }

    void Pomemeon::setData(string pName, string pDesc, string texture)
    {
        this->name = pName;
        this->description = pDesc;
        this->textureFile = texture;
    }

    CashStat Pomemeon::pick(Player* picker)
    {
        if(picker->tickTimer.getElapsedTime().asSeconds() > 3600*24 || picker->pickCount == 0)
        {
            picker->tickTimer.restart();
            CashStat cs = this->type->pick(this->owner, picker));
             if(cs==Success) picker->pickCount++;
            return cs;
        }
        else
            return CannotMake;
    }

    CashStat Pomemeon::place(Player* owner)
    {
        return this->type->place(owner);
    }

    void Pomemeon::update(Time elapsed)
    {
        this->pickTimer += elapsed.asSeconds();
    }

    string Pomemeon::getCommand()
    {
        return "pms:pomemeon\1" +
        to_string(id) + "\1" +
        to_string(pickTimer) + "\1" +
        name + "\1" +
        to_string(type->getID()) + "\1" +
        description + "\1" +
        to_string(coordinates.getNS()) + "\1" +
        to_string(coordinates.getEW()) + "\1" +
        textureFile + "\1" +
        to_string(owner->getUserID());
    }

    GPSCoords Pomemeon::getCoordinates()
    {
        return this->coordinates;
    }
}
