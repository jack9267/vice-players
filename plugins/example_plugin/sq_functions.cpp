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

#include "sq_functions.h"
#include "sq_utils.h"
#include "sq_callbacks.h"
#include <stdio.h>

SQInteger sq_helloworld(HSQUIRRELVM pVM)
{
	printf("Hello world from example module!\n");

	OnHelloWorld("Hallo!");
	sq_pushbool(pVM, true);
	return 1;
}