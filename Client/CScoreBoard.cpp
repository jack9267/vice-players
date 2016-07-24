//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

extern CNetworkManager* pNetowkManager;
extern GAME_SETTINGS tSettings;
extern IDirect3DDevice8 *pD3DDevice;
D3DDISPLAYMODE DisplayMode;
extern CChatWindow * pChatWindow;


void CScoreBoard::Draw()
{
	CPlayerManager* pPlayerManager = pNetowkManager->GetPlayerManager();
	//DWORD dwNickColor;
	//char * szNick;
	DWORD dwChatColor = 0xFFFFFFFF;
	char * szNameText = (char *)malloc(128);
	char szScoreText[32];
	char szPingText[32];
	int iCurrent = 0;

	pNetowkManager->UpdatePlayerScoresAndPings();

	pD3DDevice->GetDisplayMode(&DisplayMode);
	// Get the d3d font pointer
	CD3DFont * pFont = pChatWindow->m_pD3DFont;

	// Draw the background
	pFont->DrawBox((float)(DisplayMode.Width / 4), (float)(DisplayMode.Height / 4), (float)(DisplayMode.Width / 2), (float)(DisplayMode.Height / 2), 0.0f, 0x50000000);

	// Draw the title
	SIZE size;
	char szTitle[128];
	sprintf(szTitle, "Vice City: Players - %s:%s",  tSettings.szConnectHost,tSettings.szConnectPort);
	pFont->GetTextExtent(szTitle, &size);
	pFont->DrawText((float)((DisplayMode.Width / 2) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 40)), 0xFFFFFFFF, szTitle);

	// Draw the player title
	const char * szPlayerTitleText = "Player";
	pFont->GetTextExtent(szPlayerTitleText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + (DisplayMode.Width / 8)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 20)), 0xFFFFFFFF, szPlayerTitleText);

	// Draw the score title
	const char * szScoreTitleText = "Score";
	pFont->GetTextExtent(szScoreTitleText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 2)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 20)), 0xFFFFFFFF, szScoreTitleText);

	// Draw the ping title
	const char * szPingTitleText = "Ping";
	pFont->GetTextExtent(szPingTitleText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 3)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 20)), 0xFFFFFFFF, szPingTitleText);

	// Format the local player name
	/*dwNickColor = pPlayerManager->GetLocalPlayer()->GetTeamColorAsARGB();
	szNick = pPlayerManager->GetLocalPlayerName();
	*szNameText = '\01'; szNameText++; // color start
	memcpy(szNameText, &dwNickColor, sizeof(DWORD)); szNameText += 4; // nick color
	memcpy(szNameText, szNick, strlen(szNick)); szNameText += strlen(szNick); // nick string
	*szNameText = '\01'; szNameText++; // color start
	memcpy(szNameText, &dwChatColor, sizeof(DWORD)); szNameText += 4; // chat color
	*szNameText = '\0';*/
	sprintf(szNameText, "%s", pPlayerManager->GetLocalPlayerName());

	// Draw the local player name
	pFont->GetTextExtent(szNameText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + (DisplayMode.Width / 8)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szNameText);

	// Format the local player score
	sprintf(szScoreText, "%d", pPlayerManager->GetLocalPlayerScore());

	// Draw the local player score
	pFont->GetTextExtent(szScoreText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 2)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szScoreText);

	// Format the local player ping
	sprintf(szPingText, "%d", pPlayerManager->GetLocalPlayerPing());

	// Draw the local player ping
	pFont->GetTextExtent(szPingText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 3)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szPingText);

	iCurrent ++;

	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerManager->GetSlotState(i)) {
			// Format the player name
			/*dwNickColor = pPlayerManager->GetAt(i)->GetTeamColorAsARGB();
			szNick = pPlayerManager->GetPlayerName(i);
			*szNameText = '\01'; szNameText++; // color start
			memcpy(szNameText, &dwNickColor, sizeof(DWORD)); szNameText += 4; // nick color
			memcpy(szNameText, szNick, strlen(szNick)); szNameText += strlen(szNick); // nick string
			*szNameText = '\01'; szNameText++; // color start
			memcpy(szNameText, &dwChatColor, sizeof(DWORD)); szNameText += 4; // chat color
			*szNameText = '\0';*/
			sprintf(szNameText, "%s", pPlayerManager->GetPlayerName(i));

			// Draw the player name
			pFont->GetTextExtent(szNameText, &size);
			pFont->DrawText((float)(((DisplayMode.Width / 4) + (DisplayMode.Width / 8)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szNameText);

			// Format the player score
			sprintf(szScoreText, "%d", pPlayerManager->GetScore(i));

			// Draw the player score
			pFont->GetTextExtent(szScoreText, &size);
			pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 2)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szScoreText);

			// Format the player ping
			sprintf(szPingText, "%d", pPlayerManager->GetPing(i));

			// Draw the player ping
			pFont->GetTextExtent(szPingText, &size);
			pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 3)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szPingText);

			iCurrent++;
		}
	}

	free(szNameText);
}
