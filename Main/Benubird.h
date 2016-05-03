#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CDB.h"
#include "CPPSQLite3u.h"
//#include "iSQP0711.h"

#ifndef _FREE
#define kSingleInstanceMutex _T("Benubird PDF Pro single instance mutex")
#else
#define kSingleInstanceMutex _T("Benubird PDF single instance mutex")
#endif

#define WIN_VER_5 0 // xp 2003
#define WIN_VER_6 1 // vista
#define WIN_VER_7 2 // 7

//#import "..\Libraries\iSedQuickPDF\QuickPDFAX0712.dll" //named_guids raw_interfaces_only
//#import "..\Libraries\iSedQuickPDF\iSED.dll" named_guids raw_interfaces_only

const int c_ViewCount = 3;
const LPCTSTR c_ViewNames[c_ViewCount] =
{
	_T("Home"), 
	_T("PDF"),
	_T("Shortcuts")
};
const LPCTSTR c_ViewDescription[c_ViewCount] =
{
	_T("The current view shows a library and allows user to navigate over the opened libary."),
	_T("The current view allows user to view and edit a pdf files."),
	_T("The current view allows user to view and edit a pdf files.")
};

#define DEFAULT_MAXIMUM_FILES_COUNT 50000

////////////////////////////////////////////////////////////////////////////////
// CBenubirdApp:
// See Benubird.cpp for the implementation of this class
////////////////////////////////////////////////////////////////////////////////
class CBenubirdApp : public CWinApp, public CBCGPWorkspace
{
private:
	bool _bAmITheFirst;
	HANDLE _hMapping;
	HANDLE _hMutex;
	struct SharedData
	{
		TCHAR pszCmdLine[0x1000];
		BOOL waitToProcess;
	}* _pSharedData;

	void InitializeOptions();
	void CheckForUpdates();
	//void SaveHeadersSize();
	virtual BOOL OnIdle(LONG lCount);

public:
	CMultiDocTemplate* m_pLibDocTemplate;
	CMultiDocTemplate* m_pPdfDocTemplate;

public:
	void OnOpenDefaultLibrary();
	void CreateLibraryView(LPCTSTR strLibName);
	void CreatePDFView(LPCTSTR strPDFName);

public:
	CBenubirdApp();

	virtual void PreLoadState ();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	CDocument* OpenDocumentFile_ForPDFView(LPCTSTR lpszFileName,LPCTSTR lpszLibName);

	bool IsRegistered();
	CString GetSerialNumber();

	UINT m_nAppLook;
	BOOL m_bAppInitialized;

public:
	bool ImportFilesFromZip_ForLibrary(TCHAR* szZipPath,char* szPassword,TCHAR *szRootPath);
	
	// Library
	afx_msg void OnNewLibrary();
	afx_msg void OnOpenLibrary();
	afx_msg void OnImportLibraryFromZip();
	afx_msg void OnExportLibraryToZip();

	DECLARE_MESSAGE_MAP()
};

extern CBenubirdApp theApp;
