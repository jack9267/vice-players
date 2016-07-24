function onServerInit()
{
	print("Admin started");
}

function onPlayerCommand(playerid, cmdtext)
{
	print("onPlayerCommand(" + playerid + ", " + cmdtext + ");");
	local cmd = split(cmdtext, " ");
	if(cmd[0] == "/kick")
	{
		local kick = cmd[1].tointeger();
		if(!isConnected(kick)) {
			sendPlayerMessage(playerid, 0x00FF00FF, "Player not connected!");
			return;
		}
		kickPlayer(kick);
		
	}
	if(cmd[0] == "/ban")
	{
		local mask = cmd[1];
		banIP(mask);
		sendPlayerMessage(playerid, 0x00FF00FF, "Mask " + mask + " banned.");
	}
}