package com.sppmacd.pomemeon;

import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.EditText;

public class CommandActivity extends Activity 
{
	public static String logStr;
	public static String strToSend;
	public static boolean needSend;
	public static CommandActivity instance;
	public static boolean successfullyConnected;
	public static boolean logMustBeUpdated;
	
	public static void breakpoint(String bp)
	{
		try 
		{
			throw new Exception(bp);
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void onCreate(Bundle bundle)
	{
		super.onCreate(bundle);
		
		instance = this;
		
		setContentView(R.layout.activity_commands);
		
		final Handler handler = new Handler();
	    final Runnable r = new Runnable() 
	    {
	    	public void run() 
	    	{
				handler.postDelayed(this, 100);
				
				if(logMustBeUpdated)
				{
					((TextView)instance.findViewById(R.id.log)).setText(logStr);
					logMustBeUpdated = false;
				}
	        }
	    };
	    handler.postDelayed(r, 0);
		
		((Button)findViewById(R.id.send)).setOnClickListener(
		new View.OnClickListener()
		{
			@Override
			public void onClick(View v) 
			{
				needSend = true;
				strToSend = ((EditText)findViewById(R.id.commandLine)).getText().toString();
				((EditText)findViewById(R.id.commandLine)).setText("");	
				breakpoint("Clicked Send button");
			}
		});
	}
	
	public static void update()
	{
		logMustBeUpdated = true;
	}
	
	public static void networkLoop()
	{
		if(successfullyConnected)
		{
			// receive data
			try 
			{
				if(CommandLineActivity.client.getInputStream().available() > 0)
				{
					byte[] arr = new byte[CommandLineActivity.client.getInputStream().available()];
					CommandLineActivity.client.getInputStream().read(arr);
					logStr += new String(arr);
					
					update();
				}
			} 
			catch (IOException e) 
			{
				try 
				{
					CommandLineActivity.client.close();
				} 
				catch (IOException e1) 
				{
					e1.printStackTrace();
					CommandLineActivity.instance.error("Receiving error: " + e1.getMessage());
					instance.finish();
				}
			}
			
			// send commands
			if(needSend)
			{
				breakpoint("Sending");
				
				logStr += strToSend;
				update();
				try 
				{
					CommandLineActivity.client.getOutputStream().write(strToSend.getBytes());
				} 
				catch (IOException e) 
				{
					try 
					{
						CommandLineActivity.client.close();
					} 
					catch (IOException e1) 
					{
						e1.printStackTrace();
						CommandLineActivity.instance.error("Sending error: " + e1.getMessage());
					}
					instance.finish();
				}
				needSend = false;
				strToSend = "";
			}
		}
		else
		{
			if(instance != null)
				instance.finish();
		}
	}
}
