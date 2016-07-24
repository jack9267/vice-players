//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

DWORD dwPlayerPedPtrs[MAX_PLAYERS];

//-----------------------------------------------------------

void VCMP_SAFECALL GameToggleCheatCodes(bool bToggle)
{
	Unprotect(0x602BDC, 16);

	if(bToggle)
	{
		BYTE byteOriginalCode[5] = { 0xE8, 0x34, 0x91, 0xEA, 0xFF };
		*(BYTE *)0x602BDC = 0x50;
		memcpy((void *)0x602BE7, byteOriginalCode, sizeof(byteOriginalCode));
	}
	else
	{
		*(BYTE *)0x602BDC = 0x90;
		memset((void *)0x602BE7, 0x90, 5);
	}
}

//-----------------------------------------------------------
// Return the PED_TYPE * of the local player.

PED_TYPE * VCMP_SAFECALL GamePool_FindPlayerPed()
{
	return CPools::GetPlayerPedFromIndex(0);
}

//-----------------------------------------------------------
// Convert weapon ID to ide model ID

int VCMP_SAFECALL GameGetWeaponModelFromWeapon(int iWeaponID)
{
	switch(iWeaponID)
	{
	case WEAPON_BRASSKNUCKLE:
		return WEAPON_MODEL_BRASSKNUCKLE;

	case WEAPON_SCREWDRIVER:
		return WEAPON_MODEL_SCREWDRIVER;

	case WEAPON_GOLFCLUB:
		return WEAPON_MODEL_GOLFCLUB;

	case WEAPON_NITESTICK:
		return WEAPON_MODEL_NITESTICK;

	case WEAPON_KNIFECUR:
		return WEAPON_MODEL_KNIFECUR;

	case WEAPON_BASEBALL_BAT:
		return WEAPON_MODEL_BASEBALL_BAT;

	case WEAPON_HAMMER:
		return WEAPON_MODEL_HAMMER;

	case WEAPON_CLEAVER:
		return WEAPON_MODEL_CLEAVER;

	case WEAPON_MACHETE:
		return WEAPON_MODEL_MACHETE;

	case WEAPON_KATANA:
		return WEAPON_MODEL_KATANA;

	case WEAPON_CHAINSAW:
		return WEAPON_MODEL_CHAINSAW;

	case WEAPON_GRENADE:
		return WEAPON_MODEL_GRENADE;

	case WEAPON_TEARGAS:
		return WEAPON_MODEL_TEARGAS;

	case WEAPON_MOLOTOV:
		return WEAPON_MODEL_MOLOTOV;

	case WEAPON_MISSILE:
		return WEAPON_MODEL_MISSILE;

	case WEAPON_COLT45:
		return WEAPON_MODEL_COLT45;

	case WEAPON_PYTHON:
		return WEAPON_MODEL_PYTHON;

	case WEAPON_CHROMEGUN:
		return WEAPON_MODEL_CHROMEGUN;

	case WEAPON_SHOTGSPA:
		return WEAPON_MODEL_SHOTGSPA;

	case WEAPON_BUDDYSHOT:
		return WEAPON_MODEL_BUDDYSHOT;

	case WEAPON_TEC9:
		return WEAPON_MODEL_TEC9;

	case WEAPON_UZI:
		return WEAPON_MODEL_UZI;

	case WEAPON_INGRAMSL:
		return WEAPON_MODEL_INGRAMSL;

	case WEAPON_MP5LNG:
		return WEAPON_MODEL_MP5LNG;

	case WEAPON_M4:
		return WEAPON_MODEL_M4;

	case WEAPON_RUGER:
		return WEAPON_MODEL_RUGER;

	case WEAPON_SNIPER:
		return WEAPON_MODEL_SNIPER;

	case WEAPON_LASER:
		return WEAPON_MODEL_LASER;

	case WEAPON_ROCKETLA:
		return WEAPON_MODEL_ROCKETLA;

	case WEAPON_FLAME:
		return WEAPON_MODEL_FLAME;

	case WEAPON_M60:
		return WEAPON_MODEL_M60;

	case WEAPON_MINIGUN:
		return WEAPON_MODEL_MINIGUN;

	case WEAPON_BOMB:
		return WEAPON_MODEL_BOMB;

	case WEAPON_HELICANNON:
		return WEAPON_MODEL_M60;

	case WEAPON_CAMERA:
		return WEAPON_MODEL_CAMERA;
	}

	return -1;
}

//-----------------------------------------------------------

DWORD dwHudColors[] = 
{
	0x778899FF, // light slate grey
	0xFF8C13FF, // dark orange
	0xC715FFFF, // Medium violet red
	0x20B2AAFF, // sea green
	0xFFD720FF, // gold
	0xDC143CFF, // crimson
	0x6495EDFF, // cornflower blue
	0xFF1493FF, // deeppink
	0xF4A460FF, // sandy
	0xEE82EEFF, // violet
	0x8B4513FF, // chocolate
	0xF0E68CFF, // khaki
	0x148B8BFF, // dark cyan
	0x14FF7FFF, // spring green
	0x556B2FFF, // olive green
	0x191970FF // midnight blue
};

DWORD VCMP_SAFECALL TranslateColorCodeToRGBA(int iCode)
{
	if(iCode < sizeof(dwHudColors)) {
		return dwHudColors[iCode];
	}

	return 0xFFFFFFFF;
}

//-----------------------------------------------------------

void VCMP_SAFECALL InitPlayerPedPtrRecords() 
{
	memset(&dwPlayerPedPtrs[0], 0, sizeof(DWORD) * MAX_PLAYERS);
}

//-----------------------------------------------------------

void VCMP_SAFECALL SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr) 
{
	dwPlayerPedPtrs[bytePlayer] = dwPedPtr;
}

//-----------------------------------------------------------

BYTE VCMP_SAFECALL FindPlayerNumFromPedPtr(DWORD dwPedPtr)
{
	for(BYTE x = 0; x < MAX_PLAYERS; x++)
	{
		if(dwPlayerPedPtrs[x] == dwPedPtr)
		{
			return x;
		}
	}

	return 0;
}

//-----------------------------------------------------------

void Unprotect(DWORD dwAddress, size_t sSize)
{
	DWORD dwOldProt;
	VirtualProtect((LPVOID)dwAddress, sSize, PAGE_EXECUTE_READWRITE, &dwOldProt);
}

//-----------------------------------------------------------