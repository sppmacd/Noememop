package com.pomemeon.game.shop;

import com.pomemeon.game.Player;
import com.pomemeon.main.PomemeonClient;

public class ShopElement 
{
	public int cost;
	public EnumProductType type;
	
	public ShopElement(EnumProductType type, int cost)
	{
		this.cost = cost;
		this.type = type;
	}
	
	public boolean onBuy(Player player)
	{
		player.tryRemoveCash(PomemeonClient.instance.serverHandler, cost);
	}
}
