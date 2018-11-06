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
        this->xp = 0;
        this->pickCount = 0;

        this->isDailyRewardCollected = false;

        this->needUpdate = true;

        this->ensureUpdated();

        this->leaderboardPlace = PMSServer::getInstance()->getPlayerList()->size();
        this->level = 1;
        this->freePomemeonPlaced = false;

        this->logCount = 0;
    }

    DataNode Player::getNode()
    {
        return DataNode{{
        to_string(this->cashCount),
        to_string(this->currentPoints),
        to_string(this->xp),
        to_string(this->totalPoints),
        to_string(this->leaderboardPlace),
        to_string(this->level),
        (this->freePomemeonPlaced?"true":"false"),
        (this->isDailyRewardCollected?"true":"false"),
        to_string(this->logCount)}};
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
    void Player::addXP(int count)
    {
        this->xp += count;
        this->needUpdate = true;
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

        this->totalPoints += count;
        this->needUpdate = true;
    }

    Command Player::getCommand()
    {
        return Command(SCmdUserData,
        {to_string(this->cashCount),
        to_string(this->currentPoints),
        to_string(this->xp),
        to_string(this->totalPoints),
        to_string(this->leaderboardPlace),
        to_string(this->level),
        (this->freePomemeonPlaced?"true":"false"),
        (this->isDailyRewardCollected?"true":"false"),
        to_string(this->logCount)});
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
                if(player->getUserID() != this->getUserID() && player->xp < this->xp)
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
