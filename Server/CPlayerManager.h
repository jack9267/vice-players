//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

#define VALID_KILL		  1
#define TEAM_KILL		  2
#define SELF_KILL		  3

//----------------------------------------------------

class CPlayerManager
{
private:
	
	BOOL	m_bPlayerSlotState[MAX_PLAYERS];
	CPlayer *m_pPlayers[MAX_PLAYERS];
	CHAR	m_szPlayerName[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int		m_iPlayerScore[MAX_PLAYERS];
	BOOL	m_bIsAnAdmin[MAX_PLAYERS];
public:
	
	CPlayerManager();
	~CPlayerManager();

	void Process();

	BOOL New(EntityId playerID, PCHAR szPlayerName, PCHAR szSerial);
	BOOL Delete(EntityId playerID, BYTE byteReason);
		
	// Retrieve a player
	CPlayer* GetAt(EntityId playerID) {
		if(playerID > MAX_PLAYERS) { return NULL; }
		return m_pPlayers[playerID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(EntityId playerID) {
		if(playerID > MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[playerID];
	};

	PCHAR GetPlayerName(EntityId playerID) { return m_szPlayerName[playerID]; };

	BYTE AddResponsibleDeath(BYTE byteWhoKilled, BYTE byteWhoDied);
	float GetDistanceFromPlayerToPlayer(EntityId player1, EntityId player2);

	void SetAdmin(EntityId playerID) { m_bIsAnAdmin[playerID] = TRUE; };
	BOOL IsAdmin(EntityId playerID) { return m_bIsAnAdmin[playerID]; };

	int GetScore(EntityId playerID) { return m_iPlayerScore[playerID]; };

	BOOL IsNickInUse(PCHAR szNick);
	BOOL IsConnected(EntityId playerID);
	void SetGameTime(BYTE hours, BYTE minutes);

	int GetPlayerCount() {
		BYTE x=0,count=0;
		while(x!=MAX_PLAYERS) {
			if(GetSlotState(x)) count++;
			x++;
		}
		return (int)count;
	};

	int	m_iPause;
};

//----------------------------------------------------
