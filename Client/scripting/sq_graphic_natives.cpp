//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

extern CGame *pGame;
extern CNetworkManager *pNetowkManager;
extern CChatWindow *pChatWindow;
extern IDirect3DDevice8 *pD3DDevice;
extern D3DDISPLAYMODE D3DDisplayMode;

extern CGUI *pGUI;

using namespace RakNet;

SQInteger sq_drawText(SQVM * pVM)
{
	const char * message;
	SQInteger a,r,g,b;
	float x,y;
	sq_getfloat(pVM, -7, &x);
	sq_getfloat(pVM, -6, &y);
	sq_getstring(pVM, -5, &message);
	sq_getinteger(pVM, -4, &a);
	sq_getinteger(pVM, -3, &r);
	sq_getinteger(pVM, -2, &g);
	sq_getinteger(pVM, -1, &b);
	pChatWindow->m_pD3DFont->DrawTextA(x, y, D3DCOLOR_ARGB(a,r,g,b), message, 0, 0);
	return 1;
}

SQInteger sq_drawBox(SQVM * pVM)
{
	float x, y;
	float width, height;
	SQInteger a,r,g,b;
	sq_getfloat(pVM, -8, &x);
	sq_getfloat(pVM, -7, &y);
	sq_getfloat(pVM, -6, &width);
	sq_getfloat(pVM, -5, &height);
	sq_getinteger(pVM, -4, &a);
	sq_getinteger(pVM, -3, &r);
	sq_getinteger(pVM, -2, &g);
	sq_getinteger(pVM, -1, &b);
	pChatWindow->m_pD3DFont->DrawBox(x, y, width, height, 0, D3DCOLOR_ARGB(a, r, g, b));
	return 1;
}

SQInteger sq_getResolution(SQVM * pVM)
{
	D3DDISPLAYMODE mode;
	pD3DDevice->GetDisplayMode(&mode);
	sq_newarray(pVM, 0);
	sq_pushinteger(pVM, mode.Width);
	sq_arrayappend(pVM, -2);
	sq_pushinteger(pVM, mode.Height);
	sq_arrayappend(pVM, -2);
	sq_push(pVM, -1);
	return 1;
}

SQInteger sq_showCursor(SQVM * pVM)
{
	SQBool show;
	sq_getbool(pVM, -1, &show);

	pGUI->SetCursorVisible(show != 0);
	pGame->SetInputsEnabled(!show != 0);

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_showMessage(SQVM * pVM)
{
	const char * message;
	const char * title;
	sq_getstring(pVM, -2, &title);
	sq_getstring(pVM, -1, &message);

	pGUI->ShowMessageBox(message, title);

	sq_pushbool(pVM, true);
	return 1;
}