//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

PED_TYPE * VCMP_SAFECALL CPools::GetPedFromIndex(int iIndex)
{
	PED_TYPE * pPed;

	DWORD dwFunc = FUNC_CPool_CPed_AtHandle;
	_asm
	{
		mov ebx, VAR_PedPool
		mov ecx, [ebx]
		push iIndex
		call dwFunc
		mov pPed, eax
	}

	return pPed;	
}

int VCMP_SAFECALL CPools::GetIndexFromPed(PED_TYPE * pPed)
{
	int iIndex;

	DWORD dwFunc = FUNC_CPool_CPed__HandleOf;
	_asm
	{
		mov ebx, VAR_PedPool
		mov ecx, [ebx]
		push pPed
		call dwFunc
		mov iIndex, eax
	}

	return iIndex;
}

VEHICLE_TYPE * VCMP_SAFECALL CPools::GetVehicleFromIndex(int iIndex)
{
	VEHICLE_TYPE * pVehicle;

	DWORD dwFunc = FUNC_CPool_CVehicle_AtHandle;
	_asm
	{
		mov ebx, VAR_VehicleManager
		mov ecx, [ebx]
		push iIndex
		call dwFunc
		mov pVehicle, eax
	}

	return pVehicle;
}

int VCMP_SAFECALL CPools::GetIndexFromVehicle(VEHICLE_TYPE * pVehicle)
{
	int iIndex;

	DWORD dwFunc = FUNC_CPool_CVehicle__HandleOf;
	_asm
	{
		mov ebx, VAR_VehicleManager
		mov ecx, [ebx]
		push pVehicle
		call dwFunc
		mov iIndex, eax
	}

	return iIndex;	
}

ENTITY_TYPE * VCMP_SAFECALL CPools::GetObjectFromIndex(int iIndex)
{	
	ENTITY_TYPE *pObject;

	DWORD dwFunc = FUNC_CPool_CObject__AtHandle;

	_asm
	{
		mov ebx, VAR_ObjectManager
		mov ecx, [ebx]
		push iIndex
		call dwFunc
		mov pObject, eax
	}

	return pObject;
}

struct PLAYER_TYPE
{
	PED_TYPE * pPed;     // 000-004
	_pad(__pad0, 0x16C); // 004-170
};

PED_TYPE * VCMP_SAFECALL CPools::GetPlayerPedFromIndex(int iIndex)
{
	PLAYER_TYPE * pPlayer = (PLAYER_TYPE *)(VAR_Players + (iIndex * sizeof(PLAYER_TYPE)));
	
	if(pPlayer)
	{
		return pPlayer->pPed;
	}

	return NULL;
}
