package com.pomemeon.game;

import org.osmdroid.util.GeoPoint;

public class Player
{
	public int cashCount;
	public boolean isDailyRewardCollected;
    public float totalPoints;
    public float currentPoints;
    public int xp;
    public int leaderboardPlace;
    public int logCount;
    public int level;

    public int pickCount;

    public boolean tryRemoveCash(PomemeonServerHandler server, int count)
    {
    	return server.tryRemoveCash(count, this);
    }
    
    public boolean tryCollectReward(PomemeonServerHandler server)
    {
		return server.tryCollectReward(this);
    }

    public boolean tryRemovePoints(PomemeonServerHandler server, float count)
    {
    	return server.tryRemovePoints(count, this);
    }
};

