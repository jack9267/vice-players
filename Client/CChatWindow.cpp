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

extern CGame *pGame;
extern IDirect3DDevice8 *pD3DDevice;

//----------------------------------------------------

CChatWindow::CChatWindow(IDirect3DDevice8 *pD3DDevice)
{
	int x=0;

	m_bEnabled			= TRUE;
	m_fDrawStartX		= 25.0f;
	m_fDrawStartY		= 30.0f;
	m_fDrawPadding		= 3.0f;
	m_pD3DFont			= NULL;

	if(pD3DDevice) {
		m_pD3DFont = new CD3DFont(CHAT_FONT,CHAT_FONT_SIZE+1,0);
		m_pD3DFont->InitDeviceObjects(pD3DDevice);
		m_pD3DFont->RestoreDeviceObjects();
	}

  	m_pD3DFont->GetTextExtent("W",&m_sizeText);

	// Init the chat window lines to 0
	while(x!=MAX_DISP_MESSAGES) {
		memset(m_szChatWindowBuffer[x],0,MAX_MESSAGE_LENGTH);
		x++;
	}

	m_dwChatTextColor = 0xFFFFFFFFFFFFFFFF;
	m_dwChatInfoColor = 0xFFFFFFFFFF88AA62;
	m_dwChatDebugColor = 0xFFFFFFFFFFA9C4E4;
	m_dwChatBackgroundColor = -16777216;

	m_pD3DDevice = pD3DDevice;
}

//----------------------------------------------------

CChatWindow::~CChatWindow()
{
	if(m_pD3DFont) delete m_pD3DFont;
}

//----------------------------------------------------

void CChatWindow::Draw()
{
	D3DDISPLAYMODE DisplayMode;
	float fDrawX=m_fDrawStartX;
	float fDrawY=m_fDrawStartY; 
	DWORD dwColorChat=0;

	int x=0;

	if(m_pD3DFont && m_bEnabled)
	{
		while(x!=(MAX_DISP_MESSAGES)) {
			dwColorChat = m_dwChatColor[x];

			// Draws a drop shadow on the text. Too CPU intense?
			m_pD3DFont->DrawText(fDrawX+1.0f,fDrawY+1.0f,m_dwChatBackgroundColor,
				m_szChatWindowBuffer[x],0L,TRUE);

			m_pD3DFont->DrawText(fDrawX,fDrawY,dwColorChat,
				m_szChatWindowBuffer[x]);

			pD3DDevice->GetDisplayMode(&DisplayMode);
			//m_pD3DFont->DrawText(m_fDrawStartX,((float)(DisplayMode.Height)-20),D3DCOLOR_ARGB(255,255,255,255),
			//	"Vice City: Players - Development Version (" __TIME__ ", " __DATE__ ")");

			fDrawY+=m_sizeText.cy+m_fDrawPadding;
		
			x++;
		}
	} else {
		if((NULL == m_pD3DFont) && m_pD3DDevice) { // Create/recreate it.
			m_pD3DFont = new CD3DFont(CHAT_FONT,CHAT_FONT_SIZE,0);
			m_pD3DFont->InitDeviceObjects(m_pD3DDevice);
			m_pD3DFont->RestoreDeviceObjects();
		}
	}
}

//----------------------------------------------------

void CChatWindow::AddClientMessage(DWORD dwColor, CHAR * szMessage)
{
	FilterInvalidChars(szMessage);
	AddToChatWindowBuffer(szMessage,dwColor); // (dwColor >> 8) | 0xFF000000
}

//----------------------------------------------------

void CChatWindow::AddChatMessage(CHAR *szNick, DWORD dwNickColor, CHAR *szMessage)
{
	char tmp_buf[512] = {0};
	char * t = tmp_buf;

	memset(tmp_buf,0,512);

	// NICKNAME
	*t = '\01'; t++; // color start
	memcpy(t,&dwNickColor,sizeof(DWORD)); t+=4; // nick color
	memcpy(t,szNick,strlen(szNick)); t+=strlen(szNick); // nick string
	*t = '\01'; t++; // color start
	memcpy(t,&m_dwChatTextColor,sizeof(DWORD)); t+=4; // chat color
	*t = ':'; t++;
	*t = ' '; t++;

	// MESSAGE
	FilterInvalidChars(szMessage);

	strcpy(t,szMessage);

	AddToChatWindowBuffer(tmp_buf, m_dwChatTextColor);
}

//----------------------------------------------------

void CChatWindow::AddInfoMessage(CHAR * szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf,0,512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(tmp_buf, m_dwChatInfoColor);	
}

//----------------------------------------------------

void CChatWindow::AddDebugMessage(CHAR * szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf,0,512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(tmp_buf, m_dwChatDebugColor);	
}

//----------------------------------------------------

void CChatWindow::FilterInvalidChars(PCHAR szString)
{
	while(*szString) {
		if(*szString < ' ' || *szString > 'z') {
			*szString = ' ';
		}
		szString++;
	}
}

//----------------------------------------------------

void CChatWindow::AddToChatWindowBuffer(PCHAR szString, DWORD dwColor)
{
	int iBestLineLength;
	
	PushBack();

	if(strlen(szString) > MAX_LINE_LENGTH)
	{
		iBestLineLength = MAX_LINE_LENGTH;
		// see if we can locate a space.
		while(szString[iBestLineLength] != ' ' && iBestLineLength)
			iBestLineLength--;

		if((MAX_LINE_LENGTH - iBestLineLength) > 8) {
			// we should just take the whole line
			strncpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString,MAX_LINE_LENGTH);
			m_szChatWindowBuffer[MAX_DISP_MESSAGES-1][MAX_LINE_LENGTH] = '\0';
			m_dwChatColor[MAX_DISP_MESSAGES-1] = dwColor;
			PushBack();
			strcpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString+MAX_LINE_LENGTH);
			m_dwChatColor[MAX_DISP_MESSAGES-1] = dwColor;
		}
		else {
			// grab upto the found space.
			strncpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString,iBestLineLength);
			m_szChatWindowBuffer[MAX_DISP_MESSAGES-1][iBestLineLength] = '\0';
			m_dwChatColor[MAX_DISP_MESSAGES-1] = dwColor;
			PushBack();
			strcpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString+(iBestLineLength+1));
			m_dwChatColor[MAX_DISP_MESSAGES-1] = dwColor;
		}
	}
	else {
		strcpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString);
		m_dwChatColor[MAX_DISP_MESSAGES-1] = dwColor;
	}
	
}

//----------------------------------------------------

void CChatWindow::PushBack()
{
	for(int i = 0; i < (MAX_DISP_MESSAGES - 1); i++) {
		strcpy(m_szChatWindowBuffer[i],m_szChatWindowBuffer[i + 1]);
		m_dwChatColor[i] = m_dwChatColor[i + 1];
	}
}

//----------------------------------------------------

void CChatWindow::DeleteDeviceObjects()
{
	SAFE_DELETE(m_pD3DFont);
}

//----------------------------------------------------
// EOF