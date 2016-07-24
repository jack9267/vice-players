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

#include <stdio.h>
#include "main.h"
#include "sq_functions.h"

SQVM * pVMS[128];
int	g_iSockets = 0;

void RegisterFunction(HSQUIRRELVM pVM, const char * szName, SQFUNCTION func)
{
	sq_pushroottable(pVM);
	sq_pushstring(pVM, szName, -1);
	sq_newclosure(pVM, func, 0);
	sq_createslot(pVM, -3);
	sq_pop(pVM, 1);
}

EXPORT void OnPluginLoad(char * szName)
{
	for(int i = 0; i < 128; i++)
	{
		pVMS[i] = NULL;
	}
	printf("Hello world from %s plugin!\n", szName);
}

EXPORT void OnPluginUnload()
{
	printf("Example plugin got unloaded.\n");
}

EXPORT void OnScriptLoad(SQVM * pVM)
{
	RegisterFunction(pVM, "HelloWorld", sq_helloworld);
	RegisterFunction(pVM, "sockOpen", sq_sockOpen);
	RegisterFunction(pVM, "sockWrite", sq_sockWrite);
	RegisterFunction(pVM, "sockClose", sq_sockClose);

	for(int i = 0; i < 128; i++)
	{
		if(pVMS[i] != NULL)
		{
			continue;
		}
		else
		{
			printf("[Sockets] Registered script %d\n", i);
			pVMS[i] = pVM;
			break;
		}
	}
}

EXPORT void OnScriptUnload(SQVM * pVM)
{

}

EXPORT void OnPulse()
{
	CSocketManager::DoPulse();
}