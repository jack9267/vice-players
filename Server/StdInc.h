// Standard
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string>
#include <time.h>

#ifndef WIN32
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#endif

// RakNet
#include "../RakNet/RakPeer.h"
#include "../RakNet/RPC4Plugin.h"
#include "../RakNet/BitStream.h"
#include "../RakNet/RakPeerInterface.h"
#include "../RakNet/WindowsIncludes.h"

// TinyXML
#include "../Shared/TinyXML/tinyxml.h"
#include "../Shared/TinyXML/ticpp.h"

// VCP
#include "main.h"
#include "system.h"
#include "CConfig.h"
#include "CNetworkManager.h"
#include "CRcon.h"
#include "CScripts.h"
#include "CPlugins.h"
#include "CMasterList.h"
#include "sq_utils.h"

// Squirrel
#include "../vendor/squirrel/sqstdaux.h"
#include "../vendor/squirrel/sqstdblob.h"
#include "../vendor/squirrel/sqstdio.h"
#include "../vendor/squirrel/sqstdmath.h"
#include "../vendor/squirrel/sqstdstring.h"
#include "../vendor/squirrel/sqstdsystem.h"
#include "../vendor/squirrel/sqstring.h"

// Shared
#include "../Shared/CXML.h"