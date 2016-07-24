//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "common.h"
#include "address.h"

#define VAR_PedPool								0x97F2AC
#define VAR_VehicleManager							0xA0FDE4
#define VAR_ObjectManager							0x94DBE0
#define FUNC_CPool_CPed__HandleOf				0x451CF0
#define FUNC_CPool_CPed_AtHandle				0x451CB0
#define FUNC_CPool_CVehicle_AtHandle			0x451C70
#define FUNC_CPool_CVehicle__HandleOf			0x42C4B0
#define FUNC_CPool_CObject__AtHandle			0x451C30
#define VAR_Players								0x94AD28

#define VCMP_SAFECALL __stdcall

class CPools
{
public:
	static PED_TYPE *     VCMP_SAFECALL GetPedFromIndex(int iIndex);
	static int            VCMP_SAFECALL GetIndexFromPed(PED_TYPE * pPed);
	static VEHICLE_TYPE * VCMP_SAFECALL GetVehicleFromIndex(int iIndex);
	static int            VCMP_SAFECALL GetIndexFromVehicle(VEHICLE_TYPE * pVehicle);
	static ENTITY_TYPE  * VCMP_SAFECALL GetObjectFromIndex(int iIndex);
	static PED_TYPE *     VCMP_SAFECALL GetPlayerPedFromIndex(int iIndex);
};
