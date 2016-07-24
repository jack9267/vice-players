//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
// Portions based on public domain code by Michael Fötsch (screenshot)
//
//----------------------------------------------------

#include "StdInc.h"
#include <string>
#include <windowsx.h>

extern CGame			*pGame;
extern CChatWindow		*pChatWindow;
extern CCmdWindow		*pCmdWindow;
extern IDirect3DDevice8 *pD3DDevice;
extern BOOL				bShowNameTags;
extern CNetworkManager			*pNetowkManager;
extern CGUI				*pGUI;

WNDPROC hOldProc;
LRESULT APIENTRY NewWndProc(HWND,UINT,WPARAM,LPARAM);
void GetScreenshotFileName(std::string& FileName);

//----------------------------------------------------


void SendKeyEvent(DWORD key, bool state) // state: true == down, false == up.
{
	if(!pNetowkManager) return;
	if(!pNetowkManager->IsConnected()) return;
	if(key < 32 || key > 128) return;
	if(pCmdWindow->IsEnabled()) return;

	BitStream bsSend;
	bsSend.Write(key);

	if(state)
	{
		bsSend.Write1();
	}
	else
	{
		bsSend.Write0();
	}

	pNetowkManager->GetRPC4()->Call("KeyEvent",&bsSend,LOW_PRIORITY,UNRELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------

BOOL HandleKeyPress(DWORD vKey) 
{
	switch(vKey) {

		case VK_F4:
			{
				if(bShowNameTags)
				{
					bShowNameTags = FALSE;
					break;
				}
				else
				{
					bShowNameTags = TRUE;
					break;
				}
			}
		/*case VK_F6:
			pCmdWindow->ToggleEnabled();
			break;*/

		case VK_F7:
			pChatWindow->ToggleEnabled();
			break;

		case VK_F8:
			{
				CScreenshot ScreenShot(pD3DDevice);
				std::string sFileName;
				GetScreenshotFileName(sFileName);
				if(ScreenShot.TakeScreenShot((PCHAR)sFileName.c_str())) {
					pChatWindow->AddInfoMessage("Screenshot Taken - %s",sFileName.c_str());
				} else {
					pChatWindow->AddInfoMessage("Unable to take a screenshot");
				}
			}
			break;
		case VK_RETURN:
			pCmdWindow->ProcessInput();
			break;
	}

	return FALSE;
}

//----------------------------------------------------

BOOL HandleCharacterInput(DWORD dwChar)
{
	if(pCmdWindow->IsEnabled()) {
		if(dwChar == 8) { // backspace
			pCmdWindow->BackSpace();
			return TRUE;
		}
		else if(dwChar == VK_ESCAPE) {
			pCmdWindow->Disable();
			return TRUE;
		}
		pCmdWindow->AddChar((char)dwChar);
		return TRUE;
	}
	else {
		switch(dwChar) {
			case '`':
			case 't':
			case 'T':
				pCmdWindow->Enable();
				return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------

BOOL SubclassGameWindow(HWND hWnd)
{	
	if(hWnd) {
		hOldProc = SubclassWindow(hWnd, NewWndProc);
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------

LRESULT APIENTRY NewWndProc( HWND hwnd,UINT uMsg,
							 WPARAM wParam,LPARAM lParam ) 
{ 
	if(pGUI) pGUI->MsgProc(hwnd, uMsg, wParam, lParam);
	switch(uMsg) {
		case WM_KEYUP:
			{
				SendKeyEvent((DWORD)wParam, false);
				if(HandleKeyPress((DWORD)wParam)) { // 'I' handled it.
					return 0;
				}
			}
			break;
		case WM_KEYDOWN:
			{
				SendKeyEvent((DWORD)wParam, true);
			}
			break;
		case WM_CHAR:
			if(HandleCharacterInput((DWORD)wParam)) { // 'I' handled it.
				return 0;
			}
			break;
	}
	return CallWindowProc(hOldProc, hwnd, uMsg, wParam, lParam);
}

//----------------------------------------------------

void GetScreenshotFileName(std::string & FileName)
{
	std::string ModuleFileName;
    ModuleFileName.reserve(MAX_PATH);
    GetModuleFileName(NULL,(char *)(ModuleFileName.data()),MAX_PATH);
    FileName = ModuleFileName.substr(0, ModuleFileName.find_last_of(":\\"));

    char Buf[MAX_PATH];
    WIN32_FIND_DATA ffd;
    HANDLE h;
    for (int i = 0; i < 1000; i++)
    {
        wsprintf(Buf, (FileName + "vc-mp-%03i.jpg").c_str(), i);
        h = FindFirstFile(Buf, &ffd);
        if(h != INVALID_HANDLE_VALUE) {   
			FindClose(h);
		}
        else {   
			break;
		}
    }
    FileName = Buf;
}

//----------------------------------------------------
// EOF