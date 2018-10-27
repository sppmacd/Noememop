package com.sppmacd.pomemeon;

import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class CommandActivity extends Activity 
{
	public static String logStr;
	public static String strToSend;
	public static boolean needSend;
	public static CommandActivity instance;
	
	public void onCreate(Bundle bundle)
	{
		instance = this;
	}
	
	public static void update()
	{
		TextView log = (TextView)instance.findViewById(R.id.commandLine);
		log.setText(logStr);
	}
	
	public static void networkLoop()
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
				instance.finish();
			}
		}
		
		// send commands
		if(needSend)
		{
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
				}
				instance.finish();
			}
			needSend = false;
		}
	}
}
