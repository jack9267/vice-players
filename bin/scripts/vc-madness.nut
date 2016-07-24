local testCP = -1;
local testText = -1;

function onServerInit()
{
	print("---------------------------\n");
	print("Vice Madness started\n");
	print("---------------------------\n");


	// Player classes
	addPlayerClass(0, 1, -673.2837, -1325.3040, 11.0715, 116.6886, 4, 1, 3, 35, 19, 15);
	addPlayerClass(1, 87, -673.2837, -1325.3040, 11.0715, 116.6886, 9, 1, 22, 70, 21, 13);
	addPlayerClass(2, 93, -673.2837, -1325.3040, 11.0715, 116.6886, 10, 1, 23, 80, 19, 23);

	setModeName("Vice Madness");
	setMapName("Blood Vice-City");

	//testCP = createCheckpoint(-670.2837, -1325.3040, 11.0715, 2.0);
	testText = createText(0xFFFF00FF, "Tahoma", 16, 45.0, 50.0, "You in cp!");
	
	// Some test vehicles
	//local v1 = createVehicle(149, -681.5270, -1329.4580, 10.8290, 292.6041, 5, 5);
	local v2 = createVehicle(150, -683.3698, -1326.5831, 10.8105, 290.5928, 5, 5);
	local v3 = createVehicle(151, -684.9037, -1323.8152, 10.9755, 292.5149, 5, 5);
	local v4 = createVehicle(152, -687.1147, -1321.3459, 11.1368, 292.0442, 5, 5);
	local v5 = createVehicle(162, -674.5535, -1343.4515, 11.2688, 294.7513, 5, 5);
	local v6 = createVehicle(162, -677.5064, -1342.6252, 11.2689, 289.3499, 5, 5);
	local v7 = createVehicle(164, -680.6094, -1333.6694, 10.8321, 290.6213, 5, 5);
	local v8 = createVehicle(164, -679.5953, -1337.7463, 10.8321, 292.4391, 5, 5);
	createVehicle(163,-1743.0,-211.8,14.0,270.0,3,1);
	createVehicle(159,573.0,866.0,13.0,10.0,10,0);
	createVehicle(168,151.767,-1526.207,10.997,90.0,3,3);
	createVehicle(206,-1021.5,-870.0,13.0,150.0,36,36);
	createVehicle(145,-831.6,-902.0,11.1,150.0,0,0);
	createVehicle(191,-465.0,1330.0,11.7,0,0,1);
	createVehicle(198,162.857,-1508.879,11.03,90.0,60,78);
	createVehicle(150,-1191.3,-335.0,10.45,20.0,6,75);
	createVehicle(140,-1404.0,-797.7,13.5,0,74,35);
	createVehicle(191,294.639,270.122,16.729,0,0,0);
	createVehicle(217,375.8,332.9,11.5,180,36,26);
	createVehicle(227,-614.0,804.0,30.0,0,0,0);
	createVehicle(155,-1681.1,-103.6,14.6 ,180,0,0);
	createVehicle(234,-1285.7,1014.5,263.0 ,104.4,51,39);
	createVehicle(234,-1299.5,1030.7,263.0,132.6,51,9);
	createVehicle(156,497.9,502.9,11.3,180,46,1);
	createVehicle(157,488.8,525.3,11.3,90.0,46,1);
	createVehicle(175,-4.277,-1204.872,10.482,90.0,0,0);
	createVehicle(191,301.391,1327.879,21.974,176.0,13,13);
	createVehicle(205,-1004.0,330.0,11.2,168.0,70,76);
	createVehicle(191,-6.4,957.0,10.3,355.1,35,35);
	createVehicle(191,-596.1,676.9,11.2,180.0,2,2);
	createVehicle(220,-665.2,777.5,10.0,0,0,0);
	createVehicle(137,-821.7,563.3,11.5,180.0,0,0);
	createVehicle(207,445.351,642.714,11.2,90.0,36,2);
	createVehicle(164,-1069.5,-125.7,11.1,225.0,16,48);
	createVehicle(236,-6.031,-1232.052,10.482,270.0,5,4);
	createVehicle(147,17.647,-1244.059,10.482,0,3,3);
	createVehicle(145,521.5,114.4,11.0,45.0,3,3);
	createVehicle(210,242.6,-1274.5,11.0,165.0,77,77);
	createVehicle(141,365.2,-837.9,11.0,335.0,52,52);
	createVehicle(230,-515.6,1129.3,11.0,350.0,32,65);
	createVehicle(132,81.3,-871.7,10.6,57.0,35,35);
	createVehicle(158,-870.2,-353.9,11.0,357.8,4,75);
	createVehicle(153,-867.6,-572.2,11.1,277.5,1,56);
	createVehicle(221,-659.4,-1388.3,11.1,0,30,72);
	createVehicle(145,1064.3,-466.0,11.0,90.0,36,36);
	createVehicle(152,-980.0,-681.0,11.7,270.0,67,8);
	createVehicle(159,213.2,-491.9,11.5,350.0,3,3);
	createVehicle(156,355.7,-511.3,12,320.0,46,1);
	createVehicle(156,-665.2,788.5,12,0,46,1);
	createVehicle(201,-865.2,1158.3,10.7,180.0,0,0);
	createVehicle(211,-758.6,113.8,10.8,172.0,13,13);
	createVehicle(178,195.2,-1029.3,11.0,300.0,14,6);
	createVehicle(154,-654.6,1391.7,12.0,353.0,3,3);
	createVehicle(174,-466.7,872.4,11.0,85.0,32,32);
	createVehicle(131,-853.7,-110.8,11.0,330.0,67,1);
	createVehicle(174,-67.9,-1120.5,10.3,189.0,77,0);
	createVehicle(168,-947.7,-1264.1,11.9,180.0,30,65);
	createVehicle(199,-391.0,-573.0,39.5,90.0,1,4);
	createVehicle(193,497.6,-87.7,9.5,345.0,1,1);
	createVehicle(207,-877.2,755.1,11.0,225.0,0,26);
	createVehicle(157,364.4,-518.2,12.3,320.0,46,1);
	createVehicle(179,-1096.0,333.2,11.3,270.0,0,72);
	createVehicle(200,-1100.8,299.4,11.3,270.0,0,36);
	createVehicle(151,-1071.9,309.2,11.9,315.0,0,36);
	createVehicle(191,-1060.3,347.2,11.3,45.0,0,36);
	createVehicle(157,-851.3,-666.6,11.2,183.0,46,1);
	createVehicle(134,-965.0,133.5,9.2,10.0,66,1);
	createVehicle(230,1466.4,-809.0,14.4,190.0,48,65);
	createVehicle(219,-1445.3,-834.1,14.9,70.0,56,82);
	createVehicle(132,-1425.0,-822.6,14.5,125.0,4,4);
	createVehicle(208,-1434.5,-788.7,14.5,90.0,43,82);
	createVehicle(225,-772.6,1031.8,11.0,270.0,0,3);
	createVehicle(198,-677.3,-1259.0,16.3,180.0,53,53);
	createVehicle(131,-713.5,-1267.4,11.0,180.0,8,8);
	createVehicle(174,-687.3,-1303.8,11.0,20.0,91,91);
	createVehicle(156,-844.3,-679.6,10.9,273.0,46,1);
	createVehicle(159,261.6,1288.8,16.1,105.0,8,8);
	createVehicle(131,279.0,-846.6,10.0,157.0,47,76);
	createVehicle(141,262.1,-249.5,11.3,170.0,59,69);
	createVehicle(211,237.2,-227.2,11.5,250.0,4,4);
	createVehicle(132,-357.5,-533.7,11.5,0,3,3);
	createVehicle(154,409.653,632.27,11.437,160.0,4,4);
	createVehicle(210,343.8,270.3,11.2,0,36,36);
	createVehicle(187,109.0,284.2,20.7,0,1,1);
	createVehicle(175,541.913,1253.249,15.595,270.0,1,1);
	createVehicle(206,-1085.0,114.0,11.2,100.0,72,72);
	createVehicle(219,430.3,957.0,12.2,270.0,36,36);
	createVehicle(206,-722.0,367.2,11.1,315.0,61,39);

	setScriptAuthor("adamix & jenksta & chris & bpeterson");
	setScriptVersion("0.1");
}

function onServerPulse()
{

}

function onPlayerSync(playerid)
{

}

function onVehicleSync(vehicleid)
{

}

function onPlayerConnect(playerid)
{
	print("Connect: [ID: " + playerid + "] [Name: " + getPlayerName(playerid) + "] [IP: " + getPlayerIP(playerid) + "]");
	sendPlayerMessage(playerid, 0xFFFF00FF, "-> Welcome to Vice Madness");
	sendPlayerMessage(playerid, 0xFFFF00FF, "-> For help type /help");
	sendPlayerMessage(playerid, 0xFFFF00FF, "-> Thanks for playing at Vice Madness!");
	
	sendPlayerMessageToAll(0xE60000FF, "[Join]: "  + getPlayerName(playerid) + " [Serial: "+ getPlayerSerial(playerid) + "] has joined Vice Maddness!.");
	loadClientScript(playerid, "clienttest.nut");
}


function onPlayerDisconnect(playerid, reason)
{
	print("Disconnect: [ID: " + playerid + "] [Reason: " + returnQuitReason(reason) + "]");
	sendPlayerMessageToAll(0xE60000FF, "[Join]: "  + getPlayerName(playerid) + " has left Vice Maddness!. Reason: " + returnQuitReason(reason));
}


function onPlayerSpawn(playerid)
{
	print("Spawn: [ID: " + playerid + "]");
	sendPlayerMessageToAll(0xE60000FF, "[Spawn]: " + getPlayerName(playerid) + " has spawned on Vice Maddness!");
	setPlayerTime(playerid, 12, 12);
}


function onPlayerText(playerid, text)
{
	return true;
}

function onPlayerCommand(playerid, cmdtext)
{
	print("onPlayerCommand(" + playerid + ", " + cmdtext + ");");
	local cmd = split(cmdtext, " ");
	if(cmd[0] == "/ispaused")
	{
		if(isPlayerPaused(cmd[1].tointeger()))
		{
			sendPlayerMessage(playerid, 0xE60000FF, "Playerid: " + cmd[1].tointeger() + " is paused.");	
		}
		else
		{
			sendPlayerMessage(playerid, 0xE60000FF, "Playerid: " + cmd[1].tointeger() + " is NOT paused.");	
		}
	}
	if(cmd[0] == "/createtext")
	{
		local text = createText(0xFFFF00FF, "Arial BOLD", 14, cmd[1].tofloat(), cmd[2].tofloat(), "HELLO WORLD AND DIE KITTY :D");
		toggleTextForPlayer(playerid, text, true);
		
	}
	if(cmd[0] == "/destroytext")
	{
		destroyText(cmd[1].tointeger());
	}
	if(cmd[0] == "/createcp")
	{
		local pos = getPlayerPosition(playerid);
		createCheckpoint(pos[0], pos[1], pos[2], cmd[1].tofloat());
	}
	if(cmd[0] == "/destroycp")
	{
		destroyCheckpoint(cmd[1].tointeger());
	}
	if(cmd[0] == "/cpickup")
	{
		local pos = getPlayerPosition(playerid);
		createPickup(cmd[1].tointeger(), cmd[2].tointeger(), pos[0], pos[1]+3, pos[2]);
	}
	if(cmd[0] == "/hidetextall")
	{
		toggleTextForAll(cmd[1].tointeger(), false);
	}
	if(cmd[0] == "/settextall")
	{
		setTextForAll(cmd[1].tointeger(), cmd[2].tostring());
	}
	if(cmd[0] == "/settext")
	{
		setText(cmd[1].tointeger(), cmd[2].tointeger(), cmd[3].tostring());
	}
	if(cmd[0] == "/settextcolorall")
	{
		setTextColorForAll(cmd[1].tointeger(), cmd[2].tointeger());
	}
	if(cmd[0] == "/settextposall")
	{
		setTextPositionForAll(cmd[1].tointeger(), cmd[2].tofloat(), cmd[3].tofloat());
	}
	if(cmd[0] == "/settextpos")
	{
		setTextPositionForPlayer(cmd[1].tointeger(), cmd[2].tointeger(), cmd[3].tofloat(), cmd[4].tofloat());
	}
	if(cmd[0] == "/showtext")
	{
		toggleTextForPlayer(playerid, cmd[1].tointeger(), true);
		
	}
	if(cmd[0] == "/hidetext")
	{
		toggleTextForPlayer(playerid, cmd[1].tointeger(), false);
		
	}
	if(cmd[0] == "/createcar")
	{
		print("Processing command");
		local pos = getPlayerPosition(playerid);
		print("getPlayerPosition");
		local carid = cmd[1].tointeger();
		print("local carid = cmd[1].tointeger();");
		local car = createVehicle(carid, pos[0], pos[1], pos[2], 0.0, 0, 0);
		print("local car = createVehicle(carid, pos[0], pos[1], pos[2], 0.0, 0, 0);");
		putPlayerInVehicle(playerid, car);
		print("putPlayerInVehicle(playerid, car);");
		setVehicleColor(car, 5, 5);
		print("setVehicleColor(car, 5, 5);");
		
	}
	if(cmd[0] == "/destroycar")
	{
		local car = getPlayerVehicleID(playerid);
		if(car == false) return 0;
		destroyVehicle(car);
	}
		if(cmd[0] == "/setcash")
	{
		setPlayerCash(playerid,1000)
		sendPlayerMessage(playerid, 0x00FF00FF, "Cash set!");
	}
		if(cmd[0] == "/getcash")
	{
		sendPlayerMessage(playerid, 0x00FF00FF, "Cash " + getPlayerCash(playerid));
	}
	if(cmd[0] == "/bleed")
	{
		togglePlayerBleeding(playerid, cmd[1].tointeger());
	}
	if(cmd[0] == "/mycar")
	{
		sendPlayerMessage(playerid, 0x00FF00FF, "Your Vehicle: " + getPlayerVehicleID(playerid));
	}
	if(cmd[0] == "/sky")
	{
		setSkyColor(playerid, cmd[1].tointeger(), cmd[2].tointeger());
	}
	if(cmd[0] == "/classselect")
	{
		forceClassSelection(playerid);
	}
	if(cmd[0] == "/vehicleme") 
	{
		if(isPlayerInVehicle(playerid)) sendPlayerMessage(playerid, 0x00FF00FF, "true");
			else sendPlayerMessage(playerid, 0x00FF00FF, "false11");
	}
	if(cmd[0] == "/vm") 
	{
    local pos = getPlayerPosition(playerid);
    local carid = cmd[1].tointeger();
    local car = createVehicle(carid, pos[0], pos[1], pos[2], 0.0, 0, 0);
    putPlayerInVehicle(playerid, car);
	}
	if(cmd[0] == "/flashitem")
	{
		setItemFlashing(playerid, cmd[1].tointeger());
		sendPlayerMessage(playerid, 0xFFFF00FF, "Flashing item: " + cmd[1]);
	}
		if(cmd[0] == "/setgrav")
	{
		local amount = cmd[1].tofloat();
		sendPlayerMessage(playerid, 0xFFFF00FF, "gravity set to " + amount);
		setPlayerGravity(playerid,amount)
	}
		if(cmd[0] == "/getgrav")
	{
		sendPlayerMessage(playerid, 0xFFFF00FF, "Gravity: " + getPlayerGravity(playerid));
	}
	if(cmd[0] == "/cellphone")
 	{
 		toggleCellPhone(playerid, cmd[1].tointeger());
 	}
 	if(cmd[0] == "/camshake")
 
 	{
 		setCameraShakeIntensity(playerid, cmd[1].tointeger());
 	}
	if(cmd[0] == "/trunk")
	{
		popVehicleTrunk(getPlayerVehicleID(playerid));
	}
	if(cmd[0] == "/mypos")
	{
		local pos = getPlayerPosition(playerid);
		sendPlayerMessage(playerid, 0x00FF00FF, "My Pos: " + pos[0] + " " + pos[1] + " " + pos[2]);
	}
	if(cmd[0] == "/setskin")
	{
		local skin = cmd[1].tointeger();
		setPlayerSkin(playerid, skin);
		sendPlayerMessage(playerid, 0x00FF00FF, "Skin: " + getPlayerSkin(playerid));
	}
	if(cmd[0] == "/object")
	{
		local pos = getPlayerPosition(playerid);
		createObject(cmd[1].tointeger(), pos[0], pos[1]+10, pos[2], 0.0, 0.0, 0.0);
	}
}

function onPlayerRequestClass(playerid, classid)
{
	print("onPlayerRequestClass(" + playerid + ", " + classid + ")");
}


function onPlayerDeath(playerid, killerid, reason)
{
	print("onPlayerDeath(" + playerid + ", " + killerid + ", " + reason + ")");
}


function onPlayerEnterVehicle(playerid, vehicle)
{
	print("onPlayerEnterVehicle(" + playerid + ", " + vehicle + ")");
}


function onPlayerExitVehicle(playerid, vehicle)
{
	print("onPlayerExitVehicle(" + playerid + ", " + vehicle + ")");
}


function onRconCommand(command, args)
{
	print("onRconCommand(" + command + ", " + args + ")");
}

function onVehicleCreate(vehicleid)
{
	print("onVehicleCreate(" + vehicleid + ")");
}

function onVehicleDestroy(vehicleid)
{
	print("onVehicleDestroy(" + vehicleid + ")");
}

function onBan(ipmask)
{
	print("onBan(" + ipmask + ")");
}

function onKick(playerid)
{
	print("onKick(" + playerid + ")");
}

function onPlayerDamage(playerid, oldhp, newhp)
{
	print("onPlayerDamage(" + playerid + ", " + oldhp + ", " + newhp + ");");
}

function onVehicleDamage(vehicleid, oldhp, newhp)
{
	print("onVehicleDamage(" + vehicleid + ", " + oldhp + ", " + newhp + ");");
}

function onVehicleSpawn(vehicleid)
{
	print("onVehicleSpawn(" + vehicleid + ")");
}

function onVehicleDeath(vehicleid)
{
	print("onVehicleDeath(" + vehicleid + ")");
}

function onTimerCreate(timerid)
{

}

function onTimerDestroy(timerid)
{

}

function onPlayerKeyEvent(playerid, type, key)
{
	print("onPlayerKeyEvent(" + playerid + ", " + type + ", " + key + ");");
}

function onCheckpointEnter(playerid, cpid)
{
	print(format("onCheckpointEnter(%d, %d)", playerid, cpid));
	if(cpid == testCP)
	{
		toggleTextForPlayer(playerid, testText, true);
	}
}

function onCheckpointLeave(playerid, cpid)
{
	print(format("onCheckpointLeave(%d, %d)", playerid, cpid));
	if(cpid == testCP)
	{
		toggleTextForPlayer(playerid, testText, false);
	}
}

function returnQuitReason(reason)
{
	switch(reason)
	{
		case 0:
			return "Crash/Timeout";
		break;
		case 1:
			return "Quit/Left";
		break;
	}
}
