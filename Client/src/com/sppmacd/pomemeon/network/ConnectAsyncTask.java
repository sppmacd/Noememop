package com.sppmacd.pomemeon.network;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import com.sppmacd.pomemeon.CommandActivity;
import com.sppmacd.pomemeon.CommandLineActivity;
import com.sppmacd.pomemeon.ConnectingActivity;
import com.sppmacd.pomemeon.R;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Looper;
import android.widget.EditText;

public class ConnectAsyncTask extends AsyncTask<CommandLineActivity, Void, Void> 
{
    private Exception exception;
    private CommandLineActivity caller;

    protected void doInBackground() 
    {
    	final String ip = ((EditText)caller.findViewById(R.id.ip1)).getText().toString();
		
		Thread connectThread = new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				Looper.prepare();
				
				try
				{
					Intent intent = new Intent(caller, ConnectingActivity.class);
					caller.startActivity(intent);
					
					caller.client = new Socket(ip, 12346);
					caller.client.getOutputStream().write(new String("pmc:setuserid " + Long.toString(caller.userID)).getBytes());
					CommandActivity.successfullyConnected = true;
				} 
				catch (IOException e)
				{
					caller.error("Cannot connect to server: " + e.getMessage());
					CommandActivity.successfullyConnected = false;
				}
				
				if(CommandActivity.successfullyConnected)
					System.out.println("Successfully connected to " + ip);
				
				while(caller.running)
				{
					CommandActivity.networkLoop();
				}
			}
	
		}, "Network Thread");
		
		connectThread.start();
		
		Intent intent = new Intent(caller, CommandActivity.class);
		caller.startActivity(intent);
    }

	@Override
	protected Void doInBackground(CommandLineActivity... params) 
	{
		caller = params[0];
		doInBackground();
		return null;
	}
}