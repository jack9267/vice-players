//----------------------------------------------------
//
// GPL code license:
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
//
//-----------------------------------------------------

#include "sq_functions.h"
#include <stdio.h>

SQInteger sq_helloworld(HSQUIRRELVM pVM)
{
	printf("Hello world from example module!\n");
	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_sockOpen(HSQUIRRELVM pVM)
{
    if ( pVM )
    {
            // Put the host in a string, and the port in an unsigned short
            const char * szstrHost; 
			sq_getstring(pVM, -2, &szstrHost);
			SQInteger usPort;
			sq_getinteger ( pVM, -1, &usPort );

			string strHost = szstrHost;

            // Create the socket
            CSocket* pSocket = new CSocket(pVM, strHost, usPort);
            int ID  = pSocket->GetID();

            // The socket has got a userdata value if successfully created. It doesn't otherwise
            if (ID == -1)
            {
                if(pSocket) delete(pSocket);
                sq_pushbool(pVM, false);
                return 1;
            }

            // Add the socket to the Pulse list
            CSocketManager::SocketAdd(pSocket);

            // Return the userdata
			sq_pushinteger(pVM, ID);
            return 1;
    }

	sq_pushbool(pVM, false);
    return 1;
}

SQInteger sq_sockWrite(HSQUIRRELVM pVM)
{
    if ( pVM )
    {
            // Prepare variables
			SQInteger ID;
			sq_getinteger(pVM, -2, &ID);

            CSocket* pSocket = NULL;

            // Get the socket from the userdata. Gets stored in pSocket. If failed, returns false
            if (CSocketManager::GetSocket(pSocket, ID))
            {
                // Try to send the data
                string strData;
				const char * szStrData;
				sq_getstring(pVM, -1, &szStrData);
				strData = szStrData;
                bool bSuccess  = pSocket->Send(strData);

                // Return whether or not it was successful
                sq_pushbool(pVM, bSuccess);
                return 1;
            }
    }

    sq_pushbool(pVM, false);
    return 1;
}

SQInteger sq_sockClose(HSQUIRRELVM pVM)
{
    if ( pVM )
    {
            // Prepare vars
            SQInteger ID;
			sq_getinteger(pVM, -1, &ID);
            CSocket* pSocket = NULL;

            // Get the socket
            if (CSocketManager::GetSocket(pSocket, ID))
            {
                // Remove it
                CSocketManager::SocketRemove(pSocket);

                sq_pushbool(pVM, true);
                return 1;
            }
    }

    sq_pushbool(pVM, false);
    return 1;
}
