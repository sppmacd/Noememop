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
    
    string Pomemeon::getCommand()
    {
        return "pms:pomemeon\255" + 
        to_string(id) + "\255" +
        to_string(pickTimer) + "\255" +
        name + "\255" +
        to_string(type->getID()) + "\255" +
        description + "\255" +
        to_string(coordinates.getNS()) + "\255" +
        to_string(coordinates.getEW()) + "\255" +
        textureFile + "\255" +
        to_string(owner->getID());
    }
    
    GPSCoords Pomemeon::getCoordinates()
    {
        return this->coordinates;
    }
}
