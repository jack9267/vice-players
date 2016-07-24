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

extern "C" {
#include "../vendor/jpeg/jpeglib.h"
}

class CScreenshot
{
private:

	IDirect3DDevice8 *m_pD3DDevice;

	HANDLE CreateDibFromCurrentSurface();
	void FreeDib(HANDLE hDib);

	BOOL CreateJpegFileFromDib(HANDLE hDib, int nQuality, PCHAR szJpegFile);
	void ConvertDibToSamps(HANDLE hDib,int nSampsPerRow,
		struct jpeg_compress_struct cinfo,JSAMPARRAY jsmpPixels);

public:

	BOOL TakeScreenShot(PCHAR szFileName);
	CScreenshot(IDirect3DDevice8 *pD3DDevice);
	~CScreenshot() {};
};


//----------------------------------------------------