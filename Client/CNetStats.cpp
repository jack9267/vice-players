//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman (in a rush)
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"
#include "../raknet/GetTime.h"

using namespace RakNet;

extern CNetworkManager* pNetowkManager;
extern GAME_SETTINGS tSettings;
extern IDirect3DDevice8 *pD3DDevice;
extern CChatWindow *pChatWindow;

CNetStats::CNetStats()
{
	m_dwLastUpdateTick = GetTickCount();
	m_dwLastTotalBytesRecv = 0;
	m_dwLastTotalBytesSent = 0;
	m_dwBPSDownload = 0;
	m_dwBPSUpload = 0;
}

void CNetStats::Draw()
{
	char szDispBuf[1024];
	D3DDISPLAYMODE dDisplayMode;
	RakNetStatistics *pRakStats = pNetowkManager->GetRakPeer()->GetStatistics(UNASSIGNED_SYSTEM_ADDRESS);

	unsigned int uBytesPerSecondReceived = (unsigned int)pRakStats->valueOverLastSecond[ACTUAL_BYTES_RECEIVED];
	unsigned int uBytesPerSecondSent = (unsigned int)pRakStats->valueOverLastSecond[ACTUAL_BYTES_SENT];
	unsigned int uTotalBytesReceived= (unsigned int)pRakStats->runningTotal[ACTUAL_BYTES_RECEIVED];
	unsigned int uTotalBytesSent = (unsigned int)pRakStats->runningTotal[ACTUAL_BYTES_SENT];
	unsigned int uConnectionTime = (unsigned int)((GetTimeUS() - pRakStats->connectionStartTime)/1000000);
	float fCurrentPacketLoss = pRakStats->packetlossLastSecond;
	float fAveragePacketLoss = pRakStats->packetlossTotal;

	sprintf(szDispBuf,"--- Network Stats ---\n" \
					  "\n" \
					  "Bytes Per Second Received: %d\n" \
					  "Bytes Per Second Sent: %d\n" \
					  "Total Bytes Received: %d\n" \
					  "Total Bytes Sent: %d\n" \
					  "Current Packet Loss: %.02f\n" \
					  "Average Packet Loss: %.02f\n" \
					  "Connection Time: %ds\n" \
					  "\n", 
					  uBytesPerSecondReceived, uBytesPerSecondSent, 
					  uTotalBytesReceived, uTotalBytesSent, 
					  fCurrentPacketLoss, fAveragePacketLoss, 
					  uConnectionTime);
	
//	StatisticsToString(pRakStats,szStatBuf,1);

	pD3DDevice->GetDisplayMode(&dDisplayMode);
	pChatWindow->m_pD3DFont->DrawTextBox((float)((dDisplayMode.Width / 2) - 100),50.0f,50.0f,0x50000000,0xFFFFFFFF,szDispBuf,0L);
}
