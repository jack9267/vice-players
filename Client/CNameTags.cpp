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

#define HEALTH_BAR_COLOR D3DCOLOR_XRGB(255, 153, 230)
#define ARMOUR_BAR_COLOR D3DCOLOR_XRGB(125, 125, 125)

extern CNetworkManager *pNetowkManager;

//----------------------------------------------------

DWORD BarOldStateBlock = 0;
DWORD BarNewStateBlock = 0;

struct BarVertices_s
{
	float x, y, z;
	D3DCOLOR c;
};

// Bar Border Vertices
BarVertices_s BarBDRVertices[4] = 
{
	//  x     y      z     rhw   c
	{-0.3f,  -0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},  // 1 . . 4
	{-0.3f,   0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},  // 2 . . 3
	{ 0.3f,   0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},
	{ 0.3f,  -0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)}
};

// Bar Background Vertices
BarVertices_s BarBGVertices[4] = 
{
	//  x     y      z     rhw   c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)}
};

// Bar Fill Vertices
BarVertices_s BarVertices[4] = 
{
	//  x     y      z     rhw   c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)}
};

D3DMATRIX matView;
CBBFont * bbfont;

CNameTags::CNameTags(IDirect3DDevice8 *pD3DDevice)
{
	bbfont = NULL;
	m_pD3DDevice = pD3DDevice;
}

CNameTags::~CNameTags()
{
	
}

void CNameTags::Draw()
{
	if(!bbfont) 
	{ 
		bbfont = new CBBFont(m_pD3DDevice, "vcpfnt");
		bbfont->Initialise(); 
	} 

	if(!BarOldStateBlock) 
	{ 
		m_pD3DDevice->BeginStateBlock(); 
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 3); 
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_WRAP0, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15); 
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, 5); 
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, 6); 
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, 1); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 4); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); 
		m_pD3DDevice->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE); 
		m_pD3DDevice->SetStreamSource(0, NULL, 0); 
		m_pD3DDevice->EndStateBlock(&BarOldStateBlock); 
	} 

	if(!BarNewStateBlock) 
	{ 
		m_pD3DDevice->BeginStateBlock(); 
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 3); 
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_WRAP0, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15); 
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, 5); 
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, 6); 
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, 1); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 1); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); 
		m_pD3DDevice->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE); 
		m_pD3DDevice->EndStateBlock(&BarNewStateBlock); 
	} 

	m_pD3DDevice->CaptureStateBlock(BarOldStateBlock); 
	m_pD3DDevice->ApplyStateBlock(BarNewStateBlock); 

	D3DXMATRIX matTransposed; 
	D3DXMatrixTranspose(&matTransposed, (D3DXMATRIX*)&matView); 
	matTransposed._14 = matTransposed._24 = matTransposed._34 = 0.0f; 

	if(pNetowkManager) 
	{ 
		CPlayerManager* pPlayerManager = pNetowkManager->GetPlayerManager(); 
		for(int x = 0; x < MAX_PLAYERS; x++)
		{ 
			if(pPlayerManager->GetSlotState(x) == TRUE)
			{ // Player is in use
				CRemotePlayer* Player = pPlayerManager->GetAt(x); 

				if(Player->IsActive() && (Player->GetDistanceFromLocalPlayer() <= 80.0f))
				{ // Active and within reasonable distance 
					CPlayerPed* PlayerPed = Player->GetPlayerPed(); 

					if(PlayerPed->IsOnScreen()) { // They're onscreen 
						// Get their position
						Vector3 vPos;
						PlayerPed->GetPosition(&vPos);

						// Set the matrix position
						matTransposed._41 = vPos.X;
						matTransposed._42 = vPos.Y;
						matTransposed._43 = vPos.Z + 1.0f;

						// Set the world transformation
						m_pD3DDevice->SetTransform(D3DTS_WORLD, &matTransposed); 

						// Get their health and armour
						// TODO: Use the player ped health/armour?
						float Health = Player->GetReportedHealth(); 
						float Armour = Player->GetReportedArmour();

						// Cap their health and armour if needed
						if(Health > 100.0f) Health = 100.0f; 
						if(Armour > 100.0f) Armour = 100.0f; 

						// Set the health bar offsets
						BarBGVertices[0].x = BarBGVertices[1].x = (0.0058f * Health) - 0.29f;

						// Set the health bar color
						BarVertices[0].c = BarVertices[1].c = BarVertices[2].c = BarVertices[3].c 
							= HEALTH_BAR_COLOR;

						// Draw the health bar border
						m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BarBDRVertices, sizeof(BarVertices_s));

						// Draw the health bar fill
						m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BarVertices, sizeof(BarVertices_s));

						// Draw the health bar background
						m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BarBGVertices, sizeof(BarVertices_s));

						if(Armour > 0.0f) { // They have armour
							// Add an offset to the z position
							matTransposed._43 += 0.1f;

							// Set the world transformation
							m_pD3DDevice->SetTransform(D3DTS_WORLD, &matTransposed);

							// Set the armour bar offsets
							BarBGVertices[0].x = BarBGVertices[1].x = (0.0058f * Armour) - 0.29f;

							// Set the armour bar color
							BarVertices[0].c = BarVertices[1].c = BarVertices[2].c = 
								BarVertices[3].c = ARMOUR_BAR_COLOR;

							// Draw the armour bar border
							m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BarBDRVertices, sizeof(BarVertices_s));

							// Draw the armour bar fill
							m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BarVertices, sizeof(BarVertices_s));

							// Draw the armour bar background
							m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, BarBGVertices, sizeof(BarVertices_s));
						}

						// Draw the name tag
						if(bbfont) { 
							bbfont->Begin(); 
							//bbfont->Draw(pPlayerManager->GetPlayerName(x), 0.285f, 0xFF000000);
							bbfont->Draw(pPlayerManager->GetPlayerName(x), 0.300f, Player->GetTeamColorAsARGB()); 
							bbfont->End(); 
						} 
					} 
				} 
			} 
		} 
	} 

	m_pD3DDevice->ApplyStateBlock(BarOldStateBlock); 
}
