local socket = -1;
local username = "vice-players";
local realname = "VC-P Echo Bot";
local nickname = "vcpnightly";
local server = "irc.gtanet.com";
local port = 6667;
local channel = "#vice-players";

function onServerInit()
{
        print("Starting creating sockets...");
        socket = sockOpen(server, port);
}

function onSockClosed(socketid)
{
        print("onSockClosed(" + socketid + ");");
}

function onSockData(socketid, data)
{
        if(socketid != socket) return;
        if(data.len() < 100) {
                print("onSockData(" + socketid + ", " + data + ");");
        }
        local rex = regexp("^PING");
        if(rex.search(data))
        {
                sockWrite(socketid, "PONG :REPLY\r\n");
        }
}

function onSockOpened(socketid)
{
        if(socketid != socket) return;
        print("onSockOpened(" + socketid + ");");
        sockWrite(socketid, "USER " + username  + " Bot * :" + realname  + "\r\n");
        sockWrite(socketid, "NICK " + nickname  +"\r\n");
        sockWrite(socketid, "JOIN :" + channel  + "\r\n");
}

function onPlayerText(playerid, text)
{
        if(socket == -1) return;
        local name = getPlayerName(playerid);
        local send = format("[%s][%d]: %s", name, playerid, text);
        sockWrite(socket, "PRIVMSG " + channel  + " :" + send + "\r\n");
}


function onPlayerConnect(playerid)
{
        if(socket == -1) return;
        local name = getPlayerName(playerid);
        local send = format("[%s] joined to the server.", name);
        sockWrite(socket, "PRIVMSG " + channel  + " :" + send + "\r\n");
}


