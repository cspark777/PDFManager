#pragma once
#include "stdafx.h"
class CCheckForUpdates
{
public:
	static void CheckForUpdates(bool silent);
private:
	static UINT __cdecl CheckForUpdatesControllingFunction( LPVOID pParam );
};