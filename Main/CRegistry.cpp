// CRegistry.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "CRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*****************************************************************************
CRegistry::CRegistry(BOOL inReadOnly) : CBCGPRegistry(FALSE, inReadOnly)
{
}
//*****************************************************************************
CRegistry::~CRegistry()
{
}