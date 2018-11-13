package com.pomemeon.main;

import org.osmdroid.views.MapView;

import com.pomemeon.activity.MainActivity;
import com.pomemeon.activity.R;
import com.pomemeon.game.PomemeonServerHandler;
import com.pomemeon.gui.EnumGUIType;
import com.pomemeon.gui.events.Events;

import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.view.View.OnClickListener;

public class PomemeonClient 
{
	public static final PomemeonServerHandler serverHandler;
	public MainActivity activityInstance;
	public MapView map;
	
	public PomemeonClient(MainActivity activity)
	{
		instance = this;
		activityInstance = activity;
	}
	
	public static PomemeonClient instance;
	
	public void setOnClick(final Activity activity, final Events.EventHandler onClickFunc, int viewId)
	{
		View view = activity.findViewById(viewId);
		view.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				onClickFunc.run(activity);
			}
			
		});
	}
	
	public void changeActivity(Activity changer, Class<?> activityClass)
	{
		changer.startActivity(new Intent(changer.getApplicationContext(), activityClass));
	}
}
