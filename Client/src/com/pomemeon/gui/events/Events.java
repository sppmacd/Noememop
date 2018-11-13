package com.pomemeon.gui.events;

import com.pomemeon.activity.ShopActivity;
import com.pomemeon.main.PomemeonClient;

import android.app.Activity;

public class Events 
{
	public static interface EventHandler
	{
		public void run(Activity caller);
	}
	
	public static class MainActivity
	{
		public static class Click
		{
			public static final EventHandler shop = new EventHandler()
			{
				public void run(Activity caller)
				{
					PomemeonClient.instance.changeActivity(caller, ShopActivity.class);
				}
			};
			public static final EventHandler stats = new EventHandler()
			{
				public void run(Activity caller)
				{
					PomemeonClient.instance.changeActivity(caller, StatsActivity.class);
				}
			};
		}
	}
}
