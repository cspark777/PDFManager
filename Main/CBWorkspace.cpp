#include "stdafx.h"
#include "CBWorkspace.h"

#include "resource.h"
#include "CUtils.h"
#include "COptionsDefs.h" 
#include "MainFrm.h"

#include "CBRepository.h"

extern CString GUIDgen();


IMPLEMENT_DYNAMIC(CBWorkSpaceWnd, CWnd)

BEGIN_MESSAGE_MAP(CBWorkSpaceWnd, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CBWorkSpaceWnd::CBWorkSpaceWnd(CBWorkspace *pWorkspace)
{
	m_pParent = pWorkspace;
}

void CBWorkSpaceWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pParent->m_ControlThread == NULL)
	{
		m_pParent->m_SuspendThtread = false;
		m_pParent->m_ControlThread = AfxBeginThread(&CBWorkspace::WorkspaceCheckControllingFunction, (LPVOID)m_pParent);
	}

	CWnd::OnTimer(nIDEvent);
}


UINT __cdecl CBWorkspace::WorkspaceCheckControllingFunction( LPVOID pParam )
{

	CBWorkspace *pWorkspace = (CBWorkspace*)pParam;

	CIntArray lChangeFileList;


	if (!pWorkspace->m_SuspendThtread)
	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile(pWorkspace->m_strWorkspacePath + _T("*.*"));
		while (bWorking)
		{
			if (pWorkspace->m_SuspendThtread)
				break;

			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory() || finder.IsDots())
				continue;

		   CString lDirName = finder.GetFilePath();
		   CString lName = finder.GetFileName();

		   //remove the file ID from name
		   int lPos = lName.Find('_');
		   if (lPos < 0)	//bad folder probably
			   continue;

		   CString lStrID = lName.Left(lPos);

		   int lFileID = _ttoi(lStrID);
		   if (pWorkspace->CheckFile(lFileID) == 2)
			   lChangeFileList.Add(lFileID);

		}
	} 

	pWorkspace->m_ChangedFilesArr.RemoveAll();
	pWorkspace->m_ChangedFilesArr.Append(lChangeFileList);

	CMainFrame* lMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	lMainFrm->UpdateSysTrayIcon();

	if (pWorkspace->m_ShowMessage)
	{
		pWorkspace->m_ShowMessage = false;
		lMainFrm->ShowSysTrayIconMessage(_T("Benubird Workspace"), _T("There are unsaved files in the Benubird Workspace. You need to save these and restore to Repository."));
	}
	pWorkspace->m_ControlThread = NULL;
	AfxEndThread(0);
	return 0;
}


CBWorkspace::CBWorkspace()
{
	m_ControlThread = NULL;
	m_Wnd = NULL;
	m_ShowMessage = false;

}

void CBWorkspace::Set_ParamFromParent(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;

}

CBWorkspace::~CBWorkspace()
{
	if (m_ControlThread != NULL)
	{
		m_SuspendThtread = true;
		::WaitForSingleObject(m_ControlThread,INFINITE);
		m_ControlThread = NULL;
	}

	if (m_Wnd != NULL)
	{
		m_Wnd->DestroyWindow();
		delete m_Wnd;
		m_Wnd = NULL;
	}
}

bool CBWorkspace::Initialize(LPCTSTR path)
{
	m_strWorkspacePath = path;

	if (m_Wnd == NULL)
	{
		m_Wnd = new CBWorkSpaceWnd(this);
		m_Wnd->CreateEx(0, NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED, CRect(0,0,0,0), AfxGetMainWnd(), 0);

	}

	m_hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_WORKSPACE_NOTIFY),
			IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

	m_Wnd->SetTimer(0, 3000, NULL); 

	return true;
}
void CBWorkspace::SetWorkspacePath(LPCTSTR path)
{
	m_strWorkspacePath = path;
}
bool CBWorkspace::AddFile(int inFileID, bool inOpen)
{
	CTFiles lFiles(m_pParamCDB);
	if (!lFiles.GetFileByID(inFileID))
		return false;

	CString lFolder, lFileName;

	if (!GetWorkspaceFileFolder(inFileID, lFolder, lFileName))
		return false;

	CString lFilePath = lFolder + lFileName;

	if(FileIsInWorkspace(inFileID))
	{
		theApp.OpenDocumentFile_ForPDFView(lFilePath,m_pParamCDB->szLibPathName);
		::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)false, (LPARAM) inFileID);
		return true;
	}

	if (CheckFile(inFileID) != 2)
	{
		//save properties to file
		lFiles.UpdateFileOnDiskFromMembers();

		::SHCreateDirectoryEx(NULL, lFolder, NULL);
		::CopyFile(lFiles.mPath, lFilePath, false);
	//	SetFileAttributes(lFilePath, 0);

		if (CheckFile(inFileID) == 0)
			SaveCompareFile(lFilePath);
	}

	if (inOpen)
	{
		AfxMessageBox(lFilePath + " has been added to the workspace for editing", MB_OK | MB_ICONINFORMATION );
		theApp.OpenDocumentFile_ForPDFView(lFilePath,m_pParamCDB->szLibPathName);
	}

	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)false, (LPARAM) inFileID);

	return true;
}

bool CBWorkspace::AskForSaveToRepository(int inFileID)
{
	int lChange = CheckFile(inFileID);

	if (lChange == 2)
	{
		CTFiles lFiles(m_pParamCDB);
		lFiles.GetFileByID(inFileID);

		CString lTitle, lMsgFmt, lMsg;

		lTitle.LoadString(IDS_FILE_CHANGE_MSG_TITLE);
		lMsgFmt.LoadString(IDS_FILE_CHANGE_MSG);

		lMsg.Format(lMsgFmt, lFiles.mName);

		if (IDYES == MessageBox(AfxGetMainWnd()->GetSafeHwnd(), lMsg, lTitle, MB_YESNO|MB_ICONQUESTION))
		{
			if (SaveBackToRepository(inFileID) == false)
				return false;
		}
	}

	if (RemoveFile(inFileID) == false)
		return false;

	return true;
}


bool CBWorkspace::SaveBackToRepository(int inFileID)
{
	CTFiles lFiles(m_pParamCDB);
	if (!lFiles.GetFileByID(inFileID))
	{
		AfxMessageBox(_T("The file was not found in the Repository"));
		return false;
	}

	CString lStrFolder, lStrFileName;

	if (!GetWorkspaceFileFolder(inFileID, lStrFolder, lStrFileName))
	{
		AfxMessageBox(_T("Unable to resolve path to file"));
		return false;
	}

	if (!::CopyFile(lStrFolder + lStrFileName, lFiles.mPath, false))
	{
		AfxMessageBox(_T("Copy file failed"));
		return false;
	}

	UpdateFileProperties(inFileID, lStrFolder + lStrFileName);

	for (int i = 0; i < m_ChangedFilesArr.GetCount(); i++)
	{
		if (m_ChangedFilesArr.GetAt(i) == inFileID)
			m_ChangedFilesArr.RemoveAt(i);
	}

	SaveCompareFile(lStrFolder + lStrFileName);

	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)false, (LPARAM) inFileID);

	return true;
}


bool CBWorkspace::SaveCompareFile(CString& inFileName)
{
	//store check file
	WIN32_FIND_DATA lFindData;
	HANDLE lFileHandle = FindFirstFile(inFileName, &lFindData);
	if (lFileHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FindClose(lFileHandle);

	CString lPath = GetCompareFilePath(inFileName);

	CString lStrTimeLow, lStrTimeHigh;
	lStrTimeLow.Format(_T("%d"), lFindData.ftLastWriteTime.dwLowDateTime);
	lStrTimeHigh.Format(_T("%d"), lFindData.ftLastWriteTime.dwHighDateTime);

	FILE* f = _tfopen(lPath, _T("w+b"));
	if (f == NULL)
		return false;

	fwrite(&lFindData, sizeof(BYTE), sizeof(lFindData), f);
	fclose(f);

	SaveFilePropsFile(inFileName);

	return true;
}

bool CBWorkspace::SaveFilePropsFile(CString& inFileName)
{
	//store check file
	WIN32_FIND_DATA lFindData;
	HANDLE lFileHandle = FindFirstFile(inFileName, &lFindData);
	if (lFileHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FindClose(lFileHandle);


	CTFiles lFiles(m_pParamCDB);
	lFiles.mPath = inFileName;

	lFiles.UpdateFileFromDisk(false);

	CFileProperties_Entry lEntry;
	lEntry.mTitle = lFiles.mTitle;
	lEntry.mAuthor = lFiles.mAuthor;
	lEntry.mSubject = lFiles.mAuthor;
	lEntry.mComments = lFiles.mAuthor;
	lEntry.mTags = lFiles.mKeywords;
	lEntry.mCategories = lFiles.mCategory;

	CString lPath = GetPropertiesFilePath(inFileName);

	FILE* f = _tfopen(lPath, _T("w+b"));
	if (f == NULL)
		return false;

	fwrite(&lEntry, sizeof(BYTE), sizeof(CFileProperties_Entry), f);
	fclose(f);

	return false;
}

bool CBWorkspace::UpdateFileProperties(int inFileID, CString & inFileName)
{
	CString lPath = GetPropertiesFilePath(inFileName);

	//nacitam file properties zo suboru
	CTFiles lFiles(m_pParamCDB);
	lFiles.mPath = inFileName;
	lFiles.UpdateFileFromDisk(false);

	CTFiles lFiles2(m_pParamCDB);
	lFiles2.GetFileByID(inFileID);
	lFiles.UpdateFileFromDisk(false);

	//nacitam ulozene file properties
	CFileProperties_Entry lEntry;
	FILE* f = _tfopen(lPath, _T("w+b"));
	if (f != NULL)
	{
		fread(&lEntry, sizeof(BYTE), sizeof(CFileProperties_Entry), f);
		fclose(f);

		bool lChanged = (lEntry.mAuthor != lFiles.mAuthor || lEntry.mTitle != lFiles.mTitle || lEntry.mSubject != lFiles.mSubject ||
			lEntry.mCategories != lFiles.mCategory || lEntry.mComments != lFiles.mComments || lEntry.mTags != lFiles.mKeywords);

		if (lChanged)
		{
			lFiles2.mAuthor = lFiles.mAuthor;
			lFiles2.mTitle = lFiles.mTitle;
			lFiles2.mSubject = lFiles.mSubject;
			lFiles2.mCategory = lFiles.mCategory;
			lFiles2.mComments = lFiles.mComments;
			lFiles2.mKeywords = lFiles.mKeywords;
		}
	}

	lFiles2.UpdateTablebyID();

	return true;
}

int CBWorkspace::CompareFile(CString& inFileName)
{
	//function returns:
	//  0 - not changed
	//  1 - file was chaged after compare file was saved
	// -1 - file was chaged before compare file was saved (should not occur)

	int lResult = 0;

	//store check file
	WIN32_FIND_DATA lFindData, lFindDataOld;
	HANDLE lFileHandle = FindFirstFile(inFileName, &lFindData);
	if (lFileHandle == NULL)
		return 0;
	FindClose(lFileHandle);

	CString lPath = GetCompareFilePath(inFileName);

	FILE* f = _tfopen(lPath, _T("rb"));
	if (f == NULL)
	{
		SaveCompareFile(inFileName);
		return 0;
	}

	size_t lSz = fread(&lFindDataOld, 1, sizeof(WIN32_FIND_DATA), f);
	if (lSz == sizeof(lFindDataOld))
	{
		lResult = CompareFileTime(&lFindData.ftLastWriteTime, &lFindDataOld.ftLastWriteTime);
	}

	fclose(f);

	return lResult;
}

bool CBWorkspace::GetWorkspaceFileFolder(int inFileID, CString & outFolder, CString & outFileName)
{
	CTFiles lFiles(m_pParamCDB);
	if (!lFiles.GetFileByID(inFileID))
		return false;

	//create folder for file
	//outFolder.Format(_T("%s%06d_%s\\"), m_Folder, inFileID, lFiles.mName);
	outFolder = m_strWorkspacePath;
	outFileName = lFiles.mName;

	return true;

}

CString CBWorkspace::GetCompareFilePath(CString & inFileName)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lName[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(inFileName, lDrv, lFld, lName, lExt);

	TCHAR lPath[_MAX_PATH];
	_tmakepath(lPath, lDrv, lFld, lName, _T(".fdt"));

	return CString(lPath);
}

CString CBWorkspace::GetPropertiesFilePath(CString & inFileName)
{
	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lName[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];

	_tsplitpath(inFileName, lDrv, lFld, lName, lExt);

	TCHAR lPath[_MAX_PATH];
	_tmakepath(lPath, lDrv, lFld, lName, _T(".pdt"));

	return CString(lPath);
}


bool CBWorkspace::RemoveFile(int inFileID)
{
	CString lStrFolder, lStrFileName;
	if (GetWorkspaceFileFolder(inFileID, lStrFolder, lStrFileName) == false)
	{
		return false;
	}

	// Try to remove regardless... -- KDA 30 April, 2009
	if (FileIsInWorkspace(inFileID))
	{
		if (::DeleteFile(lStrFolder + lStrFileName) == FALSE)
		{
	//		if (GetLastError() == ERROR_ACCESS_DENIED)
			{
				AfxMessageBox(_T("Unable to remove file from workspace."));
				return false;
			}
		}
	}

	//remove file compare file
	CString lStrCmpFile = GetCompareFilePath(lStrFolder + lStrFileName);
	::DeleteFile(lStrCmpFile);

	//remove properties compare file
	CString lStrPropFile = GetPropertiesFilePath(lStrFolder + lStrFileName);
	::DeleteFile(lStrPropFile);

	::RemoveDirectory(lStrFolder);

	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)false, (LPARAM) inFileID);	
	return true;
}

int CBWorkspace::CheckFile(int inFileID)
{
	CString lFolder, lFileName;
	if (!GetWorkspaceFileFolder(inFileID, lFolder, lFileName))
		return 0;

	int lRet = CompareFile(lFolder + lFileName);

	if (lRet == 0) return 0;

	int lOpt = m_pParamCDB->nWorkspaceType;


	switch (lOpt)
	{
	case 0:		//keep original file
		return 0;
	case 1:		//auto save back to repository
		SaveBackToRepository(inFileID);
		return 1;
	case 2:		// no action on change
		SaveCompareFile(lFolder + lFileName);
		return 2;
	}

	for (int i = 0; i < m_ChangedFilesArr.GetCount(); i++)
	{
		if (m_ChangedFilesArr.GetAt( i ) == inFileID) 
			return 2;
	}

	m_ChangedFilesArr.Add(inFileID);

	m_ShowMessage = true;

//	CMainFrame* lFrame = (CMainFrame*)AfxGetMainWnd();
//	lFrame->ShowSysTrayIconMessage(_T("Title"), _T("Message"));
/*
	CBCGPPopupWindow* lPopupWnd = new CBCGPPopupWindow;

		lPopupWnd->SetAnimationType (CBCGPPopupMenu::SLIDE);
		lPopupWnd->SetAnimationSpeed (165);
		lPopupWnd->SetTransparency (165);
		lPopupWnd->SetSmallCaption (true);
		lPopupWnd->SetAutoCloseTime (3000);

		CBCGPPopupWndParams params;
		params.m_strText = _T("A file was changed");
		params.m_strURL = lFileName;
		params.m_nURLCmdID = 101;
//		params.m_hIcon = m_hIcon;

		lPopupWnd->Create (m_Wnd, params, NULL, CPoint(-1,-1));
*/

	return 2;
}

bool CBWorkspace::FileIsInWorkspace(int inFileID)
{
	CTFiles lFiles(m_pParamCDB);
	if (lFiles.GetFileByID(inFileID) == false)
		return false;

	CString lStrFolder, lStrFileName;
	if (!GetWorkspaceFileFolder(inFileID, lStrFolder, lStrFileName))
		return false;

	CFileFind lFinder;
	return (lFinder.FindFile(lStrFolder + lStrFileName) == TRUE);
}

bool CBWorkspace::FileIsModified(int inFileID)
{
	if (!FileIsInWorkspace(inFileID))
		return false;

	return CheckFile(inFileID) == 2;
}

