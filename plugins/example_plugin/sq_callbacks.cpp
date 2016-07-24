//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: adamix
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "sq_callbacks.h"
#include "sq_utils.h"
extern SQVM * g_VMs[];
void OnHelloWorld(const char * teststring)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(g_VMs[i]) {
			// get the script vm pointer
			SQVM * pVM = g_VMs[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onHelloWorld", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				sq_pushstring(pVM, teststring, -1);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}