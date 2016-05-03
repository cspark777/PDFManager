#include "stdafx.h"
#include "COrganizer.h"
#include "Benubird.h"
#include "COptionsDefs.h"

#include "COptionsFileManagerDlg.h"
#include "CAlertDlg.h"
#include "CProgressDlg.h"
#include "CConfirmOverwriteDlg.h"
#include "CUtils.h"
#include "MainFrm.h"
#include "CLog.h"
#include "CRenameFile1Dlg.h"

COrganizer* gOrganizer = NULL;

bool COrganizer::Start()
{
	if (gOrganizer != NULL) return true;
	gOrganizer = new COrganizer;
	return (gOrganizer != NULL);
}

void COrganizer::End()
{
	if (gOrganizer != NULL)
	{
		delete gOrganizer;
		gOrganizer = NULL;
	}
}


COrganizer::COrganizer()
{
	Initialize();
}

COrganizer::~COrganizer()
{
}

bool COrganizer::Initialize()
{
	if (!GetBenubirdFolder(NULL, 0))
		return false;

	return true;
}

bool COrganizer::GetBenubirdFolder(LPTSTR fldBuf, int maxBuf, bool create)
{
	CWinApp* lApp = AfxGetApp();
	if (m_B_path.IsEmpty())
	{
		bool regFound = false;

		CString lStrFld = lApp->GetProfileString(kOptions, kFilesFolderLocation, _T(""));
		BOOL lCheck = lApp->GetProfileInt(kOptions, kCopyFilesToFolder, BST_UNCHECKED);

		m_B_path = lStrFld;

		if (m_B_path.GetLength() == 0)
		{
			TCHAR lPath[MAX_PATH+1];
			memset(lPath, 0, sizeof(lPath));

			if (::SHGetFolderPath(NULL, CSIDL_PERSONAL | (create == true ? CSIDL_FLAG_CREATE : 0), NULL, SHGFP_TYPE_CURRENT, lPath) != S_OK)
				m_B_path = _T("C:");
			else
				m_B_path = lPath;

			m_B_path += _T("\\Benubird library");
		}

		CFileFind lFinder;
		if (!lFinder.FindFile(m_B_path))
		{
			//just for sure... create the directory
			if (::SHCreateDirectoryEx(NULL, m_B_path, NULL) != ERROR_SUCCESS)
			{
			}
		}

		if (m_B_path.Right(1) != _T("\\"))
			m_B_path.AppendChar('\\');

		_tsplitpath(m_B_path, m_B_drv, m_B_fld, NULL, NULL);
	}

	if (fldBuf != NULL)
		_tcsncpy(fldBuf, m_B_path, maxBuf);

	return true;
}

int COrganizer::OrganizeFiles(CStringList & inFilePaths, CTWatchedFolder_Entry * inFolderEntry, CIntArray* outAddedFilesArray,  bool inUseProgMon)
{
	CTWatchedFolder_Entry lFolderEntry;
	if (inFolderEntry)
		lFolderEntry = *inFolderEntry;
	lFolderEntry.mFileOperation = CTWatchedFolder_Entry::foLink;

	int lSameNameAction = kConfirmUnknown;
	bool lApplyForAll = false;

	int lNumAddedFiles = 0;
	if (outAddedFilesArray != NULL)
		outAddedFilesArray->RemoveAll();

	POSITION lPos = inFilePaths.GetHeadPosition();
	if (lPos == NULL)
		return 0;

	CProgressDlg* lProgressDlg = NULL;

	CStringList lNewFilePaths;

	if (inUseProgMon)
	{
		lProgressDlg = new CProgressDlg;
		lProgressDlg->SetMax((int)inFilePaths.GetCount());
		lProgressDlg->SetCaption(_T("Preprocessing files..."));
		lProgressDlg->CreateProgressDlg();
	}

	CMainFrame* lMainFrame = (CMainFrame*)AfxGetMainWnd();

	while (lPos)
	{
		if (!lApplyForAll)
			lSameNameAction = kConfirmUnknown;

		POSITION lOldPos = lPos;
		CString lFilePath = inFilePaths.GetNext(lPos);

		if (!IsAddThisFileEnabled(lFilePath))
			continue;

		CString lStrProgress;
		lStrProgress.Format(_T("Preprocessing files... %s"), lFilePath);

		if (lProgressDlg)
		{
			lProgressDlg->SetMessage(lStrProgress);
			lProgressDlg->StepIt();
			if (lProgressDlg->IsCancelled())
				break;
		}

		//check if the file is already in the B-folder
		if (IsInOrganizedFolder(lFilePath))
		{
			lNewFilePaths.AddTail(lFilePath);
			continue;
		}

		//copy file into the B-folder
		//create new file path in B-Folder

		TCHAR lDrv[_MAX_DRIVE];
		TCHAR lDir[_MAX_DIR];
		TCHAR lNam[_MAX_FNAME];
		TCHAR lExt[_MAX_EXT];

		if (_tsplitpath_s(lFilePath, lDrv, lDir, lNam, lExt) != 0)
		{
			//error spliting file path
			//write file path into error list and log file
		}

		TCHAR lNewFilePath[_MAX_PATH];

		CFileFind lFinder;
		int lNumTry = 0;

		while (true)
		{
			CString lFName = lNam;

			if (lSameNameAction == kConfirmAutoRename && lNumTry > 0)
				lFName.Format(_T("%s(%d)"), lNam, lNumTry);
			else if (lSameNameAction == kConfirmRename)
			{
				CRenameFile1Dlg lDlg(lFName);
				if (lDlg.DoModal())
				{
					lFName = lDlg.mFileName;
				}
			}

			_tmakepath_s(lNewFilePath, m_B_drv, m_B_fld, lFName, lExt);

			if (lFinder.FindFile(lNewFilePath) == FALSE)
			{
				lSameNameAction = kConfirmOverwrite;
				break;
			}
			if (lSameNameAction == kConfirmUnknown)
			{
				CConfirmOverwriteDlg lDlg(lNewFilePath, lFilePath);
				lDlg.DoModal();

				lSameNameAction = lDlg.GetResultAction(lApplyForAll);
			}

			if (lSameNameAction == kConfirmAutoRename)
				lNumTry++;
			else if (lSameNameAction == kConfirmCancel || lSameNameAction == kConfirmOverwrite || 
				lSameNameAction == kConfirmOverwriteOlder || lSameNameAction == kConfirmSkip)
				break;
		}

		//stop the process if canceled
		if (lSameNameAction == kConfirmCancel)
			break;
		if (lSameNameAction == kConfirmSkip)
			continue;


		if ( lSameNameAction == kConfirmOverwrite || lSameNameAction == kConfirmOverwriteOlder )
		{

			bool lCopyFile = true;

			while (lFinder.FindFile(lNewFilePath) == TRUE)
			{
				lCopyFile = true;
				if (DeleteFile(lNewFilePath) == FALSE)
				{
					lCopyFile = false;
					CString lErrStr;
					lErrStr.Format(_T("Delete file failed\n%s\n%s"), lNewFilePath, CUtils::GetLastErrorString());
					CLog::StoreUserMessage(lErrStr);

					//error cpying file
					//write file into error list and log file

					int lRet = Alert(kAlertQuestion, lErrStr, _T("Retry"), _T("Skip file"));
					if (!lRet)
						break;
				}
				else 
					break;
			}

			while (lCopyFile)
			{
				if (CopyFile(lFilePath, lNewFilePath, TRUE) == FALSE)
				{
					CString lErrStr;			
					lErrStr.Format(_T("Copy file failed \n%s -> %s \n %s"), lFilePath, lNewFilePath, CUtils::GetLastErrorString());
					CLog::StoreUserMessage(lErrStr);

					//error cpying file
					//write file into error list and log file

					int lRet = Alert(kAlertQuestion, lErrStr, _T("Retry"), _T("Skip file"));

					if (lRet)
						break;
				}
				else
				{
					lNewFilePaths.AddTail(lNewFilePath);
					break;
				}
			}
		}
		else
		{
			lNewFilePaths.AddTail(lNewFilePath);
		}
	}

	if (lProgressDlg != NULL)
	{
		delete lProgressDlg;
		lProgressDlg = NULL;
	}

	CTFiles lFiles;
	lNumAddedFiles = lFiles.AddFiles(lNewFilePaths, &lFolderEntry, outAddedFilesArray);

	CWinApp* lWinApp = AfxGetApp();

	//delete original files from disk:

	bool lDelete = (inFolderEntry && inFolderEntry->mFileOperation == CTWatchedFolder_Entry::foMove);
	if (!lDelete && lWinApp->GetProfileInt(kOptions, kActionAfterCopy, 0) == 0)
	{
		//ask user to delete file
		if (AfxMessageBox(_T("Do you want to delete original files?"), MB_YESNO ) == IDYES)
			lDelete = true;
	}

	if (lDelete)
	{
		POSITION lPos = inFilePaths.GetHeadPosition();

		while (lPos)
		{
			CString lFilePath = inFilePaths.GetNext(lPos);
			::DeleteFile(lFilePath);
		}
	}

	return lNumAddedFiles;
}


bool COrganizer::IsInOrganizedFolder(CString inFilePath)
{
	TCHAR inDrv[_MAX_DRIVE];
	TCHAR inFld[_MAX_DIR];
	_tsplitpath(inFilePath, inDrv, inFld, NULL, NULL);

	bool lIsIn = false;

	if ( _tcsnicmp(inDrv, m_B_drv, _tcslen(m_B_drv)) == 0 && _tcsnicmp(inFld, m_B_fld, _tcslen(m_B_fld)) == 0 )
		lIsIn = true;

	return lIsIn;
}


