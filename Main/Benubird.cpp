// Benubird.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Benubird.h"

#include <Psapi.h>
#include "CLog.h"
#include "sn.h"

#include "CRegistry.h"
#include "MainFrm.h"
#include "LibraryDoc.h"
#include "LibraryView.h"
#include "PDFDoc.h"
#include "PDFView.h"
#include "CLimitSingleInstance.h"
#include "CSplashScreenDlg.h"
#include "CCheckForUpdates.h"
#include "CBRepository.h"
#include "CBWorkspace.h"
#include "COptionsDefs.h"
#include "zip.h"
#include "unzip.h"
#include "CImportZipDlg.h"
#include "ImportLibraryFromZip.h"

#include "CAboutDlg.h"
#include "CTrialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// To build free version, remove following definition
//#define PRO_VERSION

#define STRIKENUM_MAX 3
typedef struct _FindAppT
{
	TCHAR *pzExeName;
	HWND hwnd;
} 
FindAppT, *pFindAppT;

bool gIsPro = true;
DWORD g_dwWinVer;		

/// global variable

CTFilesUpdateOnIdle gTFilesUpdateOnIdle;

TCHAR g_szDBPath_Backup[MAX_PATH];

///
int nLibraryCount;

/// for restore previous option

CString g_strPrevLib[0x100];
CView *g_pViewArray[0x100];

// for default library
CDB g_dbDefaultLibrary;

//////////  Global variable
HWND g_AssistantHwnd = 0;
CString g_strLibPathArray[256];

int g_nMaximumFileCount = 0;

#define LIBPATHARRAY_LEN 256


/////////   Global functions

void InitLibPathArray()
{
	for(int i=0;i<LIBPATHARRAY_LEN;i++)
	{
		g_strLibPathArray[i].Empty();
		g_pViewArray[i] = NULL;
	}
}

CView* IsOpenedLibPath(CString path)
{
	for(int i=0;i<LIBPATHARRAY_LEN;i++)
	{
		if(wcscmp(path.GetBuffer(),g_strLibPathArray[i].GetBuffer())==0)
		{
			return g_pViewArray[i];
		}
	}
	return NULL;
}

BOOL InsertNewLibPath(CString path)
{
	for(int i=0;i<LIBPATHARRAY_LEN;i++)
	{
		if(g_strLibPathArray[i].IsEmpty())
		{
			g_strLibPathArray[i] = path;
			return TRUE;
		}
	}
	return FALSE;
}


BOOL SetCViewPointerOfLib(CString path,CView* pView)
{
	for(int i=0;i<LIBPATHARRAY_LEN;i++)
	{
		if(wcscmp(path.GetBuffer(),g_strLibPathArray[i].GetBuffer())==0)
		{
			g_pViewArray[i] = pView;
			return true;
		}
	}
	return false;
}

BOOL RemoveLibPath(CString path)
{
	for(int i=0;i<LIBPATHARRAY_LEN;i++)
	{
		if(wcscmp(path.GetBuffer(),g_strLibPathArray[i].GetBuffer())==0)
		{
			g_strLibPathArray[i].Empty();
			g_pViewArray[i] = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

CString GetDefaultPath(CString key)
{
	CString result;
	result.Empty();

	TCHAR lDBPath[MAX_PATH + 10];
	memset(lDBPath, 0, (MAX_PATH + 10) * sizeof(TCHAR));

	if (::SHGetSpecialFolderPath(NULL, lDBPath, CSIDL_APPDATA, TRUE) == FALSE) 
	{
		return result;		
	}
	
	if (::PathAppend(lDBPath, _T("Benubird\\")) == FALSE) 
	{
		return result;
	}
	CreateDirectory(lDBPath, NULL);

	if(::PathAppend(lDBPath, key.GetBuffer()) == FALSE) 
	{
		return result;
	}
	CreateDirectory(lDBPath, NULL);
	result = lDBPath;
	return result;	
}

CString GUIDgen()
{
 GUID guid;
 CoCreateGuid(&guid);

 WORD * str;
 UuidToString((UUID*)&guid, &str);

 CString unique((LPTSTR)str);

 RpcStringFree(&str);

 unique.Replace(_T("-"), _T("_"));

 return unique;
}

// windows version

CBenubirdApp theApp;



//CComPtr<iSED::IQuickPDF> gPDFLib = NULL;

BOOL CALLBACK FindApplicationWindowProc(HWND hwnd, LPARAM lParam)
{
	BOOL lRet = TRUE;
	DWORD		dwProcessID;
	INT		    iLen;
	TCHAR	    szTempName[MAX_PATH]=TEXT("\0");
	pFindAppT	pFindApp=(pFindAppT)lParam;

	::GetWindowThreadProcessId(hwnd,&dwProcessID);
	if (!dwProcessID) 
		return TRUE;

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!processHandle) 
		return TRUE;

	TCHAR* lFindChar = _T("\\");

	iLen = GetModuleFileNameEx(processHandle, NULL, szTempName,MAX_PATH);
	TCHAR* lTmp = _tcsrchr(szTempName, lFindChar[0]);
	int fnamePos = (int)(lTmp - szTempName);
//	iLen=::GetModuleFileName(processHandle,szTempName,MAX_PATH);
	if (iLen > 0)
	{
		TCHAR* lTmp = _tcsrchr(pFindApp->pzExeName, lFindChar[0]);
		int fnamePos2 = (int)(lTmp - pFindApp->pzExeName);

		if (!_tcsicmp(szTempName+fnamePos, pFindApp->pzExeName+fnamePos2) )
		{
			CString lStrAppName;
			lStrAppName.LoadString(IDR_MAINFRAME);

			// Check window title to make sure the main window is found.
			::GetWindowText(hwnd, szTempName, MAX_PATH);
			if (!_tcsnicmp(szTempName, lStrAppName, lStrAppName.GetLength()) ) // specify your title here.
			{	
				pFindApp->hwnd=hwnd;		
				lRet = FALSE;
			}
		}
	}
	CloseHandle(processHandle);

	return TRUE;
}


// CBenubirdApp

BEGIN_MESSAGE_MAP(CBenubirdApp, CWinApp)
//	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	//for library
	
	ON_COMMAND(ID_LIBRARY_NEW, OnNewLibrary)	
	ON_COMMAND(ID_LIBRARY_OPEN, OnOpenLibrary)
	ON_COMMAND(ID_LIBRARY_IMPORT, OnImportLibraryFromZip)
	ON_COMMAND(ID_LIBRARY_EXPORT, OnExportLibraryToZip)
END_MESSAGE_MAP()


// CBenubirdApp construction

CBenubirdApp::CBenubirdApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nAppLook = ID_VIEW_APPLOOK_2007_2;

	nLibraryCount = 0;
}

//limit to have just a single instance runing
//http://support.microsoft.com/kb/243953
CLimitSingleInstance gLimitSingleInstanceObj(TEXT("{6AA247BC-890B-4ad2-AB3A-F3FE55779166}"));
bool IsTechPrevEnd()
{
	return false;//case 206

/*	CTime lCurrentTime = CTime::GetCurrentTime();
	CTime lEndOfTechPreview(2008, 8, 7, 0, 0, 0);
	return (lCurrentTime > lEndOfTechPreview);
*/
}


const GUID CDECL BASED_CODE _tlid =
		{ 0x728777C4, 0xF5AC, 0x4023, { 0xAD, 0x55, 0xAC, 0xDA, 0xD7, 0xA7, 0x1E, 0xC5 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////////
// CBenubirdApp initialization
////////////////////////////////////////////////////////////////////////////////
BOOL CBenubirdApp::InitInstance()
{
	OutputDebugString(_T("PPPPPPPPPPPPPP KKKKKKKKKKKKKKKKK"));
	//run Assistance
	DWORD dwError = WinExec("BenubirdAssistant.exe",SW_SHOW);
	
	Sleep(1000);
	DWORD dwVersion, dwMajorVersion, dwMinorVersion;
	dwVersion = GetVersion();
 
	// Get the Windows version.
	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	if(dwMajorVersion >= 6 && dwMinorVersion >= 1)
		g_dwWinVer = WIN_VER_7;
	else if(dwMajorVersion >= 6 && dwMinorVersion >= 0)
		g_dwWinVer = WIN_VER_6;
	else if(dwMajorVersion >= 5 && dwMinorVersion >= 1)
		g_dwWinVer = WIN_VER_5;

	//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	if(gLimitSingleInstanceObj.IsAnotherInstanceRunning())
	{
		BOOL bGoAway = FALSE;
		INT	i, iValue;
		FindAppT findApp;
		TCHAR szTempName[MAX_PATH], szExeName[MAX_PATH];
		CWinThread* pThread = AfxGetThread();
		LPTSTR lpCmdLine = GetCommandLine();

		iValue=::GetModuleFileName(NULL,szExeName,MAX_PATH);			
		_tcscpy_s(szTempName,szExeName);
		for(i = 0; i < iValue; i++) 
		{
			if (szTempName[i]=='\\') 
				szTempName[i]='/';
		}

		memset(&findApp,0,sizeof(FindAppT));
		findApp.pzExeName=(TCHAR *)szExeName;
		iValue=pThread->GetThreadPriority();

		for (i=0; i< STRIKENUM_MAX; i++) 
		{
			::EnumWindows(FindApplicationWindowProc,(LPARAM)&findApp);

			if (findApp.hwnd) 
			{
				HWND hWnd = (HWND)findApp.hwnd;

				DWORD lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
				if (lStyle & WS_MINIMIZE)
				{
					//restore, show, activate window
					WINDOWPLACEMENT pl;
					pl.length = sizeof(WINDOWPLACEMENT);
					::GetWindowPlacement(hWnd, &pl);
					pl.showCmd = SW_RESTORE;
					::SetWindowPlacement(hWnd, &pl);
				}
				else
				{
					::SetForegroundWindow(hWnd);
					::SetFocus(hWnd);
				}

				// Send WM_COPYDATA to the first instance
				
				/*COPYDATASTRUCT* lCDS = new COPYDATASTRUCT;
				lCDS->dwData = 0;
				lCDS->cbData = ((DWORD)_tcslen(lpCmdLine)+1)*sizeof(TCHAR);
				lCDS->lpData = (WCHAR*) malloc(lCDS->cbData * sizeof(WCHAR));
				wcsncpy((WCHAR*)lCDS->lpData, lpCmdLine, lCDS->cbData);
				::SendMessage(hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)lCDS);*/

				/*typedef struct _CmdLineData_Entry
				{
				  TCHAR mPath[MAX_PATH];
				} CmdLineData_Entry, FAR * LPCmdLineData_Entry;*/

				//_CmdLineData_Entry* pCmdLineData_Entry = new _CmdLineData_Entry;
				//pCmdLineData_Entry->mPath = lpCmdLine;

				// Prepare the data transfer package
				BenubirdData_Transfer BenubirdData = {0};
				BenubirdData.mMessageType = Command_Line_Message;

				// Copy the path
				CString cmdLine = CString(lpCmdLine);
				wcscpy_s(BenubirdData.mCmdLineData.mPath, cmdLine.GetBufferSetLength(MAX_PATH));
				cmdLine.ReleaseBuffer();

				//AfxMessageBox(_T("Command line = ") + CString(BenubirdData.mCmdLineData.mPath));

				// Prepare the transfer copy structure
				COPYDATASTRUCT* pCOPYDATASTRUCT = new COPYDATASTRUCT;
				pCOPYDATASTRUCT->dwData = 1;							// function identifier
				pCOPYDATASTRUCT->cbData = sizeof(BenubirdData_Transfer)+1;
				pCOPYDATASTRUCT->lpData = &BenubirdData;

				// Call function, passing data in lCDS
				SendMessage(hWnd, WM_COPYDATA, (WPARAM) NULL, (LPARAM) (LPVOID) pCOPYDATASTRUCT);

/*				COPYDATASTRUCT cds;
				cds.dwData = 0;
				cds.cbData = ((DWORD)_tcslen(lpCmdLine)+1)*sizeof(WCHAR);
				cds.lpData = lpCmdLine; 
				::SendMessage(hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);*/

				//Command_Line_Message

				bGoAway=TRUE;
				break;
			}
			// wait for other app to finish starting or stopping
			pThread->SetThreadPriority(THREAD_PRIORITY_IDLE);
			Sleep(1000);    
			pThread->SetThreadPriority(iValue);
		}

		return FALSE;
	}

	
/*	if(IsTechPrevEnd()) // Technical preview has finished a long time ago -- Sep 25, 2009 
	{
		CTechPreviewEndDlg lTechPrevEndDlg;
		if(lTechPrevEndDlg.DoModal() == IDOK)
		{
			CString lStr;
			lStr.LoadString(IDS_TECH_PREVIEW_END_LINK);
			::ShellExecute(NULL, NULL, lStr, NULL, NULL, NULL);	
		}
		return FALSE;
	}
*/
	InitLibPathArray();
	// Register our unique class name that we wish to use
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

	//wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = LoadIcon(IDR_MAINFRAME);
	//wndcls.hCursor = LoadCursor( IDC_ARROW );
	//wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	//wndcls.lpszMenuName = NULL;

	//wndcls.style = ...;
	//... other wndcls variable inits here
	// Specify our own class name for using FindWindow later
	wndcls.lpszClassName = UniqueBenubirdClassName;
	// Register new class and exit if it fails
	if(!AfxRegisterClass(&wndcls))
	{
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Debenu"));
	SetRegistryBase (_T("Settings"));
	LoadStdProfileSettings(20);

#ifdef PRO_VERSION
	//check isRegistered or not ?
	int lDays = 0;
	int lState = 0;

	lState = sn::state(&lDays);

	switch (lState)
	{
	case sn::eStateRegistered:
		gIsPro = true;
		break;
	case sn::eStateExpired:
		{
/*				CSplashScreenDlg lDlg;
				INT_PTR lRet = lDlg.DoModal();
				if (lRet == IDCANCEL)
					return FALSE;
//		CTrialDlg trialDlg;
//		trialDlg.DoModal();
		AfxMessageBox(_T("Trial period is expired. You can continue using the product as a free version."), MB_OK);*/
		gIsPro = false;
		break;
		}
	case sn::eStateTrial:
		{
		CSplashScreenDlg lDlg;
		INT_PTR lRet = lDlg.DoModal();
		if (lRet == IDCANCEL)
			return FALSE;
//		CTrialDlg trialDlg;
//		trialDlg.DoModal();
//		CString strMsg;
//		strMsg.Format(_T("Trial version will be expired after %d days.\nPlease register the product within trial period."), lDays);
//		AfxMessageBox(strMsg, MB_ICONINFORMATION | MB_OK);
		gIsPro = true;
		break;
		}
	}
#else
		gIsPro = false;
#endif

	// Settings
	CTime lTime = CTime::GetCurrentTime();
	CString lStr;

	if(gIsPro)
	{
		lStr.Format(_T("Benubird PDF Pro launched %s\n"), lTime.Format(_T("%Y %B %d %H:%M:%S")));	
	}
	else
	{
		lStr.Format(_T("Benubird PDF launched %s\n"), lTime.Format(_T("%Y %B %d %H:%M:%S")));
	}

	CLog::StoreUserMessage(_T("*********************************************************************************"));
	CLog::StoreUserMessage(lStr);


	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	
	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// TODO: Remove this if you don't want extended tooltips:
	InitTooltipManager();
	InitShellManager();
	InitContextMenuManager();
	InitKeyboardManager();

	// Load
	m_nAppLook = GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2007_2);



	//------------------------------------------------------


	//for default library
	CString strPath1 = GetDefaultPath(_T(""));
	strPath1 = strPath1 + _T("DefaultLibrary.bb");
	g_dbDefaultLibrary.Initalize_For_Default(strPath1.GetBuffer());
	


	// -----------------------------------------------------

	CBCGPToolTipParams params;
	params.m_bVislManagerTheme = TRUE;

	theApp.GetTooltipManager ()->SetTooltipParams (
		BCGP_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS (CBCGPToolTipCtrl),
		&params);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	
	m_pLibDocTemplate = new CMultiDocTemplate(IDR_LIBRARY_TYPE,
		RUNTIME_CLASS(CLibraryDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLibraryView));

	if (!m_pLibDocTemplate)
		return FALSE;
	AddDocTemplate(m_pLibDocTemplate);

	m_pPdfDocTemplate = new CMultiDocTemplate(IDR_PDF_TYPE,
		RUNTIME_CLASS(CPDFDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CPDFView));
	if (!m_pPdfDocTemplate)
		return FALSE;
	AddDocTemplate(m_pPdfDocTemplate);

	
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,		NULL);

	// Parse command line for standard shell commands, DDE, file open
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);
//	CreateLibraryView(NULL);
//	pPdfDocTemplate->OpenDocumentFile(NULL);

	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

	m_bAppInitialized = TRUE;
	InitializeOptions();

	int lCmdRet = ((CMainFrame*)m_pMainWnd)->CmdLineProcess();

	if (lCmdRet == CMainFrame::cmdFalse)
		return FALSE;

	/////////////////////////////////////////////////////////////////////////
	// load QuickPDF library
	/*
	HRESULT hr = CoCreateInstance( iSED::CLSID_QuickPDF, NULL, CLSCTX_INPROC_SERVER, iSED::IID_IQuickPDF, (void**)&gPDFLib);
	//add your key code here
	if (gPDFLib)
	{
		CComBSTR key = "21A3F90CDEB8390A20E9CAC2609371B8";//change me!!
		long rtn = 0;//general return values
		hr = gPDFLib->UnlockKey(key,&rtn);//unlock the product
	}
	*/
	/////////////////////////////////////////////////////////////////////////

	//initialize Organized folder
//	COrganizer::Start();

	if (lCmdRet == CMainFrame::cmdContinueAndShow)
	{
		//	if application is in trial or is in expired mode
		/*
		if(gIsPro) // Only add this for the pro version
		{
			if (sn::state() != sn::eStateRegistered)
			{
				CSplashScreenDlg lDlg;
				INT_PTR lRet = lDlg.DoModal();
				if (lRet == IDCANCEL)
					return FALSE;
			}
		}*/
		// The one and only window has been initialized, so show and update it
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->SetFocus();
	}

	m_pMainWnd->UpdateWindow();

	//set the timers for watched folder
	//lucia GetMainWnd()->SetTimer( CTWFOLDER_FREQ_ALW, (10*1000), NULL);		//every N secconds
	//lucia GetMainWnd()->SetTimer( CTWFOLDER_FREQ_10MIN, (10*60*1000), NULL);
	//lucia GetMainWnd()->SetTimer( CTWFOLDER_FREQ_1HOUR, (60*60*1000), NULL);
	//lucia GetMainWnd()->SetTimer( CTWFOLDER_FREQ_1DAY, (24*60*60*1000), NULL);
	
	CheckForUpdates();

//?	pFrame->CalculateOutlookPercent();
	((CMainFrame*)m_pMainWnd)->RecalcLayout();

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	//LPTSTR lpCmdLine = GetCommandLine();
	//((CMainFrame*)m_pMainWnd)->ProcessNovaPDFCmdLine(lpCmdLine);

	return TRUE;
}

// CBenubirdApp message handlers

int CBenubirdApp::ExitInstance() 
{
	if(	gLimitSingleInstanceObj.IsAnotherInstanceRunning() ||
		IsTechPrevEnd())
			return CWinApp::ExitInstance(); //no cleanup BCG

	WriteInt(_T("ApplicationLook"), m_nAppLook);
	BCGCBProCleanUp();

//	COrganizer::End();

	CTime lTime = CTime::GetCurrentTime();
	CString lStr;

	if(gIsPro)
		lStr.Format(_T("Benubird PDF Pro %s\n"), lTime.Format(_T("%Y %B %d %H:%M:%S")));
	else
		lStr.Format(_T("Benubird PDF %s\n"), lTime.Format(_T("%Y %B %d %H:%M:%S")));

	CLog::StoreUserMessage(lStr);

	SaveStdProfileSettings();

	return CWinApp::ExitInstance();
}

bool CBenubirdApp::IsRegistered()
{
	CString lSerial = GetSerialNumber();
	return (sn::check(lSerial));
}

CString CBenubirdApp::GetSerialNumber()
{
	CString lSerial;

	//read serial number from registry

	return lSerial;
}



// CBenubirdApp message handlers
void CBenubirdApp::PreLoadState ()
{
}

void CBenubirdApp::CheckForUpdates() 
{
	time_t lNow;
	int lNextControl = 0;
		
	CString lNewVersionString, lDateString, lDetailsString;

	time(&lNow);

	int lCheck = GetProfileInt(_T("Options"), _T("CheckForUpdates"), 0);
	switch (lCheck)
	{
		case 1:
		case -1:
			break;
		default:
			lCheck = 0;
	}

	long lNextCheckForUpdates = GetProfileInt(_T("Options"), _T("NextCheckForUpdates"), 0);
	
	if (lNow < lNextCheckForUpdates)
		return;
		
	CCheckForUpdates::CheckForUpdates(true);
		
	switch (lCheck)
	{
		case 0: lNow += 86400; break;
		case 1: lNow += 86400 * 7; break;
	}
	WriteProfileInt(_T("Options"), _T("NextCheckForUpdates"), (int)  lNow);
}

BOOL CBenubirdApp::OnIdle(LONG lCount)
{
/*	// ----------------------------------------------
	if(!gUseRepository)  // perform real-time update of modifications to direct files
	{
		BOOL lNeedIdle = CWinApp::OnIdle(lCount);
		gTFilesUpdateOnIdle.UpdateFiles(); 
	}*/
	// ----------------------------------------------

	//gWFolderManager.ProcessWFolders();
//	return TRUE;

	return __super::OnIdle(lCount);
}

void CBenubirdApp::InitializeOptions()
{
	CWinApp* lApp;
	int lCheck;

	lApp = AfxGetApp();

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFTitle"), BST_CHECKED);
	lApp->WriteProfileInt(_T("Options"), _T("UFTitle"), lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFAuthor"), BST_CHECKED);
	lApp->WriteProfileInt(_T("Options"), _T("UFAuthor"), lCheck);
	
	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFSubject"), BST_UNCHECKED);
	lApp->WriteProfileInt(_T("Options"), _T("UFSubject"), lCheck);
	
	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFTags"), BST_CHECKED);
	lApp->WriteProfileInt(_T("Options"), _T("UFTags"), lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFCategories"), BST_UNCHECKED);
	lApp->WriteProfileInt(_T("Options"), _T("UFCategories"), lCheck);
		
	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFCollections"), BST_UNCHECKED);
	lApp->WriteProfileInt(_T("Options"), _T("UFCollections"), lCheck);

	g_nMaximumFileCount = lApp->GetProfileInt(kOptions, kMaximumFiles, BST_UNCHECKED);
	if(g_nMaximumFileCount == 0)
		g_nMaximumFileCount = DEFAULT_MAXIMUM_FILES_COUNT;
}

CDocument* CBenubirdApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(lpszFileName, lDrv, lFld, lNam, lExt);

	TCHAR *resExt = _wcsupr(lExt);
	if(wcscmp(resExt,_T(".PDF"))!=0)
	{
		ShellExecute(NULL, _T("edit"), lpszFileName, NULL, NULL, SW_SHOWNORMAL);
		return NULL;
	}
	
	CDocument* pDoc = m_pDocManager->OpenDocumentFile(lpszFileName);

//	AfxGetApp()->AddToRecentFileList(lpszFileName);

	return pDoc;
}

CDocument* CBenubirdApp::OpenDocumentFile_ForPDFView(LPCTSTR lpszFileName,LPCTSTR lpszLibName) //for PDFView
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(lpszFileName, lDrv, lFld, lNam, lExt);

	TCHAR *resExt = _wcsupr(lExt);
	if(wcscmp(resExt,_T(".PDF"))!=0)
	{
		HINSTANCE hRes = NULL;
		hRes = ShellExecute(NULL, _T("edit"), lpszFileName, NULL, NULL, SW_SHOWNORMAL);

		if((DWORD)hRes<32)
		{
			//error
			hRes = ShellExecute(NULL, _T("open"), lpszFileName, NULL, NULL, SW_SHOWNORMAL);
			if((DWORD)hRes<32)
			{
				hRes = ShellExecute(NULL, _T("explorer"), lpszFileName, NULL, NULL, SW_SHOWNORMAL);
			}
		}
		return NULL;
	}
	
	CDocument* pDoc = m_pDocManager->OpenDocumentFile(lpszFileName);
	CPDFDoc* pPDFDoc = DYNAMIC_DOWNCAST(CPDFDoc, pDoc);
	pPDFDoc->m_csDBPathName = lpszLibName;

//	AfxGetApp()->AddToRecentFileList(lpszFileName);

	return pDoc;
}

void CBenubirdApp::CreateLibraryView(LPCTSTR strLibName)
{	
	OutputDebugString(_T("------------------------  4"));
	OutputDebugString(strLibName);

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	
	int i = 0;
	TCHAR szTitle[MAX_PATH];

	CString strParam = strLibName;

	CView *pView = IsOpenedLibPath(strParam);
	if(pView != NULL)
	{
		//pFrame->SetActiveView(pView,TRUE);
		return;
	}
	
	OutputDebugString(strParam);

	if(!InsertNewLibPath(strParam))
		return;

	OutputDebugString(strParam);

	memset(g_szDBPath_Backup,0,sizeof(TCHAR)*MAX_PATH);
	wcscpy(g_szDBPath_Backup,strLibName);
	CDocument *pDoc = m_pLibDocTemplate->OpenDocumentFile(NULL);
	if(strLibName != NULL)
	{
		int nLen = wcslen(strLibName);
		for(i=nLen-1;i>0;i--)
		{
			if(strLibName[i] == '\\')
			{
				break;
			}
		}
		if(i==0)
		{
			RemoveLibPath(strParam);
			AfxMessageBox(_T("File Path error "));
			ExitProcess(0);
		}
		wcscpy(szTitle,(TCHAR*)(&strLibName[i+1]));
		nLen = wcslen(szTitle);
		szTitle[nLen-3] = 0;
		pDoc->SetTitle(szTitle);
	}

	OutputDebugString(_T("------------------------  5"));
}
void CBenubirdApp::CreatePDFView(LPCTSTR strPDFName)
{

}

///////////////////////////////////////////////////////////////////////////////////
// Library
///////////////////////////////////////////////////////////////////////////////////
void CBenubirdApp::OnNewLibrary()
{
	TCHAR csTmp[0x100];
	memset(csTmp,0,0x100*sizeof(TCHAR));

	BOOL res = g_dbDefaultLibrary.GetGlobalSettingInfo_For_Default(_T("defaultPathForCreateLibrary"),csTmp);
	
	CFileDialog dlg(false,_T(".bb"),  NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("BB Files (*.bb)|*.bb;|All Files(*.*)|*.*||")); 
	dlg.m_pOFN->lpstrTitle = _T("New Library");

	dlg.m_pOFN->lpstrInitialDir = csTmp;

	int nRet = dlg.DoModal();
	if(nRet == IDCANCEL)
		return;

	CString csFileName;
	csFileName = dlg.GetPathName();
	
	DWORD dwError = 0;
	BOOL bCanCreateDB = FALSE;

	HANDLE hDBFile = CreateFile(csFileName.GetBuffer(),GENERIC_READ,0,0,OPEN_EXISTING,0,0);
	if(hDBFile == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
		if(dwError == 0x02) //FILE_NOT_FOUND
		{
			bCanCreateDB = TRUE;
		}
	}
	else
	{
		CloseHandle(hDBFile);
		int nRet = MessageBox(NULL, _T("This library already exists, are you sure?"),_T("Warning"),MB_OKCANCEL);
		if(nRet == IDCANCEL)
		{
			return;
		}

		bCanCreateDB = TRUE;
	}

	if(bCanCreateDB == TRUE)
	{
		hDBFile = CreateFile(csFileName.GetBuffer(),GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
		if(hDBFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(_T("Can not create db file"));
			return ;
		}
		CloseHandle(hDBFile);
	}
	else
	{
		AfxMessageBox(_T("This library already exists and is open. Please close the library and try again"));
		return ;
	}

	OutputDebugString(_T("------------------------  6"));
	CreateLibraryView(csFileName.GetBuffer());	

	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(csFileName.GetBuffer(), lDrv, lFld, lNam, lExt);

	csFileName = lDrv;
	csFileName = csFileName + lFld;

	g_dbDefaultLibrary.SetGlobalSettingInfo_For_Default(_T("defaultPathForCreateLibrary"),csFileName.GetBuffer());

	
}
void CBenubirdApp::OnOpenLibrary()
{
	TCHAR csTmp[0x100];
	memset(csTmp,0,0x100*sizeof(TCHAR));

	BOOL res = g_dbDefaultLibrary.GetGlobalSettingInfo_For_Default(_T("defaultPathForCreateLibrary"),csTmp);

	CFileDialog dlg(true,_T(".bb"),  NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("BB Files (*.bb)|*.bb;|All Files(*.*)|*.*||")); 
	dlg.m_pOFN->lpstrTitle = _T("Open Library");
	dlg.m_pOFN->lpstrInitialDir = csTmp;

	int nRet = dlg.DoModal();
	if(nRet == IDCANCEL)
		return;
	CString csFileName;
	csFileName = dlg.GetPathName();
	
	DWORD dwError = 0;
	BOOL bCanCreateDB = FALSE;

	OutputDebugString(_T("------------------------  7"));
	CreateLibraryView(csFileName.GetBuffer());

	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(csFileName.GetBuffer(), lDrv, lFld, lNam, lExt);

	csFileName = lDrv;
	csFileName = csFileName + lFld;

	g_dbDefaultLibrary.SetGlobalSettingInfo_For_Default(_T("defaultPathForCreateLibrary"),csFileName.GetBuffer());
}

void CBenubirdApp::OnOpenDefaultLibrary()
{
	CString strPath = GetDefaultPath(_T(""));

	CString strDefaultPath = GetDefaultPath(_T("")) + _T("Home.db");

	OutputDebugString(_T("------------------------  8"));
	CreateLibraryView(strDefaultPath.GetBuffer());
}


bool CBenubirdApp::ImportFilesFromZip_ForLibrary(TCHAR* szZipPath,char* szPassword,TCHAR *szRootPath)
{
	TCHAR szTmpRootPath[0x100];
	memset(szTmpRootPath,0,0x100*sizeof(TCHAR));
	wcscpy(szTmpRootPath,szRootPath);

	HZIP hZip;
    ZIPENTRY zipEntry;
    ZIPENTRY zipItem;
    TCHAR szTmpFilePath[MAX_PATH];
	CString str_TmpDBPath;
	ZRESULT res = 0;
	hZip = OpenZip(szZipPath, szPassword);
    if (!hZip) return S_FALSE;

	wcscat(szTmpRootPath,_T("\\"));

    GetZipItem(hZip, -1, &zipEntry);
    for (int i = 0; i < zipEntry.index; i++)
    {
        GetZipItem(hZip, i, &zipItem);

		memset(szTmpFilePath,0,MAX_PATH*sizeof(TCHAR));
		wcscat(szTmpFilePath,szTmpRootPath);
		wcscat(szTmpFilePath,zipItem.name);

		if(wcsstr(zipItem.name,_T(".bb"))!=NULL)
		{
			str_TmpDBPath = szTmpFilePath;

			HANDLE file1 = CreateFile(str_TmpDBPath,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
			if(file1 == INVALID_HANDLE_VALUE)
			{
				DWORD dwError1 = GetLastError();
				if(dwError1 != ERROR_FILE_NOT_FOUND)
				{
					if(AfxMessageBox(_T("You are trying to overwrite existing libraryfile, are you sure?"), MB_OKCANCEL)!=IDOK)
					{
						continue;
					}
				}
			}
			else
			{
				CloseHandle(file1);
				if(AfxMessageBox(_T("You are trying to overwrite existing libraryfile, are you sure."), MB_OKCANCEL)!=IDOK)
				{
					continue;
				}
			}

		}

        //HANDLE hFile = CreateFile(szTmpFilePath,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
        //res = UnzipItemHandle(hZip, i, hFile);
		res = UnzipItem(hZip, i, szTmpFilePath);
    }
    CloseZip(hZip);

	CDB db;
	db.Initialize(str_TmpDBPath.GetBuffer());

	PARAM_FOR_CDB tmpCDB;
	memset(&tmpCDB,0,sizeof(PARAM_FOR_CDB));
	tmpCDB.pDB = &db;

	CTFiles files(&tmpCDB);
	BOOL bRet = files.UpdateFilePath(szTmpRootPath,0);
	if(bRet == false)
	{
		AfxMessageBox(_T("DB Creating is Failed"));
		return false;
	}
	db.Close();

	OutputDebugString(_T("------------------------  9"));
	CreateLibraryView(str_TmpDBPath);


    return S_OK;
}


void CBenubirdApp::OnImportLibraryFromZip()
{
//	CString strPath = GetDefaultPath(_T("Temp\\"));
	CString strPath;

/*		
	CImportZipDlg lDlg;
	lDlg.SetData_FromParent(NULL,1);
	char password[MAX_PATH];
	memset(password,0,MAX_PATH);

	if (lDlg.DoModal() == IDOK)
	{
		TCHAR* pPassword;
		if(lDlg.m_Password.IsEmpty())
		{
			pPassword = NULL;
		}
		else
		{
			pPassword = lDlg.m_Password.GetBuffer();
			int nLen = wcslen(pPassword);
			for(int i=0;i<nLen;i++)
			{
				password[i] = (char)pPassword[i];
			}
		}

		CString str_TmpGUID = GUIDgen();
		if (::PathAppend(lDBPath_tmp,str_TmpGUID.GetBuffer()) == FALSE) 
		{
			return ;
		}

		BOOL lRet = ImportFilesFromZip_ForLibrary(lDlg.m_SourcePath.GetBuffer(), password, lDBPath_tmp);
		return ;
		//Refresh();
	}
*/
	CImportLibraryFromZip lDlg;
	char password[MAX_PATH];
	memset(password,0,MAX_PATH);

	if (lDlg.DoModal() == IDOK)
	{
		TCHAR* pPassword;
		if(lDlg.m_StrPassword.IsEmpty())
		{
			pPassword = NULL;
		}
		else
		{
			pPassword = lDlg.m_StrPassword.GetBuffer();
			int nLen = wcslen(pPassword);
			for(int i=0;i<nLen;i++)
			{
				password[i] = (char)pPassword[i];
			}
		}

		strPath = strPath + lDlg.m_StrDestDirectory;
		CreateDirectory(strPath, NULL);

		BOOL lRet = ImportFilesFromZip_ForLibrary(lDlg.m_StrZipFilePath.GetBuffer(), password, strPath.GetBuffer());
	}
}

void CBenubirdApp::OnExportLibraryToZip()
{
	CString strPath = GetDefaultPath(_T("Temp\\"));

/*
	CFileDialog dlg(true,_T(".zip"),  NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Zip Archive Files (*.zip)|*.zip;|All Files(*.*)|*.*||")); 

	dlg.m_pOFN->lpstrInitialDir = lDBPath_tmp;

	int nRet = dlg.DoModal();
	if(nRet == IDCANCEL)
		return;
	CString csFileName;
	csFileName = dlg.GetPathName();
*/
	

	
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(this->m_pMainWnd))->MDIGetActive();
	CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView());

	pView->OnExport_Library_toZip(strPath.GetBuffer());

}



