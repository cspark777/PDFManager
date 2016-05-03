// CEmailUtils.cpp 
//*****************************************************************************

#include "stdafx.h"
#include <mapi.h>
#include "CEmailUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*****************************************************************************
CEmailUtils::CEmailUtils()
{
}
//*****************************************************************************
BOOL CEmailUtils::SendMail(HWND hWndParent, std::vector<CString> inFiles, LPSTR inSubject)
{
	if (inFiles.size() <= 0)
		return FALSE;

	if (!hWndParent || !::IsWindow(hWndParent))
		return FALSE;

	HINSTANCE hMAPI = ::LoadLibraryA("MAPI32.DLL");
	if (!hMAPI)
		return FALSE;

	ULONG (PASCAL *SendMail)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)SendMail = GetProcAddress(hMAPI, "MAPISendMail");

	if (!SendMail)
		return FALSE;

	const int lNumFiles = (int)inFiles.size();
		
	MapiFileDesc *lFileDesc = new MapiFileDesc[lNumFiles];
	LPSTR* lFileName = new LPSTR[lNumFiles];
	for (int i = 0; i < lNumFiles; ++i)
	{
		lFileName[i] = new char[_MAX_PATH];
		strcpy(lFileName[i], CT2CA(inFiles[i]));

		::ZeroMemory(&lFileDesc[i], sizeof(MapiFileDesc));
		lFileDesc[i].nPosition = (ULONG)-1;
		lFileDesc[i].lpszPathName = lFileName[i];
		lFileDesc[i].lpszFileName = lFileName[i];
	}

	MapiMessage lMessage;
	::ZeroMemory(&lMessage, sizeof(lMessage));
	lMessage.lpszSubject = inSubject;
	lMessage.nFileCount = lNumFiles;
	lMessage.lpFiles = lFileDesc;

	int lError = SendMail(0, (ULONG_PTR)hWndParent, &lMessage, MAPI_LOGON_UI|MAPI_DIALOG, 0);
	for (int i = 0; i < lNumFiles; ++i)
	{
		delete[] lFileName[i];
	}
	delete[] lFileName;
	delete[] lFileDesc;

	if (lError != SUCCESS_SUCCESS && lError != MAPI_USER_ABORT && lError != MAPI_E_LOGIN_FAILURE)
		return FALSE;

	return TRUE;
}
