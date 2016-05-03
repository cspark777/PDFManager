// BenubirdAutoProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "LibraryDoc.h"
#include "BenubirdAutoProxy.h"
#include "MainFrm.h"
#include "LibraryView.h"
#include "CBRepository.h"
#include "CConvertToPDFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CBenubirdApp theApp;
extern CDB g_dbDefaultLibrary;
extern CView* IsOpenedLibPath(CString path);

CString g_strDBG;
// CBenubirdAutoProxy

IMPLEMENT_DYNCREATE(CBenubirdAutoProxy, CCmdTarget)

CBenubirdAutoProxy::CBenubirdAutoProxy()
{
	g_strDBG.Empty();
	g_strDBG.Format(_T("------------------------------------- start AutoProxy  %X"),this);
	OutputDebugString(g_strDBG);


	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CMainFrame, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))
		{
			m_pDialog = reinterpret_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CBenubirdAutoProxy::~CBenubirdAutoProxy()
{
	g_strDBG.Empty();
	g_strDBG.Format(_T("------------------------------------- end AutoProxy1  %X"),this);
	OutputDebugString(g_strDBG);

	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CBenubirdAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CBenubirdAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBenubirdAutoProxy, CCmdTarget)
	DISP_FUNCTION_ID(CBenubirdAutoProxy, "Show", dispidShow, Show, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CBenubirdAutoProxy, "AddFile", dispidAddFile, AddFile, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CBenubirdAutoProxy, "ConvertToPDF", dispidConvertToPDF, ConvertToPDF, VT_I4, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IBenubird to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {7A558363-0BC9-4962-85CD-01E74CB64771}
static const IID IID_IBenubird =
{ 0x7A558363, 0xBC9, 0x4962, { 0x85, 0xCD, 0x1, 0xE7, 0x4C, 0xB6, 0x47, 0x71 } };

BEGIN_INTERFACE_MAP(CBenubirdAutoProxy, CCmdTarget)
	INTERFACE_PART(CBenubirdAutoProxy, IID_IBenubird, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {B1789690-8215-4669-A691-174D19A147A3}
IMPLEMENT_OLECREATE2(CBenubirdAutoProxy, "Benubird.Application", 0xb1789690, 0x8215, 0x4669, 0xa6, 0x91, 0x17, 0x4d, 0x19, 0xa1, 0x47, 0xa3)


// CBenubirdAutoProxy message handlers


/*
void StartBenubird()
{
  //::ShellExecute(NULL, "open", strDLLDirectory, NULL, NULL, SW_NORMAL);
	CString strPath = _T("c:\\Program Files\\Benubird PDF\\BenubirdPDF.exe");
  ::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOW);
  //AfxMessageBox(strDLLDirectory);
}
*/

void CBenubirdAutoProxy::Show(void)
{ 
	g_strDBG.Empty();
	g_strDBG.Format(_T("------------------------------------- show AutoProxy  %X"),this);
	OutputDebugString(g_strDBG);

	//for default
	TCHAR szDefaultLib[0x100];
	memset(szDefaultLib,0,0x100*sizeof(TCHAR));
	g_dbDefaultLibrary.GetDefaultLibrary_For_Default(szDefaultLib);

	OutputDebugString(_T("------------------------  11"));
	theApp.CreateLibraryView(szDefaultLib);


	AFX_MANAGE_STATE(AfxGetAppModuleState());

	theApp.m_pMainWnd->ShowWindow(SW_SHOW); // Display the main Benubird application

// ----------------------------------
// Used to bring the dialogs displayed during the right-click menu
// to the foreground. For some reason, they were hiding in the background
// and other methods were failing...
// http://visualcpp.net/index.php?qID=52
// ----------------------------------
/*
	HWND hCurrWnd;
	int iMyTID;
	int iCurrTID;

	hCurrWnd = ::GetForegroundWindow();
	iMyTID   = GetCurrentThreadId();
	iCurrTID = GetWindowThreadProcessId(hCurrWnd,0);

	AttachThreadInput(iMyTID, iCurrTID, TRUE);
	SetForegroundWindow(theApp.m_pMainWnd->GetSafeHwnd());
	AttachThreadInput(iMyTID, iCurrTID, FALSE);
	*/
// ----------------------------------
}

LONG CBenubirdAutoProxy::AddFile(LPCTSTR file)
{
	//for default
	TCHAR szDefaultLib[0x100];
	memset(szDefaultLib,0,0x100*sizeof(TCHAR));
	g_dbDefaultLibrary.GetDefaultLibrary_For_Default(szDefaultLib);

	CView *pViewPtr = IsOpenedLibPath(szDefaultLib);

	CTFiles lTFiles;
	CIntArray lAddedFilesIDs;

	CStringList lFileNames;
	lFileNames.AddTail(file);

	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pViewPtr);

	int lAddedFiles = 0;
	
	lAddedFiles = pView->m_Repository.AddFiles(lFileNames);

	//int lAddedFiles = gRepository.AddFiles(lFileNames); pyj
	
	if (lAddedFiles > 0)
	{
		m_pDialog->InitFindText();
		
//?		if(((CLibraryDoc*)m_pDialog->GetActiveDocument())->GetMode() == CLibraryDoc::e_ModeHome)
//			((CLibraryView*)m_pDialog->GetActiveView())->Refresh();
		
	}
	return lAddedFiles;

}

LONG CBenubirdAutoProxy::ConvertToPDF(LPCTSTR files)
{
	OutputDebugString(_T("------------------------------------- convert AutoProxy"));

	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CStringList lFileList;
	CString lFiles = files;
	int lPos;
	CString lFile;

	//a tricky solution: if #part1 is true the #part2 is not executed
	while (((lPos = lFiles.Find(_T("####"))) != -1 /*#Part1*/) || ((lPos = lFiles.GetLength()) /*#Part2*/)) 
	{
		lFile = lFiles.Left(lPos);
		if (!lFile.IsEmpty())
			lFileList.AddTail(lFile);
		lFiles = lFiles.Right(lFiles.GetLength() - lPos - 4);
	}

	CConvertToPDFDlg lConvertToPDFDlg(&lFileList);
	lConvertToPDFDlg.DoModal();

	return 0;
}
