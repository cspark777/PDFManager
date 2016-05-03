// CRegistry.h
//*****************************************************************************

#ifndef _CRegistry_H_
#define _CRegistry_H_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// CRegistry class
class CRegistry : public CBCGPRegistry
{

// Public methods
public:
	CRegistry(BOOL inReadOnly);
	virtual ~CRegistry();
	void SetKey(HKEY inKey){ m_hKey = inKey; }
};

#endif