//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#define GAMESTATE_NONE			0
#define GAMESTATE_CONNECTING	1
#define GAMESTATE_CONNECTED		2
#define GAMESTATE_AWAIT_JOIN	3
#define GAMESTATE_DISCONNECTED  4

#define PACK_VEHICLE_HEALTH(f)		(BYTE)(f / 4)
#define UNPACK_VEHICLE_HEALTH(b)	(float)b * 4

typedef struct _C_VECTOR1 {
	short X,Y,Z;
} C_VECTOR1;

//----------------------------------------------------

void CompressVector1(Vector3 * vec, C_VECTOR1 * c1);
void DecompressVector1(Vector3 * vec, C_VECTOR1 * c1);

//----------------------------------------------------

using namespace RakNet;

//----------------------------------------------------

class CNetworkManager
{
private:

	CPlayerManager			*m_pPlayerManager;
	CVehicleManager		*m_pVehicleManager;
	CObjectManager			*m_pObjectManager;
	CPickupManager			*m_pPickupManager;
	CTextManager			*m_pTextManager;
	CCheckpoints		*m_pCheckpoints;
	RakPeerInterface	*m_pRakPeer;
	static RPC4			*m_pRPC4;
	CSpawnSelection	*m_pGameLogic;

	char                m_szHostOrIp[128];
	int                 m_iPort;
	char                m_szPass[128];
	int					m_iGameState;
	DWORD				m_dwLastScoreUpdateTick;
	UINT				m_uiLastRandSeed;

	// TODO: Make private
public:
	Vector3				m_vecInitPlayerPos;
	Vector3				m_vecInitCameraPos;
	Vector3				m_vecInitCameraLook;
	float				m_WorldBounds[4]; // pos_x neg_x pos_y neg_y
	BYTE				m_byteFriendlyFire;
	BYTE				m_byteShowOnRadar;

private:
	void UpdateNetwork();

	void PlayerSync(Packet *p);
	void VehicleSync(Packet *p);
	void ConnectionSucceeded(Packet *p);

public:
	CNetworkManager(PCHAR szHostOrIp,int iPort,PCHAR szPlayerName,PCHAR szPass);
	~CNetworkManager();

	int GetGameState() { return m_iGameState; };
	void SetGameState(int iGameState) { m_iGameState = iGameState; };

	CPlayerManager * GetPlayerManager() { return m_pPlayerManager; };
	CVehicleManager * GetVehicleManager() { return m_pVehicleManager; };
	CObjectManager * GetObjectManager() { return m_pObjectManager; };
	CPickupManager * GetPickupManager() { return m_pPickupManager; };
	CTextManager * GetTextManager() { return m_pTextManager; };
	CCheckpoints * GetCheckpoints() { return m_pCheckpoints; };
	RakPeerInterface * GetRakPeer() { return m_pRakPeer; };
	RPC4 * GetRPC4() { return m_pRPC4; };
	CSpawnSelection * GetGameLogic() { return m_pGameLogic; };
	BYTE GetFriendlyFire() { return m_byteFriendlyFire; };

	void InitGameLogic();
	void Process();
	void UpdatePlayerScoresAndPings();
	void Connect();
	bool IsConnected();
	void Shutdown();
};

//----------------------------------------------------