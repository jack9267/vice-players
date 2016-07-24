//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

typedef struct _GTA_CONTROLSET
{
	DWORD dwFrontPad;
	WORD wKeys1[19];
	DWORD dwFrontPad2;
	WORD wKeys2[19];
	/*WORD wTurnLeftRightAnalog[10];
	BYTE bytePadding1[138];
	BYTE byteCrouchAnalog[5];*/
	_pad(__pad0, 0xC0);
	//0x114
} GTA_CONTROLSET;

//-----------------------------------------------------------

GTA_CONTROLSET *GameGetInternalKeys();
GTA_CONTROLSET *GameGetLocalPlayerKeys();
GTA_CONTROLSET *GameGetPlayerKeys(int iPlayer);

void GameKeyStatesInit();
void GameStoreLocalPlayerKeys();
void GameSetLocalPlayerKeys();
void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET *pGcsKeyStates);
void GameSetRemotePlayerKeys(int iPlayer);
void GameResetPlayerKeys(int iPlayer);
void GameResetLocalKeys();

//-----------------------------------------------------------

#define	KEY_INCAR_TURRETLR			0
#define	KEY_INCAR_TURRETUD			1
#define	KEY_INCAR_RADIO				2
#define	KEY_INCAR_LOOKL				3
#define	KEY_INCAR_HANDBRAKE			4
#define	KEY_INCAR_LOOKR				5
#define	KEY_INCAR_TURNL				8
#define	KEY_INCAR_TURNR				9
#define	KEY_INCAR_CAMERA			11
#define	KEY_INCAR_BACKWARD			12
#define	KEY_INCAR_EXITVEHICLE		13
#define	KEY_INCAR_FORWARD			14
#define	KEY_INCAR_FIRE				15
#define	KEY_INCAR_HORN				16
#define	KEY_ONFOOT_TURNLR			0
#define	KEY_ONFOOT_ACTION			2
#define	KEY_ONFOOT_NEXTWEAPON		3
#define	KEY_ONFOOT_TARGET			4
#define	KEY_ONFOOT_PREVWEAPON		5
#define	KEY_ONFOOT_FORWARD			6
#define	KEY_ONFOOT_BACKWARD			7
#define	KEY_ONFOOT_LEFT				8
#define	KEY_ONFOOT_RIGHT			9
#define	KEY_ONFOOT_JUMP				12
#define	KEY_ONFOOT_ENTERVEHICLE		13
#define	KEY_ONFOOT_SPRINT			14
#define	KEY_ONFOOT_FIRE				15
#define	KEY_ONFOOT_CROUCH			16
#define	KEY_ONFOOT_LOOKBEHIND		17

//-----------------------------------------------------------
