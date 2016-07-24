//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#define VCMP_SAFECALL __stdcall

int VCMP_SAFECALL GameGetWeaponModelFromWeapon(int iWeaponID);
void VCMP_SAFECALL GameToggleCheatCodes(bool bToggle);
PED_TYPE * VCMP_SAFECALL GamePool_FindPlayerPed();
DWORD VCMP_SAFECALL TranslateColorCodeToRGBA(int iCode);
void VCMP_SAFECALL InitPlayerPedPtrRecords();
void VCMP_SAFECALL SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr);
BYTE VCMP_SAFECALL FindPlayerNumFromPedPtr(DWORD dwPedPtr);
void Unprotect(DWORD dwAddress, size_t sSize);

//-----------------------------------------------------------
