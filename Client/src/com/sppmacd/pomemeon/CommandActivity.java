package com.sppmacd.pomemeon;

import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.graphics.Typeface;
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
	public static boolean sent;
	public static boolean isReady;
	
	public static CommandActivity instance;
	public Handler handler;
	public Runnable onSend = new Runnable()
	{
		public void run()
		{
			updateLogString("CLIENT SENT: " + stringToSend);
			stringToSend = null;
			((EditText) instance.findViewById(R.id.commandLine)).setText("");
			((TextView) instance.findViewById(R.id.log)).setText(logString);
		}
	};
	
	public Runnable onReceive = new Runnable()
	{
		public void run()
		{
			updateLogString("SERVER SENT: " + receivedString);
			((TextView) instance.findViewById(R.id.log)).setText(logString);
		}
	};
	
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_commands);
		
		Typeface tf = Typeface.createFromAsset(getAssets(), "fonts/consolas.ttf");
		TextView tv = (TextView) findViewById(R.id.log);
		tv.setTypeface(tf);
		
		handler = new Handler();
		
		logString = "";
		
		((Button) findViewById(R.id.send)).setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v)
			{
				stringToSend = ((EditText) instance.findViewById(R.id.commandLine)).getText().toString();
				sent = false;
			}
		});
		
		instance = this;
		isReady = true;
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
	
	public static void parseCommand(String command)
	    {
		String cmd = new String();
		List<String> args = new ArrayList();
		int lastp = 0;

		for(int i = 0; i < command.length(); i++)
		{
		    if((command.charAt(i) == '\255' || i == command.length() - 1) && i != 0)
		    {
			if(lastp == 0)
			    cmd = command.substring(lastp, i+1).trim();
			else
			    args.add(command.substring(lastp, i+1).trim());

			lastp = i;
		    }
		}

		// process commands

		if(cmd.equals("pms:userid") && args.size() == 1)
		{
			CommandLineActivity.instance.userID = Long.parseLong(args.get(0));
			updateLogString("Setting user id to " + args.get(0));
		}
		else if(cmd.equals("pms:disconnect") && args.size() == 1)
		{
			CommandLineActivity.client.close();
			updateLogString("Disconnected: " + args[0]); //used if the players cheated (XD)
		}
		else
			updateLogString("Undefined or invalid command: " + command);
	    }
	
	public static void updateLogString(String strToAdd)
	{
		logString += strToAdd + "\n";
	}
	
	public static void networkLoop()
	{
		Socket socket = CommandLineActivity.client;
		
		stringToSend = null;
		
		while(true)
		{
			if(connected && instance.isReady)
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
						
						parseCommand(new String(b));
					}
					
					// send
					if(stringToSend != null && !sent)
					{
						if(!stringToSend.isEmpty())
						{
							String strtosend = new String(stringToSend) + "\0";
							strtosend.replaceAll("/\xFF/g", "\255");
							instance.handler.post(instance.onSend);
							
							System.out.println("STRTS=" + strtosend);
							socket.getOutputStream().write(strtosend.getBytes());
							
							sent = true;
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
