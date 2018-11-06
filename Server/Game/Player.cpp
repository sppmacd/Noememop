#include "Player.hpp"
#include "../PMSServer.hpp"
#include "../config.hpp"
#include "PomemeonType.hpp"

namespace pms
{
    Player::Player(int id) : lastPos(52, 16)
    {
        this->id = id;
        this->cashCount = 100; //START CASH AND POINTS
        this->totalPoints = 100.f;
        this->currentPoints = 100.f;
        this->maxPoints = 100.f;
        this->pickCount = 0;

        this->isDailyRewardCollected = false;

        this->needUpdate = true;

        this->ensureUpdated();

        this->leaderboardPlace = PMSServer::getInstance()->getPlayerList()->size();
        this->level = 1;
        this->freePomemeonPlaced = false;

        this->logCount = 0;
    }

    bool Player::tryAddCash(int count)
    {
        if(this->cashCount + count <= 1000000)
        {
            this->cashCount += count;
            this->needUpdate = true;
            return true;
        }
        return false;
    }

    bool Player::isPomemeonUnlocked(PomemeonType* type)
    {
        return pow(type->getID(),2) <= level;
    }
    bool Player::updateCoords(GPSCoords coords)
    {
        double dist = coords.distance(this->lastPos);

        if(dist / lastPosUpdateTime.restart().asSeconds() < 0.1)
            lastPos = coords;
        else
            return false;
        return true;
    }

    bool Player::tryRemoveCash(int count)
    {
        if(this->cashCount - count >= 0)
        {
            this->cashCount -= count;
            this->needUpdate = true;
            return true;
        }
        return false;
    }

    bool Player::tryCollectReward()
    {
        if(!this->isDailyRewardCollected)
        {
            this->addPoints(1);
            this->isDailyRewardCollected = false;
            return true;
        }
        return false;
    }

    void Player::addPoints(float count)
    {
        this->currentPoints += count;
        this->maxPoints += count;
        this->totalPoints += count;
        if(this->currentPoints > this->maxPoints)
            this->maxPoints = this->currentPoints;

        this->needUpdate = true;
    }

    string Player::getCommand()
    {
        return "pms:userdata\1"
        +to_string(this->cashCount)+"\1"
        +to_string(this->currentPoints)+"\1"
        +to_string(this->maxPoints)+"\1"
        +to_string(this->totalPoints)+"\1"
        +to_string(this->leaderboardPlace)+"\1"
        +to_string(this->level)+"\1"
        +(this->freePomemeonPlaced?"true":"false")+"\1"
        +(this->isDailyRewardCollected?"true":"false")+"\1"
        +to_string(this->logCount);
    }

    bool Player::tryRemovePoints(float count)
    {
        if(this->currentPoints - count >= 0)
        {
            this->currentPoints -= count; //we are updating ONLY current points, not TOTAL POINTS
            this->needUpdate = true;
            return true;
        }
        return false;
    }

    void Player::ensureUpdated()
    {
        if(this->needUpdate)
        {
            // Update level
            this->level = std::pow(std::log(totalPoints+1.f),1.2f)+1;

            // Update leaderboard place
            vector<Player*>* players = PMSServer::getInstance()->getPlayerList();

            this->leaderboardPlace = players->size();

            for(int i = players->size() - 1; i >= 0; i--)
            {
                Player* player = players->at(i);
                if(player->getUserID() != this->getUserID() && player->maxPoints < this->maxPoints)
                    this->leaderboardPlace--;
            }

            this->needUpdate = false;
        }
    }

    void Player::setReward()
    {
        this->isDailyRewardCollected = false;
    }

    void Player::login()
    {
        this->logCount++;
    }

    int Player::getUserID()
    {
        return id;
    }
}
