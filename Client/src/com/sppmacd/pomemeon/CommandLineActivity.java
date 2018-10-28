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
	public static long userID;
	public static CommandLineActivity instance;
	public int classUUID;
	
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
		
		classUUID = this.hashCode();
		
		instance = this;
		
		context = this.getApplicationContext();
		setContentView(R.layout.activity_command_line);
		
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
	protected void onRestoreInstanceState(Bundle bundle)
	{
		super.onRestoreInstanceState(bundle);
		
		System.out.println("Restoring instance state...");
		userID = bundle.getLong("LONG_USERID");
		((EditText)findViewById(R.id.ip1)).setText(bundle.getString("STRING_LASTCONNECTIP"));
	}
	
	@Override
	protected void onStart()
	{
		super.onStart();
		
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
