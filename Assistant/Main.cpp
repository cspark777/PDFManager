#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "CppSQLite3U.h"
#include "CNotifyDirCheck.h"
#include "FileFinder.h"
#include "COptionsDefs.h"
#include "CLimitSingleInstance.h"
#include "CBRepository.h"
#include "CDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////
// Declarations
///////////////////////////////////////////////////////////////////////////////////
void MainLoop();
void ProcessMessage_FromBenubird(int msg, int len,LPCTSTR strBuf);
bool GetWatchedFolders();
bool ReleaseWatchedFolders();

static DWORD WINAPI ProcessAlwaysWatchedFolderArray(CTWatchedFolderArray *pArray);
static DWORD WINAPI ProcessMinWatchedFolderArray(CTWatchedFolderArray *pArray);
static DWORD WINAPI ProcessHourWatchedFolderArray(CTWatchedFolderArray *pArray);
static DWORD WINAPI ProcessDayWatchedFolderArray(CTWatchedFolderArray *pArray);

//int AddFiles(CStringList &inFilesPathList, CTWatchedFolder_Entry& inFolderEntry, CIntArray *outAddedFilesIDs = NULL, CIntArray *outExistingFilesIDs = NULL);
void PeekMessages();

// CBenubirdDaemonApp initialization
// The one and only CBenubirdAssistantApp object

/////////////////// Global variable
CDB g_DB;
HWND g_BenubirdWnd = 0;
HWND g_AssistantWnd = 0;
CString g_strDBName = _T("watch.bb");
CString g_strDBFullPath;

CBenubirdAssistantApp theApp;

// Variables
UINT_PTR gTimer10min;
UINT_PTR gTimer1hour;
UINT_PTR gTimer1day;
UINT_PTR gTimerAlways;
bool gTimersPaused;

//CNotifyDirCheckArray gWatchedFolderArrayAlways;
CTWatchedFolderArray gWatchedFolderArrayAlways;
CTWatchedFolderArray gWatchedFolderArrayMin;	// array of CTWatchedFolder_Entry
CTWatchedFolderArray gWatchedFolderArrayHour;	// array of CTWatchedFolder_Entry
CTWatchedFolderArray gWatchedFolderArrayDay;	// array of CTWatchedFolder_Entry

CLimitSingleInstance gLimitSingleInstanceObj(TEXT("{3ba81000-7fe0-4e2d-9585-ce61f444399f}"));

// Global Mutex Object
/*
CCriticalSection g_mWatchFolderMinProcess;
CCriticalSection g_mWatchFolderHourProcess;
CCriticalSection g_mWatchFolderDayProcess;
//CCriticalSection g_mWatchFolderAlwaysProcess;

*/
HANDLE g_mWatchFolderMinProcess;
HANDLE g_mWatchFolderHourProcess;
HANDLE g_mWatchFolderDayProcess;
HANDLE g_mWatchFolderAlwaysProcess;

///////////////////////////////////////////////////////////////////////////////////
// CBenubirdAssistantApp
///////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBenubirdAssistantApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CBenubirdAssistantApp::CBenubirdAssistantApp() : 
CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
}

// CBenubirdAssistantApp initialization
BOOL CBenubirdAssistantApp::InitInstance()
{

	// Don't run if already executing
	if(gLimitSingleInstanceObj.IsAnotherInstanceRunning())
	{
		return FALSE;
	}

	g_mWatchFolderMinProcess = CreateMutex( NULL, FALSE, NULL);
	g_mWatchFolderHourProcess = CreateMutex( NULL, FALSE, NULL);
	g_mWatchFolderDayProcess = CreateMutex( NULL, FALSE, NULL);
	g_mWatchFolderAlwaysProcess = CreateMutex( NULL, FALSE, NULL);

	TCHAR lDBPath[MAX_PATH + 10];
	TCHAR lDBPath_tmp[MAX_PATH + 10];
	memset(lDBPath, 0, (MAX_PATH + 10) * sizeof(TCHAR));
	memset(lDBPath_tmp, 0, (MAX_PATH + 10) * sizeof(TCHAR));

	if (::SHGetSpecialFolderPath(NULL, lDBPath, CSIDL_APPDATA, TRUE) == FALSE) 
	{
		return FALSE;		
	}
	
	if (::PathAppend(lDBPath, _T("Benubird")) == FALSE) 
	{
		return FALSE;
	}
	CreateDirectory(lDBPath, NULL);

	memcpy(lDBPath_tmp,lDBPath,MAX_PATH + 10);
	if (::PathAppend(lDBPath_tmp, _T("Watch\\")) == FALSE) 
	{
		return FALSE;
	}
	CreateDirectory(lDBPath_tmp, NULL);
	g_strDBFullPath = lDBPath_tmp + g_strDBName;


	if (!g_DB.Initalize_For_Watch(g_strDBFullPath.GetBuffer()))
	{
		AfxMessageBox(_T("Error occured while initializing application"));
		return FALSE;
	}

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

	AfxEnableControlContainer();

	CBenubirdAssistantDlg *m_dlg = new CBenubirdAssistantDlg (NULL);
	m_dlg->Create(IDD_BENUBIRDASSISTANT_DIALOG,NULL);

	//gRepository.Initialize();

	
	MainLoop();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


int CBenubirdAssistantApp::ExitInstance() 
{
	if(	gLimitSingleInstanceObj.IsAnotherInstanceRunning())
	{
			return CWinApp::ExitInstance(); //no cleanup BCG
	}

	BCGCBProCleanUp();

	return CWinApp::ExitInstance();
}


///////////////////////////////////////////////////////////////////////////////////
// ASSISTANT DEFINITIONS
VOID CALLBACK Timer10minProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) 
{
	// Establish whether Workspace mode is to be used
	// -----------------------------------------------------
/*	if(GetProfileInt(kOptions, kUseRepository, BST_UNCHECKED) == BST_CHECKED)
	{
		//gUseRepository = true;
		TRACE(_T("Repository mode..."));
	}
	else
	{
		//gUseRepository = false;
		TRACE(_T("Direct mode..."));
	}
	*/
	// -----------------------------------------------------

	AfxBeginThread((AFX_THREADPROC)ProcessMinWatchedFolderArray, (LPVOID)&gWatchedFolderArrayMin);
}

VOID CALLBACK Timer1hourProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) 
{
	// Establish whether Workspace mode is to be used
/*	// -----------------------------------------------------
	if(GetProfileInt(kOptions, kUseRepository, BST_UNCHECKED) == BST_CHECKED)
	{
		//gUseRepository = true;
		TRACE(_T("Repository mode..."));
	}
	else
	{
		//gUseRepository = false;
		TRACE(_T("Direct mode..."));
	}
*/
	// -----------------------------------------------------

	AfxBeginThread((AFX_THREADPROC)ProcessHourWatchedFolderArray, (LPVOID)&gWatchedFolderArrayHour);
}

VOID CALLBACK Timer1dayProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) 
{
	// Establish whether Workspace mode is to be used
	// -----------------------------------------------------
/*	if(GetProfileInt(kOptions, kUseRepository, BST_UNCHECKED) == BST_CHECKED)
	{
		//gUseRepository = true;
		TRACE(_T("Repository mode..."));
	}
	else
	{
		//gUseRepository = false;
		TRACE(_T("Direct mode..."));
	}
*/	// -----------------------------------------------------

	AfxBeginThread((AFX_THREADPROC)ProcessDayWatchedFolderArray, (LPVOID)&gWatchedFolderArrayDay);
}

// lucia bez thread
VOID CALLBACK TimerAlwaysProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) 
{
/*
	// don't process files if another process is running
    CSingleLock lock(&g_mWatchFolderAlwaysProcess);

	if(lock.IsLocked())
		return;
    lock.Lock();
	*/
	AfxBeginThread((AFX_THREADPROC)ProcessAlwaysWatchedFolderArray, (LPVOID)&gWatchedFolderArrayAlways);

//	lock.Unlock();
	// Release Mutex
}
// end lucia

void MainLoop()
{
	int Counter=0;
	MSG Msg;
	gTimersPaused = true;
	gTimer10min = SetTimer(NULL, 0, 600000, &Timer10minProc);
	if (!gTimer10min)	return;
	gTimer1hour = SetTimer(NULL, 0, 3663000, &Timer1hourProc); //3600000 +1.5 min
	if (!gTimer1hour)	return;
	gTimer1day = SetTimer(NULL, 0, 86580000, &Timer1dayProc); //86400000 +3 min
	if (!gTimer1day)	return;

	// lucia bez thread
	gTimerAlways = SetTimer(NULL, 0, 10*1000, &TimerAlwaysProc);
	if (!gTimerAlways)	return;
	// end lucia

	gTimersPaused = false;
	
	//SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);

	//GetWatchedFolders();
	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_START, 0, 0);

	LRESULT ret;
	DWORD dwError;

	while (GetMessage(&Msg, NULL, 0, 0)) 
	{
		//	++Counter;
		OutputDebugString(L"GetMessage loop");
		if ( Msg.message == BENUBIRD_ASSISTANT_INIT) 
		{
			OutputDebugString(L"received BENUBIRD_ASSISTANT_INIT");
			g_BenubirdWnd = (HWND)Msg.wParam;
			COPYDATASTRUCT cds;
			cds.dwData = BENUBIRD_ASSISTANT_HWND;
			cds.cbData = 4;
			cds.lpData = &g_AssistantWnd;
			ret = ::SendMessage((HWND)g_BenubirdWnd,WM_COPYDATA,0,(LPARAM)&cds);
			OutputDebugString(L"Sent BENUBIRD_ASSISTANT_HWND");
			dwError = GetLastError();
		}
		if ( Msg.message == BENUBIRD_ASSISTANT_STOP) 
		{
			gTimersPaused = true;
			ReleaseWatchedFolders();
		}
		if ( Msg.message == BENUBIRD_ASSISTANT_START) 
		{
			GetWatchedFolders();
			gTimersPaused = false;
		}

		DispatchMessage(&Msg);
	}

	// final cleaning
	KillTimer(NULL, gTimer10min);
	KillTimer(NULL, gTimer1hour);
	KillTimer(NULL, gTimer1day);
	KillTimer(NULL, gTimerAlways);
	
	ReleaseWatchedFolders();
}

///////////////////////////////////////////////////////////////////////////////////
// Watched Folders 
bool ReleaseWatchedFolders()
{
	/* lucia bez thread
	for (int i=0; i<gWatchedFolderArrayAlways.GetCount(); i++)
	{
		CNotifyDirCheck* lNotifyDir = (CNotifyDirCheck*)gWatchedFolderArrayAlways.GetAt(i);
		lNotifyDir->Stop();
		delete(lNotifyDir);
	}
	*/
	
	gWatchedFolderArrayAlways.RemoveAll();
	gWatchedFolderArrayMin.RemoveAll();
	gWatchedFolderArrayHour.RemoveAll();
	gWatchedFolderArrayDay.RemoveAll();

	return true;
}

bool GetWatchedFolders()
{
	HANDLE file;

	// don't process files if another process is running
	DWORD dwWaitResult = WaitForSingleObject( g_mWatchFolderMinProcess, INFINITE);
	if(dwWaitResult != WAIT_OBJECT_0)
		return 0;
	dwWaitResult = WaitForSingleObject( g_mWatchFolderHourProcess, INFINITE);
	if(dwWaitResult != WAIT_OBJECT_0)
		return 0;
	dwWaitResult = WaitForSingleObject( g_mWatchFolderDayProcess, INFINITE);
	if(dwWaitResult != WAIT_OBJECT_0)
		return 0;
	dwWaitResult = WaitForSingleObject( g_mWatchFolderAlwaysProcess, INFINITE);
	if(dwWaitResult != WAIT_OBJECT_0)
		return 0;

	ReleaseWatchedFolders();

	CDB m_DB;
	CppSQLite3Query mQ; // for global watch library
	CppSQLite3Query mQ1;// for normal library
	CString lSQL;

	//mQ = g_DB.mDB.execQuery(_T("select * from librarys where Mode=1"));
	mQ = g_DB.mDB.execQuery(_T("select * from librarys"));
	while (!mQ.eof())
	{
		CString strLib = mQ.getStringField(1);
		int mode = mQ.getIntField(2);

		file = CreateFile(strLib.GetBuffer(),GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
		if(file == INVALID_HANDLE_VALUE)
		{
			DWORD error = GetLastError();
			if(error == ERROR_FILE_NOT_FOUND)
			{
				//deleted library , so remove the record from watch db.
				lSQL.Format(_T("delete from librarys where lib='%s'"),strLib.GetBuffer());
				g_DB.mDB.execDML(lSQL);
				mQ.nextRow();	
				continue;
			}
		}
		CloseHandle(file);
			
		
		m_DB.Initialize(strLib.GetBuffer());

		mQ1 = m_DB.mDB.execQuery(_T("select * from WFolders"));

		CTWatchedFolder_Entry lEntry;
		lEntry.mLibraryPathName = strLib;
		lEntry.nMode = mode;
		while (!mQ1.eof())
		{
			lEntry.mID = mQ1.getIntField(0);
			lEntry.mName = mQ1.getStringField(1);
			lEntry.mPath = mQ1.getStringField(2);
			lEntry.mIncludeSubfolders = (mQ1.getIntField(3) != 0);
			lEntry.mAllFileType = (mQ1.getIntField(4) != 0);
			lEntry.mCollectionID = mQ1.getIntField(5);
			lEntry.mCheckFrequency = mQ1.getIntField(6);

			lEntry.mUseTitle = mQ1.getIntField(7);
			lEntry.mTitle = mQ1.getStringField(8);
			lEntry.mUseAuthor = mQ1.getIntField(9);
			lEntry.mAuthor = mQ1.getStringField(10);
			lEntry.mUseSubject = mQ1.getIntField(11);
			lEntry.mSubject = mQ1.getStringField(12);
			lEntry.mUseCategories = mQ1.getIntField(13);
			lEntry.mCategories = mQ1.getStringField(14);
			lEntry.mUseComments = mQ1.getIntField(15);
			lEntry.mComments = mQ1.getStringField(16);
			lEntry.mUseTags = mQ1.getIntField(17);

			switch (lEntry.mCheckFrequency)
			{
			//case CTWFOLDER_FREQ_ALW:{
			case 0:{
				//CNotifyDirCheck* lNotifyDir = new CNotifyDirCheck(lEntry);
				//gWatchedFolderArrayAlways.Add(lNotifyDir);
				gWatchedFolderArrayAlways.Add(lEntry);
				}break;
			case 1:
			//case CTWFOLDER_FREQ_10MIN:
				gWatchedFolderArrayMin.Add(lEntry);
				break;
			case 2:
			//case CTWFOLDER_FREQ_1HOUR:
				gWatchedFolderArrayHour.Add(lEntry);
				break;
			case 3:
			//case CTWFOLDER_FREQ_1DAY:
				gWatchedFolderArrayDay.Add(lEntry);
				break;
			default:
				break;
			}

			mQ1.nextRow();	

		}
		mQ.nextRow();	
	}
	
	// run watched folders which have the "always" flag
	/* lucia bez thread
	for (int i=0; i<gWatchedFolderArrayAlways.GetCount(); i++)
	{
		CNotifyDirCheck* lNotifyDir = (CNotifyDirCheck*)gWatchedFolderArrayAlways.GetAt(i);
		ProcessWatchedFolderEntry(lNotifyDir->mWFolderEntry, &lNotifyDir->mProcessingEntry);

		lNotifyDir->Run();
	}
	*/

	ReleaseMutex(g_mWatchFolderMinProcess);
	ReleaseMutex(g_mWatchFolderHourProcess);
	ReleaseMutex(g_mWatchFolderDayProcess);
	ReleaseMutex(g_mWatchFolderAlwaysProcess);

	// process folders at start
	AfxBeginThread((AFX_THREADPROC)ProcessAlwaysWatchedFolderArray, (LPVOID)&gWatchedFolderArrayAlways);
	AfxBeginThread((AFX_THREADPROC)ProcessMinWatchedFolderArray, (LPVOID)&gWatchedFolderArrayMin);
	AfxBeginThread((AFX_THREADPROC)ProcessHourWatchedFolderArray, (LPVOID)&gWatchedFolderArrayHour);
	AfxBeginThread((AFX_THREADPROC)ProcessDayWatchedFolderArray, (LPVOID)&gWatchedFolderArrayDay);

	return true;
}

// the similar method as CTFiles::AddFiles but a little bit modified
/*
int AddFiles(CStringList &inFilesPathList, CTWatchedFolder_Entry& inFolderEntry, CIntArray *outAddedFilesIDs, CIntArray *outExistingFilesIDs)
{
	CTFiles lTFiles;

	int lRetVal = 0;
	try
	{
		int lNumFiles = (int)inFilesPathList.GetCount();
		CString lSQL_AddFile;
		POSITION lPos = inFilesPathList.GetHeadPosition();	
		CppSQLite3Query lQ;
		CString lEncodedPath, lEncodedName;
		CString lProgTex;
		int lCnt = 0;
		
		while(lPos)
		{
			PeekMessages();
			if (gTimersPaused) 
				return false;

			CString lTmpStr = inFilesPathList.GetNext(lPos);
			lTFiles.mPath = ExpandShortcut(lTmpStr);
			if(lTFiles.mPath.IsEmpty())
				lTFiles.mPath = lTmpStr;

			if (!CTFiles::IsAddThisFileEnabled(lTFiles.mPath, true, true))
				continue;

			CString lSQL;
			CString lEncPath;
			EncodeString(lTFiles.mPath, lEncPath);
			lSQL.Format(_T("select idFile from Files where Path LIKE '%s';"), lEncPath);	
			lQ = gDB.mDB.execQuery(lSQL); 
			if(!lQ.eof())
			{
				int lID = lQ.getIntField(0, 0);
				if(outExistingFilesIDs != NULL && lID != 0)
					outExistingFilesIDs->Add(lID);
					
				continue; //do not add this file more time
			}

			//If Benubird is running - send the message with the data to the Benubird and
			//add the file in there. 

			//Another possible communications:
			//- sockets
			//- named pipes
			//- remoting
			//- memory mapped files
			//- windows messages ( e.g. WM_COPYDATA )
			

			// Determine if another window with our class name exists...
 			HWND lHwnd = FindWindow(UniqueBenubirdClassName, NULL);
			//if (pWndPrev = CWnd::FindWindow(_T("MyNewClass"), NULL))
			if( lHwnd != NULL )
			{
				COPYDATASTRUCT* lCDS = new COPYDATASTRUCT;
				AssistantData_Entry* lEntry = new AssistantData_Entry;

				// Copy data into structure to be passed via WM_COPYDATA.
				// Also, we assume that truncation of the data is acceptable.
				HRESULT hResult;
				hResult = StringCbCopy(lEntry->mPath, sizeof(lEntry->mPath), lTFiles.mPath);
				if (hResult != S_OK)
					continue;
				lEntry->mWFolderID = inFolderEntry.mID;

				// Fill the COPYDATA structure
				lCDS->dwData = 1;							// function identifier
				lCDS->cbData = sizeof(AssistantData_Entry);	// size of data
				lCDS->lpData = lEntry;						// data structure

				// Call function, passing data in lCDS
				SendMessage(lHwnd, WM_COPYDATA, 0, (LPARAM) (LPVOID) lCDS );

				continue;
				//PostMessage(WM_MYMSG, reinterpret_cast<WPARAM> (ptr) , static_cast<LPARAM> 0);
			}


			// fill the private members in CTFiles class
			//lTFiles.mFlags; //0x1 - missing,
			lTFiles.mExtID = -1;
			lTFiles.m_ProgressDlg = NULL;
			lTFiles.mName = _T("");

			// load summary fom file (from disc!)
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = !inFolderEntry.mUseTitle;
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = !inFolderEntry.mUseSubject;
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = !inFolderEntry.mUseAuthor;
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = !inFolderEntry.mUseCategories;	
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = !inFolderEntry.mUseTags;	
			lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = !inFolderEntry.mUseComments;	
			lTFiles.LoadDataFromFile(); // loads also dates

			// update summary info from watched folder settings
			if(inFolderEntry.mUseTitle != 0)
			{
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = true;
				lTFiles.mTitle = inFolderEntry.mTitle;
			}
			else
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_TITLE] = false;


			if(inFolderEntry.mUseAuthor != 0)
			{
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = true;
				lTFiles.mAuthor = inFolderEntry.mAuthor;
			}
			else
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_AUTHOR] = false;

			if(inFolderEntry.mUseSubject != 0)
			{
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = true;
				lTFiles.mSubject = inFolderEntry.mSubject;
			}
			else
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_SUBJECT] = false;

			if(inFolderEntry.mUseCategories != 0)
			{
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = true;
				lTFiles.mCategory = inFolderEntry.mCategories;
			}
			else
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_CATEGORY] = false;

			if(inFolderEntry.mUseComments)
			{
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = true;
				lTFiles.mComments = inFolderEntry.mComments;
			}
			else
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_COMMENTS] = false;

			if(inFolderEntry.mUseTags)
			{
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = true;
				// read tags from the watched folder to lTFiles.mKeywords
				CIDData lTagsIDData;
				CTTags lTTags;
				if(lTTags.GetCIDDataByWFolderID(lTagsIDData, inFolderEntry.mID))
				{
					lTagsIDData.GetStringsFormatted(lTFiles.mKeywords, _T("; "));
				}
			}
			else
				lTFiles.mUpdatingFileSummaryInfoRequired[FINFO_UPDATE_KEYWORDS] = false;
		
			CString lEncName, lEncTitle, lEncSubject, lEncAuthor, lEncCat, lEncComments;
			EncodeString(lTFiles.mFindData.cFileName, lEncName);
			EncodeString(lTFiles.mTitle, lEncTitle);
			EncodeString(lTFiles.mSubject, lEncSubject);
			EncodeString(lTFiles.mAuthor, lEncAuthor);
			EncodeString(lTFiles.mCategory, lEncCat);
			EncodeString(lTFiles.mComments, lEncComments);
		
			gDB.GetCurrentFileTime(lTFiles.mAddedDate);
			CString lCreatedTmStmp, lModifiedTmStmp, lAccessedTmStmp, lAddedTmStmp;
			Filetime2Timestamp(lTFiles.mFindData.ftCreationTime, lCreatedTmStmp);
			Filetime2Timestamp(lTFiles.mFindData.ftLastWriteTime, lModifiedTmStmp);
			Filetime2Timestamp(lTFiles.mFindData.ftLastAccessTime, lAccessedTmStmp);
			Filetime2Timestamp(lTFiles.mAddedDate, lAddedTmStmp);

			// I need to update the summary info of the file on the disc at first - due to
			// avoid conflicts with the UpdadeFiles function from Benubird! 
			lTFiles.UpdateFileOnDiskFromMembers();

			lSQL_AddFile.Format(_T("insert into Files values (NULL, '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, '%s', '%s', '%s', '%s')"), 
				lEncPath,
				lEncName,
				lTFiles.mFindData.nFileSizeHigh,
				lTFiles.mFindData.nFileSizeLow,
				lTFiles.mFindData.ftCreationTime.dwHighDateTime, //cr time
				lTFiles.mFindData.ftCreationTime.dwLowDateTime,
				lTFiles.mFindData.ftLastWriteTime.dwHighDateTime, //wr time
				lTFiles.mFindData.ftLastWriteTime.dwLowDateTime,
				lTFiles.mFindData.ftLastAccessTime.dwHighDateTime,
				lTFiles.mFindData.ftLastAccessTime.dwLowDateTime,
				lTFiles.mFindData.dwFileAttributes,
				lEncTitle,
				lEncSubject,
				lEncAuthor,
				lEncCat,
				lEncComments,
				lTFiles.mAddedDate.dwHighDateTime,
				lTFiles.mAddedDate.dwLowDateTime,
				lTFiles.mFlags,
				lTFiles.mExtID,
				lCreatedTmStmp,
				lModifiedTmStmp,
				lAccessedTmStmp,
				lAddedTmStmp
				);
		
			gDB.mDB.execDML(lSQL_AddFile);

			// get the identifier of the currently inserted file
			int lID = lTFiles.GetFileIDByPath(lTFiles.mPath);

			///////////////////////////////////////////////////////////////////////////////
			// additional tables can be updated only after the insert into the CTFile table 
			// update keywords table (tags)
			lTFiles.AddTagsFromKeywords(lID, lTFiles.mKeywords);

			// update collections
			if(inFolderEntry.mCollectionID > 0)
				gDB.AddFileToCollection(inFolderEntry.mCollectionID, lID);

			//CString lLogMsg;
			//lLogMsg.Format(_T("Add file: %s"), lEncPath);
			//CLog::StoreUserMessage(lLogMsg);

			if(outAddedFilesIDs != NULL)
			{
				outAddedFilesIDs->Add(lID);
			}
			lRetVal++;
		}
	}
	catch(...)
	{
		//CLog::StoreUserMessage(_T("Add files error"));
	}

	return lRetVal;
}
*/
void FileFinderAssistantProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam)
{
	CString			sText, sNewFile;
	CStringList		*pStrList = (CStringList *)pCustomParam;

	switch (dwCode)
	{
	case FF_FOUND:
		// Update list
		sNewFile = pFinder->GetFilePath(pFinder->GetFileCount() - 1);
		pStrList->AddTail(sNewFile);
	}

	// Process all process messages
	PeekMessages();
	if (gTimersPaused)
	{
		pFinder->StopSearch();
	}
}

bool AssistantLoadFilesFromDisk(LPCTSTR inFolderPath, bool inWithSubfolders, CStringList *outFilePaths)
{
	CFileFinder lFFinder;
	lFFinder.SetCallback(FileFinderAssistantProc, outFilePaths);

//	_finder.StopSearch();
	CFileFinder::CFindOpts	lFOpts;
	// Set CFindOpts object
	lFOpts.sBaseFolder = inFolderPath;
	lFOpts.bSubfolders = inWithSubfolders;
	lFOpts.FindNormalFiles();

	lFFinder.RemoveAll();
	lFFinder.Find(lFOpts);

	return true;
}

bool ProcessWatchedFolderEntry(CTWatchedFolder_Entry& inFolderEntry)
{
	CStringList lFilePaths;
	CppSQLite3Query lQ;
	CString lSQL;
	CIntArray lAddedFileIDs;

	// don't process files if another process is running
	if (gTimersPaused)
	{
		return false; 
	}

	AssistantLoadFilesFromDisk(inFolderEntry.mPath, inFolderEntry.mIncludeSubfolders, &lFilePaths);
	if(lFilePaths.GetCount() == 0)
	{
		return true;
	}

	try
	{
		CDB tmpDB;
		tmpDB.Initialize(inFolderEntry.mLibraryPathName);
		
		if(inFolderEntry.mAllFileType == false) //do not add all files!!!
		{ 
			CTFExtensions lFExt(&tmpDB);
			CIDData lWFolderExtensions;
			lFExt.GetCIDDataByWFolderID(lWFolderExtensions, inFolderEntry.mID);
			KeepPathsByExtension(lFilePaths, lWFolderExtensions.mStrs);
		}


		int lFilesAddedNow = 0;
		TCHAR szTmp[0x100];
		memset(szTmp,0,0x100*sizeof(TCHAR));

		PARAM_FOR_CDB tmpParam;
		tmpParam.pDB = &tmpDB;
		tmpParam.nIsUseRepository = inFolderEntry.nMode;
		CBRepository mRepository;
		mRepository.Set_ParamForCDB(&tmpParam);
		if(tmpParam.pDB->GetInfoValue(_T("Repository"),szTmp)==0)
		{
			AfxMessageBox(_T("Get Repository Failed"));
			ExitProcess(0);
		}

		mRepository.InitializePath(szTmp);
		
		if(inFolderEntry.nMode) // Add files to the repository
		{
			 lFilesAddedNow = mRepository.AddFiles(lFilePaths, &inFolderEntry, true);
		}
		else
		{
			//pyj
		// add all files directly
			
			
			CTFiles lTFiles(&tmpParam);
			lTFiles.AddFiles(lFilePaths, &inFolderEntry, &lAddedFileIDs, NULL);
			lFilesAddedNow = (int)lAddedFileIDs.GetCount();
			
		}
		
		// some files have been added, refresh grid
		if (lFilesAddedNow > 0)
		{ 
			LRESULT ret;
			COPYDATASTRUCT cds;
			cds.dwData = BENUBIRD_ASSISTANT_REFRESH;
			cds.cbData = (inFolderEntry.mLibraryPathName.GetLength()+1)*2;
			cds.lpData = inFolderEntry.mLibraryPathName.GetBuffer();
			ret = ::SendMessage((HWND)g_BenubirdWnd,WM_COPYDATA,0,(LPARAM)&cds);
			//::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH, 0, 0); 
		}

		// *** what happened to refresh a single file *****
	}
	catch(...)
	{
	//	return false;
	} 

	return true;	
}
/*
ThreadWatchedFolderArray(CTWatchedFolderArray *pArray)
{
	// don't process files if another process is running
    CSingleLock lock(&g_mWatchFolderProcess);

    lock.Lock();


	lock.Unlock();
	return 0;
}
*/
static DWORD WINAPI ProcessAlwaysWatchedFolderArray(CTWatchedFolderArray *pArray)
{
	// Wait for ownership
	DWORD dwWaitResult = WaitForSingleObject( g_mWatchFolderAlwaysProcess, 0);
	if(dwWaitResult == WAIT_TIMEOUT)
		return 0;

	try
	{
		for (int i=0; i<pArray->GetCount(); i++)
		{
			CTWatchedFolder_Entry lFolderEntry = (CTWatchedFolder_Entry)pArray->GetAt(i);
			ProcessWatchedFolderEntry(lFolderEntry);
		}
	}
	catch(...)
	{
	}

	ReleaseMutex(g_mWatchFolderAlwaysProcess);

	return 0;
}
static DWORD WINAPI ProcessMinWatchedFolderArray(CTWatchedFolderArray *pArray)
{
	// Wait for ownership
	DWORD dwWaitResult = WaitForSingleObject( g_mWatchFolderMinProcess, 0);
	if(dwWaitResult == WAIT_TIMEOUT)
		return 0;

	try
	{
		for (int i=0; i<pArray->GetCount(); i++)
		{
			CTWatchedFolder_Entry lFolderEntry = (CTWatchedFolder_Entry)pArray->GetAt(i);
			ProcessWatchedFolderEntry(lFolderEntry);
		}		
	}
	catch(...)
	{
	}
	ReleaseMutex(g_mWatchFolderMinProcess);
	return 0;
}
static DWORD WINAPI ProcessDayWatchedFolderArray(CTWatchedFolderArray *pArray)
{
	// Wait for ownership
	DWORD dwWaitResult = WaitForSingleObject( g_mWatchFolderDayProcess, 0);
	if(dwWaitResult == WAIT_TIMEOUT)
		return 0;

	try
	{
		for (int i=0; i<pArray->GetCount(); i++)
		{
			CTWatchedFolder_Entry lFolderEntry = (CTWatchedFolder_Entry)pArray->GetAt(i);
			ProcessWatchedFolderEntry(lFolderEntry);
		}		
	}
	catch(...)
	{
	}
	ReleaseMutex(g_mWatchFolderDayProcess);
	return 0;
}
static DWORD WINAPI ProcessHourWatchedFolderArray(CTWatchedFolderArray *pArray)
{
	// Wait for ownership
	DWORD dwWaitResult = WaitForSingleObject( g_mWatchFolderHourProcess, 0);
	if(dwWaitResult == WAIT_TIMEOUT)
		return 0;

	try
	{
		for (int i=0; i<pArray->GetCount(); i++)
		{
			CTWatchedFolder_Entry lFolderEntry = (CTWatchedFolder_Entry)pArray->GetAt(i);
			ProcessWatchedFolderEntry(lFolderEntry);
		}		
	}
	catch(...)
	{
	}
	ReleaseMutex(g_mWatchFolderHourProcess);
	return 0;
}

void PeekMessages()
{
	MSG	msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		if ( msg.message == BENUBIRD_ASSISTANT_STOP) 
		{
			gTimersPaused = true;
			ReleaseWatchedFolders();
			TranslateMessage(&msg);
			DispatchMessage(&msg);  
		}
		if ( msg.message == BENUBIRD_ASSISTANT_START) 
		{
			GetWatchedFolders();
			gTimersPaused = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);  
		}

		//if (msg.message == WM_QUIT)
		//{
		//	PostThreadMessage(GetCurrentThreadId(), msg.message, msg.wParam, msg.lParam);
		//	return FALSE;
		//}
	}
}

void ProcessMessage_FromBenubird(int msg, int len,LPCTSTR strBuf)
{
	int mode;
	int state;
	CString strLib;

	CppSQLite3Query mQ; // for global watch library
	CString lSQL;

	switch (msg)
	{
	case BENUBIRD_ASSISTANT_LIBRARY_SETTING:
		{
			
			memcpy(&mode,strBuf,4);
			memcpy(&state,&strBuf[2],4);
			strLib = (LPCTSTR)(strBuf+4);
			if(!g_DB.SetLibrarySetting_For_Watch(strLib.GetBuffer(),mode,state))
			{
				AfxMessageBox(_T("Watch DB set Error"));
				ExitProcess(0);
			}
			break;
		}
	case BENUBIRD_ASSISTANT_GET_PREVOPTION_ASK:
		{	
			BYTE tmpBuf[0x1000];
			memset(tmpBuf,0,0x1000);

			mQ = g_DB.mDB.execQuery(_T("select * from librarys where State=1"));

			int len = 0;
			while (!mQ.eof())
			{
				CString strLib = mQ.getStringField(1);
				memcpy(&tmpBuf[len],strLib.GetBuffer(),strLib.GetLength()*2+1);
				len = len + (strLib.GetLength()+1)*2;
				mQ.nextRow();	

			}

			COPYDATASTRUCT cds;
			cds.dwData = BENUBIRD_ASSISTANT_GET_PREVOPTION_RESPONSE;
			cds.cbData = len;
			cds.lpData = tmpBuf;
			LRESULT ret = ::SendMessage((HWND)g_BenubirdWnd,WM_COPYDATA,0,(LPARAM)&cds);

			break;
		}
	}
}