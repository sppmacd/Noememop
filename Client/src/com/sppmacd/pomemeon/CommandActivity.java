package com.sppmacd.pomemeon;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class CommandActivity extends Activity
{
	public static Socket socket;
	public static BufferedReader input;
	public static BufferedWriter output;
	public static Handler handler;
	public static TextView logText;
	public static EditText commandEditText;
	public static String stringToSend;
	public static boolean connected;
	
	public static CommandActivity instance;
	
	static class CloseCommandLine implements Runnable
	{
		private String msg;
		
		public CloseCommandLine(String str)
		{
			this.msg = str;
		}
		
		public void run()
		{
			Toast.makeText(instance.getApplicationContext(), this.msg, Toast.LENGTH_LONG).show();
			connected = false;
			instance.finish();
		}
	}
	
	static class UpdateUIThread implements Runnable
	{
		private String msg;
		
		public UpdateUIThread(String str)
		{
			this.msg = str;
		}
		
		public void run()
		{
			logText.setText(logText.getText() + this.msg);
		}
	}
	
	public class OnSend implements View.OnClickListener
	{
		public void onClick(View v) 
		{
			stringToSend = commandEditText.getText().toString();
			commandEditText.setText("");
		}
	}
	
	static class NetworkThread implements Runnable
	{
		public NetworkThread()
		{
			try
			{
				input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				output = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}

		@Override
		public void run() 
		{
			while (!Thread.currentThread().isInterrupted() && connected)
			{
				try
				{
					//read
					String read = input.readLine();
					if (read != null)
					{
						handler.post(new UpdateUIThread(read));
					}
					else
					{
						return;
					}
					
					//write
					String send = stringToSend;
					stringToSend = null;
					
					if(send != null)
					{
						output.append(send);
						output.flush();
					}
				}
				catch(IOException e)
				{
					if(handler != null)
						handler.post(new CloseCommandLine("Cannot receive or send data: " + e.getMessage()));
					e.printStackTrace();
				}
			}
		}
	}
	
    static class ConnectThread implements Runnable
	{
		public void run()
		{
			Object sync = new Object();
			
			synchronized(sync)
			{
				try {
					sync.wait(1000);
				} catch (InterruptedException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				
				socket = null;
				
				try
				{
					InetAddress serverAddr = InetAddress.getByName(CommandLineActivity.ip);
					socket = new Socket(serverAddr,12346);
					
					connected = true;
					
					NetworkThread connect = new NetworkThread();
					new Thread(connect).start();
					
					Toast.makeText(instance.getApplicationContext(), "Successfully Connected!", Toast.LENGTH_SHORT).show();
					return;
				}
				catch(Exception e)
				{
					if(handler != null)
						handler.post(new CloseCommandLine("Cannot connect to server: " + e.getMessage()));
					e.printStackTrace();
				}
			}
		}
	}
	
	public void onCreate(Bundle bundle)
	{
		instance = this;
		
		super.onCreate(bundle);
		this.setContentView(R.layout.activity_commands);
		
		handler = new Handler();
		logText = (TextView)this.findViewById(R.id.log);
		commandEditText = (EditText)this.findViewById(R.id.commandLine);
		this.findViewById(R.id.send).setOnClickListener(new OnSend());
		
		Toast.makeText(instance.getApplicationContext(), "Connecting...", Toast.LENGTH_SHORT).show();
	}
	
	public void onStop()
	{
		super.onStop();
		
		try {
			socket.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}