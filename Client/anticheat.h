//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): Kryptos
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "main.h"
#include "md5.h"

struct mdFile
{
	char szFileLocation[128];
	uint32 iCorrectDigestArray[4];
	unsigned char szRawLocalDigest[16];
};


// if anticheat functions return FALSE they have failed, otherwise TRUE

BOOL MemoryModificationCheck();
BOOL FileCheckSum();
BOOL CheckFile(mdFile* mdChkFile);
