package com.sppmacd.pomemeon;

import java.io.IOException;
import java.net.Socket;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.EditText;

public class CommandActivity extends Activity 
{
	public static boolean connected;
	
	public static String logString;
	public static String stringToSend;
	public static String receivedString;
	
	public static CommandActivity instance;
	public Handler handler;
	public Runnable onSend = new Runnable()
	{
		public void run()
		{
			updateLogString("CLIENT SENT: " + stringToSend + "\n");
			((EditText) instance.findViewById(R.id.commandLine)).setText("");
		}
	};
	
	public Runnable onReceive = new Runnable()
	{
		public void run()
		{
			updateLogString("SERVER SENT: " + receivedString + "\n");
			((TextView) instance.findViewById(R.id.log)).setText(logString);
		}
	};
	
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_commands);
		handler = new Handler();
		
		((Button) findViewById(R.id.send)).setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v)
			{
				stringToSend = ((EditText) instance.findViewById(R.id.commandLine)).getText().toString();
			}
		});
		
		instance = this;
	}
	
	public void onStop()
	{
		super.onStop();
		
		try 
		{
			CommandLineActivity.client.close();
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		
		connected = false;
	}
	
	public static void error(IOException exception)
	{
		exception.printStackTrace();
		
		try
		{
			CommandLineActivity.client.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		
		connected = false;
	}
	
	public static void updateLogString(String strToAdd)
	{
		logString += strToAdd;
	}
	
	public static void networkLoop()
	{
		Socket socket = CommandLineActivity.client;
		
		stringToSend = null;
		
		while(true)
		{
			if(connected)
			{
				try 
				{
					// receive
					if(socket.getInputStream().available() > 0)
					{
						byte[] b = new byte[socket.getInputStream().available()];
						socket.getInputStream().read(b);
						updateLogString(new String(b));
						
						instance.handler.post(instance.onReceive);
					}
					
					// send
					if(stringToSend != null)
					{
						if(!stringToSend.isEmpty())
						{
							instance.handler.post(instance.onSend);
							
							System.out.println("STRTS=" + stringToSend);
							socket.getOutputStream().write(stringToSend.getBytes());
							stringToSend = null;
						}
					}
				} 
				catch (IOException e) 
				{
					e.printStackTrace();
					break;
				}
			}
		}
		
		connected = false;
		instance.finish();
		try {
			CommandLineActivity.client.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
