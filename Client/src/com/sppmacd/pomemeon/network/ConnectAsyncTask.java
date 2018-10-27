package com.sppmacd.pomemeon.network;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import com.sppmacd.pomemeon.CommandActivity;
import com.sppmacd.pomemeon.CommandLineActivity;
import com.sppmacd.pomemeon.R;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Looper;
import android.widget.EditText;

class ConnectAsyncTask extends AsyncTask<CommandLineActivity, Void, Void> {

    private Exception exception;
    private CommandLineActivity caller;

    protected void doInBackground() 
    {
    	String ip = ((EditText)caller.findViewById(R.id.ip1)).getText().toString();
    	final InetSocketAddress serverAddress = new InetSocketAddress(12346);
		try 
		{
			serverAddress.getAddress().getByName(ip);
		} 
		catch (UnknownHostException e1) 
		{
			caller.error(e1.getMessage());
		}
		
		Thread connectThread = new Thread(new Runnable()
		{
			@Override
			public void run() 
			{
				Looper.prepare();
				
				try
				{
					caller.client = new Socket();
					caller.client.connect(serverAddress);
					caller.client.getOutputStream().write(new String("pmc:setuserid " + Long.toString(caller.userID)).getBytes());
				} 
				catch (IOException e)
				{
					caller.error("Cannot connect to server: " + e.getMessage());
				}
				
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
		return null;
	}
}