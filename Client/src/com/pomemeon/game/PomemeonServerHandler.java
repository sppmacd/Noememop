package com.pomemeon.game;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetSocketAddress;
import java.net.Socket;

public class PomemeonServerHandler 
{
	public Socket socket;
	public BufferedWriter output;
	public BufferedReader input;
	public int userId;
	
	public void sendCommand(String command) throws IOException
	{
		output.write("pmc:" + command);
		output.newLine();
	}
	
	public PomemeonServerHandler(int userID, String ipAddress) throws IOException
	{
		userId = userID;
		socket.connect(new InetSocketAddress(ipAddress, 12346));
		output = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
		input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		
		sendCommand("setuserid|"+Integer.toString(userId));
	}

	public boolean tryRemoveCash(int count, Player player) 
	{
		
		return false;
	}

	public boolean tryRemovePoints(float count, Player player) {
		// TODO Auto-generated method stub
		return false;
	}

	public boolean tryCollectReward(Player player) {
		// TODO Auto-generated method stub
		return false;
	}

}
