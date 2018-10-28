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

public class ConnectAsyncTask extends AsyncTask<ConnectingActivity, Void, Void> 
{
    private Exception exception;
    private ConnectingActivity caller;

    protected void doInBackground() 
    {
		if(Looper.myLooper() == null)
			Looper.prepare();
    	
    	final String ip = ((EditText)CommandLineActivity.instance.findViewById(R.id.ip1)).getText().toString();
    	
		try
		{
			CommandLineActivity.instance.client = new Socket(ip, 12346);
			CommandLineActivity.instance.client.getOutputStream().write(new String("pmc:setuserid " + Long.toString(CommandLineActivity.instance.userID)).getBytes());
			CommandActivity.successfullyConnected = true;
			
			Intent intent2 = new Intent(caller, CommandActivity.class);
			caller.startActivity(intent2);
			
			caller.finish();
		} 
		catch (IOException e)
		{
			CommandLineActivity.instance.error("Cannot connect to server: " + e.getMessage());
			CommandActivity.successfullyConnected = false;
			
			caller.finish();
		}
		
		if(CommandActivity.successfullyConnected)
			System.out.println("Successfully connected to " + ip);
    }

	@Override
	protected Void doInBackground(ConnectingActivity... params) 
	{
		caller = params[0];
		doInBackground();
		return null;
	}
}