//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

#define ADDR_HWND								0x7897A4
#define ADDR_ID3D8DEVICE						0x7897A8
#define ADDR_ID3D8								0x7897B4
// TODO: CPad class
#define ADDR_KEYSTATES							0x7DBCB0
#define FUNC_CFont__SetDropShadowPosition		0x54ff20
#define FUNC_CFont__SetDropColor				0x54ff30
#define FUNC_CFont__SetAlphaFade				0x54ffc0
#define FUNC_CFont__SetRightJustifyWrap			0x54ffd0
#define FUNC_CFont__SetFontStyle				0x54ffe0
#define FUNC_CFont__SetPropOn					0x550020
#define FUNC_CFont__SetPropOff					0x550030
#define FUNC_CFont__SetRightJustifyOff			0x550040
#define FUNC_CFont__SetRightJustifyOn			0x550060
#define FUNC_CFont__SetBackgroundOnlyTextOff	0x550080
#define FUNC_CFont__SetBackgroundOnlyTextOn		0x550090
#define FUNC_CFont__SetBackgroundColor			0x5500a0
#define FUNC_CFont__SetBackgroundOff			0x5500d0
#define FUNC_CFont__SetBackgroundOn				0x5500e0
#define FUNC_CFont__SetCentreSize				0x5500f0
#define FUNC_CFont__SetWrapX					0x550100
#define FUNC_CFont__SetCentreOff				0x550110
#define FUNC_CFont__SetCentreOn					0x550120
#define FUNC_CFont__SetJustifyOff				0x550140
#define FUNC_CFont__SetJustifyOn				0x550150
#define FUNC_CFont__SetColor					0x550170
#define FUNC_CFont__SetScale					0x550230
#define FUNC_CFont__PrintString					0x551040
#define ADDR_ASCIITOUNICODE						0x552500
#define ADDR_SPAWN_OBJECT						0x4DB3F0
#define ADDR_GIVE_ACTOR_WEAPON					0x4FFA30
#define ADDR_SET_ACTOR_ARMED_WEAPON				0x4FF900
#define ADDR_GAME_RUNNING						0x86969C
#define ADDR_SET_CURRENT_WEAPON					0x4FF90D
// Used in CPlayerPed and hooks
#define FUNC_CPed__SetObjective					0x521F10
#define FUNC_CPed__InflictDamage				0x525B20
#define ADDR_ACTOR_PUT_IN_VEHICLE				0x4C6AC0
#define ADDR_VEHICLE_SET_DRIVER					0x512520
#define ADDR_REQUEST_MODEL_REQ_VAR				0x7D1DE0
#define ADDR_REQUEST_MODEL						0x40E310
#define ADDR_LOAD_REQUESTED_MODELS1				0x4D0DB0
#define ADDR_LOAD_REQUESTED_MODELS2				0x40B5F0
#define ADDR_LOAD_REQUESTED_MODELS3				0x4D0F30
#define ADDR_ACTOR_REMOVE_WEAPONS				0x4FF740
// TODO: CMenu class then CMenu in CGame
#define VAR_Menu								0x869630
#define VAR_Menu__StartGame						(VAR_Menu + 0x11)
#define VAR_Menu__DrawDistance					(VAR_Menu + 0x1C)
#define VAR_Menu__Brightness					(VAR_Menu + 0x18)
#define VAR_Menu__Widescreen					(VAR_Menu + 0x22)
#define VAR_Menu__FrameLimiter					(VAR_Menu + 0x25)
#define VAR_Menu__IsActive						(VAR_Menu + 0x38)
#define VAR_Menu__HudMode						(VAR_Menu + 0xA)
#define ADDR_CAMERA_ON_ENTITY					0x46A4F8
#define ADDR_RESTART_IF_WASTED_AT				0x443010
#define ADDR_IS_MODEL_LOADED_VAR				0x94DDD8
#define ADDR_IS_KEY_PRESSED						0x460C00
#define ADDR_GET_PLAYER_CONTROLS				0x4AB060
#define ADDR_HUD_SWITCH							0xA10B45
#define ADDR_Toggle_Radar						0xA10AB6

//-----------------------------------------------------------
