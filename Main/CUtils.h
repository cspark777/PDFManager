#ifndef _CUtils_H_
#define _CUtils_H_

#if _MSC_VER > 1000
#pragma once
#endif


struct DROPLISTMODE 
{ 
	UINT iMask;								
	CString csFileExt;						
};


struct TypeIconRec
{
	UINT    id;
	CString type;
}; //TypeIconRecP, FAR * LPTypeIcon;

//*****************************************************************************
// CUtils class
//*****************************************************************************
class CUtils
{
public:
	static LPCTSTR UFileSize2Str(CString &outStr, DWORD inHi, DWORD inLo);
	static BOOL IsPDF(CString inFilePath);
	static BOOL SendFileToTheRecycleBin(LPCTSTR lpszFilePathList);
};

#endif