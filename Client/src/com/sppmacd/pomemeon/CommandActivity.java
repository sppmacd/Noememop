package com.sppmacd.pomemeon;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.InetAddress;
import java.net.Socket;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.EditText;

class CommandActivity extends Activity
{
	public static Socket socket;
	public BufferedReader input;
	public BufferedWriter output;
	public Handler handler;
	public EditText logEditText;
	public EditText commandEditText;
	public String stringToSend;
	
	class UpdateUIThread implements Runnable
	{
		private String msg;
		
		public UpdateUIThread(String str)
		{
			this.msg = str;
		}
		
		public void run()
		{
			logEditText.setText(logEditText.getText() + this.msg);
		}
	}
	
	class OnSend implements View.OnClickListener
	{
		public void onClick(View v) 
		{
			stringToSend = commandEditText.getText().toString();
			commandEditText.setText("");
		}
	}
	
	class ConnectThread implements Runnable
	{
		public ConnectThread()
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
			while (!Thread.currentThread().isInterrupted())
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
						Thread thread = new Thread(new NetworkThread());
						thread.start();
						return;
					}
					
					//write
					String send = stringToSend;
					stringToSend = null;
					
					output.append(send);
				}
				catch(IOException e)
				{
					e.printStackTrace();
				}
			}
		}
	}
	
	class NetworkThread implements Runnable
	{
		public void run()
		{
			socket = null;
			
			try
			{
				InetAddress serverAddr = InetAddress.getByName(CommandLineActivity.ip);
				socket = new Socket(serverAddr, 12346);
				
				ConnectThread connect = new ConnectThread();
				new Thread(connect).start();
				return;
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}
	}
	
	public void onCreate(Bundle bundle)
	{
		super.onCreate(bundle);
		this.setContentView(R.layout.activity_commands);
		
		handler = new Handler();
		logEditText = this.findViewById(R.id.log);
		commandEditText = this.findViewById(R.id.commandLine);
		this.findViewById(R.id.send).setOnClickListener(new OnSend());
	}
}