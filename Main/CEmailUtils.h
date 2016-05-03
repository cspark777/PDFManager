// CEmailUtils.h
//*****************************************************************************

#ifndef _CEmailUtils_H_
#define _CEmailUtils_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "stdafx.h"
#include <vector>

//*****************************************************************************
// CEmailUtils class
class CEmailUtils
{

// Public methods
public:
	CEmailUtils();
	BOOL SendMail(HWND hWndParent, std::vector<CString> inFiles, LPSTR inSubject);
};

#endif