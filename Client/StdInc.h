// Standard
#include <stdio.h>
#include <string>
#include "../raknet/WindowsIncludes.h"
#include "main.h"
#include <process.h>

// DirectX stuff

#include "d3d8/include/d3d8.h"
#include "d3d8/include/d3dfont.h"
#include "d3d8/include/d3dx8tex.h"
#include "d3d8/include/DXUtil.h"

// Game stuff

#include "game/common.h"
#include "game/CGame.h"
#include "game/CObject.h"
#include "game/CPickup.h"
#include "game/AimSync.h"
#include "game/util.h"
#include "game/CPools.h"
#include "game/KeySync.h"
#include "game/CPhysical.h"
#include "game/CWorld.h"
#include "game/scripting.h"

// DirectX / DirectInput hooks
#include "d3dhook/IDirect3D8Hook.h"
#include "d3dhook/IDirect3DDevice8Hook.h"
#include "d3dhook/IDirectInput8Hook.h"
#include "d3dhook/IDirectInputDevice8Hook.h"
#include "d3dhook/d3d8hook.h"

// RakNet
#include "../raknet/RakPeer.h"
#include "../raknet/RPC4Plugin.h"
#include "../raknet/BitStream.h"
#include "../raknet/MessageIdentifiers.h"

// NetGame stuff
#include "net/CLocalPlayer.h"
#include "net/CRemotePlayer.h"
#include "net/CSpawnSelection.h"
#include "net/netrpc.h"
#include "net/CPlayerManager.h"
#include "net/CVehicleManager.h"
#include "net/CObjectManager.h"
#include "net/CPickupManager.h"
#include "net/CTextManager.h"
#include "net/CCheckpoints.h"
#include "net/CNetworkManager.h"

// Scripting
#include "scripting/CScripts.h"
#include "scripting/sq_utils.h"

// VCP
#include "CChatWindow.h"
#include "CCmdWindow.h"
#include "CBBFont.h"
#include "CScoreBoard.h"
#include "CNameTags.h"
#include "CNetStats.h"
#include "anticheat.h"
#include "CScreenshot.h"

// GUI
#include "GUI/CGUI.h"

// Squirrel
#include "../vendor/squirrel/sqstdaux.h"
#include "../vendor/squirrel/sqstdblob.h"
#include "../vendor/squirrel/sqstdio.h"
#include "../vendor/squirrel/sqstdmath.h"
#include "../vendor/squirrel/sqstdstring.h"
#include "../vendor/squirrel/sqstdsystem.h"

// TinyXML
#include "../Shared/TinyXML/tinyxml.h"
#include "../Shared/TinyXML/ticpp.h"

// Shared
#include "../Shared/CXML.h"