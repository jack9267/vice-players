//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
//				   bpeterson
// License: See LICENSE in root directory
//
//-----------------------------------------------------

class CText
{
private:
	EntityId m_iID;

	DWORD m_dwColour;
	std::string m_szFont;
	int m_iSize;
	float m_fRelativePosX;
	float m_fRelativePosY;
	std::string m_szText;

	bool m_bShow;

public:
	CText(DWORD color, const char * szFontName, int iSize, float posX, float posY, const char * szText);
	~CText();

	void SetID(EntityId id);
	void SetText(const char * szText);
	void SetPosition(float fPosX, float fPosY);
	void SetColor(DWORD color);
	void Show(bool show);

	void InitForPlayer(EntityId playerId);
	void DestroyForPlayer(EntityId playerId);
	void InitForWorld();
};
