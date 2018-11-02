package com.sppmacd.pomemeon;

import java.io.IOException;
import java.net.Socket;

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
		
		Intent intent2 = new Intent(this, CommandActivity.class);
		this.startActivity(intent2);
	}
}
