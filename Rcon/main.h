#pragma once

#ifdef WIN32
#include "../raknet/WindowsIncludes.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <tchar.h>
#include <process.h>
#else
#include "../server/system.h"
#include "../raknet/Kbhit.h"
#define Sleep(x) usleep(x * 1000)
#endif

#include "../raknet/RakPeer.h"
#include "../raknet/BitStream.h"
#include "../raknet/MessageIdentifiers.h"

#include "CRconClient.h"

extern bool bQuitApp;

void logprintf(char* format, ...);
