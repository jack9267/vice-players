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

#ifdef WIN32
  #define EXPORT extern "C" __declspec(dllexport)
#else
  #define EXPORT extern "C"
#endif

#define MAX_SCRIPTS 16

#include "../squirrel/squirrel.h"
#include <stdio.h>
#include <stdarg.h>