//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#define MAX_CPS 16

class CCheckpoint
{
public:
	CCheckpoint(EntityId id, Vector3 vecPos, BYTE type, float radius);
	~CCheckpoint();
	void Process();
	void InitForPlayer(EntityId playerId);
	void InitForWorld();
private:
	EntityId m_iID;
	Vector3 m_vecPos;
	float m_fRadius;
	BYTE m_byteType;
	bool m_bInCP;
};

class CCheckpoints
{
public:
	CCheckpoints();
	~CCheckpoints();
	void New(Vector3 vecPos, BYTE type, float radius);
	void Delete(EntityId id);
	void Process();

	void InitForPlayer(EntityId playerId);
	void InitForWorld();

	CCheckpoint * GetAt(EntityId id) {
		if(m_Checkpoints[id] = NULL) return NULL;
		return m_Checkpoints[id];
	};

	bool GetSlotState(EntityId cpId) {
		if(cpId > MAX_CPS) { return false; }
		return m_bSlotState[cpId];
	};

	EntityId GetFreeSlot();

private:
	CCheckpoint * m_Checkpoints[MAX_CPS];
	bool m_bSlotState[MAX_CPS];
};