//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 spookie
//                 Kryptos
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"


CGame					*pGame=0;
GAME_SETTINGS			tSettings;
CChatWindow				*pChatWindow=0;
CCmdWindow				*pCmdWindow=0;
CNetworkManager				*pNetowkManager=0;

BOOL					bGameInited=FALSE;
BOOL					bWindowedMode=FALSE;
BOOL					bShowNameTags=TRUE;
BOOL					bAntiCheat=FALSE;
bool					D3DInited=FALSE;

IDirect3DDevice8		*pD3DDevice;
IDirectInput8			*pDirectInput;
CGUI					*pGUI = NULL;

HANDLE					g_hInstance;
CScoreBoard				*pScoreBoard;
CNameTags				*pNameTags;
CNetStats				*pNetStats;
CScripts				*pScripts = NULL;

extern BOOL             bScriptInited;

// forwards

void SetupCommands();
void GameDebugDrawDebugScreens();
LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
int DetermineGTAVersion();

#define UNKNOWN_VERSION	0
#define VICE_10			1
#define VICE_11			2

bool ProcessServerQuery(unsigned int,struct RakNet::SystemAddress,char *,int)
{
	return false;
}

// backwards
//----------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hinstDLL;
		
		// Initialize the settings from the command line
		InitSettings();

		// Check the GTA version
		if(DetermineGTAVersion() != VICE_10)
		{
			MessageBox(0,"Incorrect gta-vc.exe version detected.\nYou must use GTA:VC 1.0 to play Vice City: Players","Vice City: Players Error",MB_OK);
			SetForegroundWindow(HWND_DESKTOP);
			ExitProcess(1);
		}

		// Ensure we have the font files
		FILE * fCheckPNG = fopen("vcpfnt.png", "rb");
		FILE * fCheckDAT = fopen("vcpfnt.dat", "rb");

		if(!fCheckPNG || !fCheckDAT)
		{
			MessageBox(0,"You are missing the vcpfnt.png/dat file from your games directory.","Vice City: Players Error",MB_OK);
			SetForegroundWindow(HWND_DESKTOP);
			ExitProcess(1);			
		}
		
		fclose(fCheckPNG);
		fclose(fCheckDAT);

		// Create the game instance
		pGame = new CGame();

		// Create the scripts instance (move to net game)
		pScripts = new CScripts();

		// Install the d3d8 hook
		InstallD3D8Hook();
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		// Uninstall the d3d8 hook
		UninstallD3D8Hook();

		if(pNetowkManager)
		{
			pScripts->onExit();
			pNetowkManager->Shutdown();
		}
	}

	return TRUE;
}

//----------------------------------------------------

void InitD3DStuff()
{
	pGUI = new CGUI(pD3DDevice);
	pGUI->Initialize();


	CGUIStaticText * pVersion = pGUI->CreateGUIStaticText();
	pVersion->setText("Vice City: Players - Development Version (" __TIME__ ", " __DATE__ ")");
	CEGUI::Font * pFont = pGUI->GetTahomaBoldFont();
	float fTextWidth = pFont->getTextExtent("Vice City: Players - Development Version (" __TIME__ ", " __DATE__ ")");
	float fTextHeight = pFont->getFontHeight();
	pVersion->setSize(CEGUI::UVector2(CEGUI::UDim(0, fTextWidth), CEGUI::UDim(0, fTextHeight)));
	float fTextX = pFont->getTextExtent("_");
	float fTextY = -(fTextX + fTextHeight);
	pVersion->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fTextX), CEGUI::UDim(1, fTextY)));
	pVersion->setProperty("BackgroundEnabled", "false");
	pVersion->setProperty("FrameEnabled", "false");
	pVersion->setProperty("Font", "Tahoma-Bold");
	pVersion->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	pVersion->setAlpha(155);
	pVersion->setVisible(true);

	// Create instances of the chat and input classes.
	pChatWindow = new CChatWindow(pD3DDevice);
	pCmdWindow = new CCmdWindow(pD3DDevice);

	pScoreBoard = new CScoreBoard();
	pNameTags = new CNameTags(pD3DDevice);
	pNetStats = new CNetStats();

	SetupCommands();

	D3DInited = true;
}

//----------------------------------------------------

void TheSceneEnd()
{
	if(bScriptInited)
	{
		// If the game is not inited then do it now.
		if(!bGameInited && FileCheckSum())
		{
			if(!tSettings.bDebug)
			{
				pNetowkManager = new CNetworkManager(tSettings.szConnectHost,atoi(tSettings.szConnectPort),
					tSettings.szNickName,tSettings.szConnectPass);
			}

			pGame->ToggleFrameLimiterState(TRUE);
			bGameInited = TRUE;
			return;
		}

		// Process the netgame if it's active.
		if(pNetowkManager) pNetowkManager->Process();

		if(!pGame->IsMenuActive() && bShowNameTags) {
			pNameTags->Draw();
		}

		if(!pGame->IsMenuActive()) {
			if((pNetowkManager) && GetAsyncKeyState(VK_F5)) {
				pGame->DisplayHud(FALSE);
				pScoreBoard->Draw();
			}
			else if((pNetowkManager) && GetAsyncKeyState(VK_F6)) {
				pGame->DisplayHud(FALSE);
				pNetStats->Draw();
			} 
			else
			{
				pGame->DisplayHud(TRUE);
				if(pChatWindow) pChatWindow->Draw();
				if(pCmdWindow) pCmdWindow->Draw();
				pNetowkManager->GetTextManager()->Process();
			}
			if(pScripts)
				pScripts->onRender();
		}
	}
}

//----------------------------------------------------

void InitSettings()
{
	// Get the command line
	char * szCmdLine = GetCommandLine();

	// Reset the settings struct
	memset(&tSettings, 0, sizeof(GAME_SETTINGS));

	while(*szCmdLine)
	{
		// Is the current char a '-' or a '/'?
		if(*szCmdLine == '-' || *szCmdLine == '/')
		{
			// Skip the '-' or '/'
			szCmdLine++;

			// Check the current char for a valid option char
			switch(*szCmdLine)
			{
			case 'd':
			case 'D':
				tSettings.bDebug = TRUE;
				break;
			case 'w':
			case 'W':
				bWindowedMode = TRUE;
				break;
			case 'h':
			case 'H':
				szCmdLine++;
				SetStringFromCommandLine(szCmdLine,tSettings.szConnectHost);
				break;
			case 'p':
			case 'P':
				szCmdLine++;
				SetStringFromCommandLine(szCmdLine,tSettings.szConnectPort);
				break;
			case 'n':
			case 'N':
				szCmdLine++;
				SetStringFromCommandLine(szCmdLine,tSettings.szNickName);
				break;
			case 'z':
			case 'Z':
				szCmdLine++;
				SetStringFromCommandLine(szCmdLine,tSettings.szConnectPass);
				break;
			}
		}

		// Increment the current char
		szCmdLine++;
	}
}

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString)
{
	// Skip any extra whitespace
	while(*szCmdLine == ' ')
	{
		szCmdLine++;
	}

	// Loop through until we find whitespace or a '-' or a '/'
	while(*szCmdLine && *szCmdLine != ' ' && *szCmdLine != '-' && *szCmdLine != '/')
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}

	// Null terminate the string
	*szString = '\0';
}

//----------------------------------------------------

int DetermineGTAVersion()
{
	BYTE * byteVerCheck = (BYTE *)0x608578;
	
	switch(*byteVerCheck)
	{
	case 0x81: 
		return VICE_11;
	case 0x5D: 
		return VICE_10;
	}
	
	return UNKNOWN_VERSION;
}

//----------------------------------------------------

void logprintf(char * format, ...)
{
	char tmp_buf[512];
	va_list args;
	va_start(args, format);
	vsprintf(tmp_buf, format, args);
	va_end(args);
	puts(tmp_buf);
	FILE* log = fopen("vc-p/client.log", "a");
	fprintf(log, "%s\n", tmp_buf);
	fclose(log);
}

//----------------------------------------------------