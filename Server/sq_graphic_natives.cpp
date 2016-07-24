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

#include "StdInc.h"

extern CNetworkManager *pNetowkManager;

// createText
SQInteger sq_createText(SQVM * pVM)
{
	SQInteger colourMessage, size;
	float posX, posY;
	const char * fontName;
	const char * messageValue;

	sq_getinteger(pVM, -6, &colourMessage);
	sq_getstring(pVM, -5, &fontName);
	sq_getinteger(pVM, -4, &size);
	sq_getfloat(pVM, -3, &posX);
	sq_getfloat(pVM, -2, &posY);
	sq_getstring(pVM, -1, &messageValue);

	EntityId text = pNetowkManager->GetTextManager()->New(colourMessage, fontName, size, posX, posY, messageValue);

	if(text != -1)
	{
		sq_pushinteger(pVM, text);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_destroyText(SQVM * pVM)
{
	SQInteger textId;

	sq_getinteger(pVM, -1, &textId);

	pNetowkManager->GetTextManager()->Delete(textId);

	sq_pushbool(pVM, true);
	return 1;
}

// showTextForPlayer
SQInteger sq_toggleTextForPlayer(SQVM * pVM)
{
	SQInteger playerId;
	SQInteger textId;
	SQBool show;

	sq_getbool(pVM, -1, &show);
	sq_getinteger(pVM, -2, &textId);
	sq_getinteger(pVM, -3, &playerId);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerId))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((EntityId)textId);
		bsSend.Write(show != 0);
		pNetowkManager->GetRPC4()->Call("Script_toggleTextForPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}

// showTextForPlayer
SQInteger sq_toggleTextForAll(SQVM * pVM)
{
	SQInteger textId;
	SQBool show;

	sq_getbool(pVM, -1, &show);
	sq_getinteger(pVM, -2, &textId);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->Show(show != 0);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}

SQInteger sq_setTextForAll(SQVM * pVM)
{
	SQInteger textId;
	const char * szText;

	sq_getstring(pVM, -1, &szText);
	sq_getinteger(pVM, -2, &textId);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->SetText(szText);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}

SQInteger sq_setTextForPlayer(SQVM * pVM)
{
	SQInteger playerId;
	SQInteger textId;
	const char * szText;

	sq_getstring(pVM, -1, &szText);
	sq_getinteger(pVM, -2, &textId);
	sq_getinteger(pVM, -3, &playerId);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerId))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((EntityId)textId);
		bsSend.Write(strlen(szText));
		bsSend.Write(szText, strlen(szText));
		pNetowkManager->GetRPC4()->Call("Script_SetText",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}

SQInteger sq_setTextColorForAll(SQVM * pVM)
{
	SQInteger textId;
	SQInteger color;

	sq_getinteger(pVM, -1, &color);
	sq_getinteger(pVM, -2, &textId);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->SetColor(color);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}

SQInteger sq_setTextColorForPlayer(SQVM * pVM)
{
	SQInteger playerId;
	SQInteger textId;
	SQInteger color;

	sq_getinteger(pVM, -1, &color);
	sq_getinteger(pVM, -2, &textId);
	sq_getinteger(pVM, -3, &playerId);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerId))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((EntityId)textId);
		bsSend.Write((int)color);
		pNetowkManager->GetRPC4()->Call("Script_SetTextColor",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}

SQInteger sq_setTextPositionForAll(SQVM * pVM)
{
	SQInteger textId;
	float x, y;

	sq_getfloat(pVM, -1, &y);
	sq_getfloat(pVM, -2, &x);
	sq_getinteger(pVM, -3, &textId);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->SetPosition(x, y);
	}

	sq_pushbool(pVM, false);
	return 1;		
}

SQInteger sq_setTextPositionForPlayer(SQVM * pVM)
{
	SQInteger playerId;
	SQInteger textId;
	float x, y;

	sq_getfloat(pVM, -1, &y);
	sq_getfloat(pVM, -2, &x);
	sq_getinteger(pVM, -3, &textId);
	sq_getinteger(pVM, -4, &playerId);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerId))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((EntityId)textId);
		bsSend.Write(x);
		bsSend.Write(y);
		pNetowkManager->GetRPC4()->Call("Script_SetTextPosition",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;		
}