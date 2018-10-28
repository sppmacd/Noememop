package com.sppmacd.pomemeon;

import java.io.IOException;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;

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
import android.widget.EditText;

import com.sppmacd.pomemeon.network.ConnectAsyncTask;

public class CommandLineActivity extends Activity
{
	public static Socket client;
	private Context context;
	public boolean running;
	public long userID;
	public static CommandLineActivity instance;
	
	public void error(String error)
	{
		Toast.makeText(context, error, Toast.LENGTH_SHORT).show();
		System.err.println(error);
	}
	
	private void onConnectButton()
	{
		Intent intent2 = new Intent(this, ConnectingActivity.class);
		this.startActivity(intent2);
	}
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		context = this.getApplicationContext();
		setContentView(R.layout.activity_command_line);
		
		if(savedInstanceState != null)
		{
			userID = savedInstanceState.getLong("LONG_USERID");
			((EditText)findViewById(R.id.ip1)).setText(savedInstanceState.getString("STRING_LASTCONNECTIP"));
		}
		
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
	protected void onStart()
	{
		super.onStart();
		
		instance = this;
		running = true;
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
		bundle.putString("STRING_LASTCONNECTIP", ((EditText)findViewById(R.id.ip1)).getText().toString());
	}
}
