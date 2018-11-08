#include "Pomemeon.hpp"
#include "PomemeonType.hpp"
#include "../PMSServer.hpp"
#include "../Network/Command.hpp"
#include "../GPSCoords.hpp"

namespace pms
{
    Pomemeon::Pomemeon(int pId, PomemeonType* pType, GPSCoords& coords, Player* own)
    : id(pId), type(pType), coordinates(coords), owner(own)
    {
        this->pickTimer = 0;
    }

    Pomemeon::Pomemeon(int id, DataNode node)
    : coordinates(0.0,0.0)
    {
        this->id = id;
        this->pickTimer=stoi(node.args[0]);
        this->name=node.args[1];
        this->type=PMSServer::getInstance()->findTypeByID(stoi(node.args[2]));
        this->description=node.args[3];
        this->coordinates.setNS(stod(node.args[4]));
        this->coordinates.setEW(stod(node.args[5]));
        this->textureFile=node.args[6];
        this->owner = PMSServer::getInstance()->findPlayerByID(stoi(node.args[7]));
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
            CashStat cs = this->type->pick(this->owner, picker);
            if(cs == Success)
                picker->pickCount++;
            return cs;
        }
        else
            return CannotMake;
    }

    CashStat Pomemeon::place(Player* owner)
    {
        return this->type->place(owner);
    }

    DataNode Pomemeon::getNode()
    {
        return DataNode{{
        to_string(pickTimer),
        name,
        to_string(type->getID()),
        description,
        to_string(coordinates.getNS()),
        to_string(coordinates.getEW()),
        textureFile,
        to_string(owner->getUserID())}};
    }

    void Pomemeon::update(Time elapsed)
    {
        this->pickTimer += elapsed.asSeconds();
    }

    Command Pomemeon::getCommand()
    {
        return Command(SCmdPomemeon,
        {to_string(id),
        to_string(pickTimer),
        name,
        to_string(type->getID()),
        description,
        to_string(coordinates.getNS()),
        to_string(coordinates.getEW()),
        textureFile,
        to_string(owner->getUserID())});
    }

    GPSCoords Pomemeon::getCoordinates()
    {
        return this->coordinates;
    }
}
