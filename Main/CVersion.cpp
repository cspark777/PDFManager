//CVersion.cpp

#include "stdafx.h"
#include "CVersion.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVersion::CVersion()
{
}

CVersion::~CVersion()
{
}

void CVersion::GetVersion(int& num1, int& num2, int& num3, int &num4)
{
	DWORD dwVerHnd=0;
	DWORD dwVerInfoSize;
	LPVOID lpBuffer = 0; 
	VS_FIXEDFILEINFO *lpvsFixedFileInfo = {0}; 
	UINT dwBytes = 0;

	TCHAR fileName[1024];

	// Version number
	GetModuleFileName(AfxGetInstanceHandle(), fileName, sizeof(fileName));

	//GetModuleFileName (hInst, szFullPath, sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(fileName, &dwVerHnd);

	if (dwVerInfoSize) 	{
		LPVOID   lpstrVffInfo;
		HANDLE  hMem;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpstrVffInfo = GlobalLock(hMem);
		GetFileVersionInfo(fileName, dwVerHnd, dwVerInfoSize, lpstrVffInfo);

		BOOL fRet = FALSE; 
		fRet = VerQueryValue(lpstrVffInfo, _T("\\"), &lpBuffer, &dwBytes); 

		if(fRet) { 
			lpvsFixedFileInfo = (VS_FIXEDFILEINFO *)lpBuffer; 
			num1 = lpvsFixedFileInfo->dwFileVersionMS>>16;
			num2 = lpvsFixedFileInfo->dwFileVersionMS & 0x0ffff;
			num3 = lpvsFixedFileInfo->dwFileVersionLS>>16;
			num4 = lpvsFixedFileInfo->dwFileVersionLS & 0x0ffff;
		}
		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
}


void CVersion::GetVersionString(char *outStrNum)
{
	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;
	GetVersion(v1, v2, v3, v4);
	outStrNum[0]='\0';
	sprintf(outStrNum, "%d.%d.%d.%d", v1, v2, v3, v4);
}

CString CVersion::GetVersionString(CString &outStr)
{
	char lVersion[100];
	GetVersionString(lVersion);
	outStr+=lVersion;
	return outStr;
}