package com.sppmacd.pomemeon;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Looper;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;
import android.widget.Button;

public class CommandLineActivity extends Activity
{
	private InetSocketAddress serverAddress;
	public static Socket client;
	private Context context;
	public boolean running;
	public long userID;
	
	public void error(String error)
	{
		CommandActivity.logStr += error + "\n";
	}
	
	private void onConnectButton()
	{
		Thread connectThread = new Thread(new Runnable()
		{
			@Override
			public void run() 
			{
				Looper.prepare();
				
				try
				{
					client = new Socket();
					client.connect(serverAddress);
					client.getOutputStream().write(new String("pmc:setuserid " + Long.toString(userID)).getBytes());
				} 
				catch (IOException e)
				{
					error("Cannot connect to server: " + e.getMessage());
				}
				
				while(running)
				{
					CommandActivity.networkLoop();
				}
			}
	
		}, "Network Thread");
		
		connectThread.start();
		
		Intent intent = new Intent(this, CommandActivity.class);
		this.startActivity(intent);
	}
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		context = this.getApplicationContext();
		setContentView(R.layout.activity_command_line);
		
		running = true;
		
		if(savedInstanceState != null)
			userID = savedInstanceState.getLong("LONG_USERID");
		
		((Button)findViewById(R.id.connect)).setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				CommandLineActivity.this.onConnectButton();
			}
			
		});
	}
	
	@Override
	protected void onStop()
	{
		super.onStop();
		
		running = false;
	}
	
	@Override
	protected void onSaveInstanceState(Bundle bundle)
	{
		super.onSaveInstanceState(bundle);
		bundle.putLong("LONG_USERID", userID);
	}
}
