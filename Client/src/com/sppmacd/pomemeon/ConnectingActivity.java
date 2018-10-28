package com.sppmacd.pomemeon;

import java.io.IOException;
import java.net.Socket;

import com.sppmacd.pomemeon.network.ConnectAsyncTask;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Looper;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;

public class ConnectingActivity extends Activity 
{
	public static ConnectingActivity instance;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		instance = this;
		setContentView(R.layout.activity_connecting);
	}
	
	protected void onStart()
	{
		super.onStart();
		
		new ConnectAsyncTask().execute(this);
	}
}
