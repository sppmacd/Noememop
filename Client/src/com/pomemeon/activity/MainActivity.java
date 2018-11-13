package com.pomemeon.activity;

import org.osmdroid.config.Configuration;
import org.osmdroid.tileprovider.tilesource.TileSourceFactory;

import com.pomemeon.gui.events.Events;
import com.pomemeon.main.PomemeonClient;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.View.OnClickListener;

public class MainActivity extends Activity 
{	
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		this.setContentView(R.layout.activity_main);
		
	    PomemeonClient client = new PomemeonClient(this);
	    
	    Context ctx = getApplicationContext();
        Configuration.getInstance().load(ctx, PreferenceManager.getDefaultSharedPreferences(ctx));
	    
	    client.map = this.findViewById(R.id.MA_mapview_MAIN);
	    client.map.setTileSource(TileSourceFactory.MAPNIK);
	    
	    client.setOnClick(this, Events.MainActivity.Click.shop, R.id.MA_button_SHOP);
		client.setOnClick(this, Events.MainActivity.Click.stats, R.id.MA_button_STATS);
	}
	
	public void onResume()
	{
        super.onResume();
        PomemeonClient.instance.map.onResume();
    }

    public void onPause()
    {
        super.onPause();
        PomemeonClient.instance.map.onPause();
    }
}
