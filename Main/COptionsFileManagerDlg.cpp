// COptionsFileManagerDlg.cpp
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "LibraryDoc.h"
#include "COptionsFileManagerDlg.h"
#include "CBrowseFolderDlg.h"
#include "COptionsDefs.h"
#include "CProgressDlg.h"

#include "CAlertDlg.h"
#include "CBRepository.h"
#include "CUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(COptionsFileManagerDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsFileManagerDlg, CDialog)
END_MESSAGE_MAP()
//*****************************************************************************
COptionsFileManagerDlg::COptionsFileManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsFileManagerDlg::IDD, pParent)
{
	m_strRepositoryPath = _T("");
	m_strWorkspacePath = _T("");
}
COptionsFileManagerDlg::COptionsFileManagerDlg(PARAM_FOR_CDB *pParamCDB,CWnd* pParent /*=NULL*/)
	: CDialog(COptionsFileManagerDlg::IDD, pParent)
{
	m_pParamCDB = pParamCDB;
	m_pDB = m_pParamCDB->pDB;
	m_strRepositoryPath = _T("");
	m_strWorkspacePath = _T("");
}
//*****************************************************************************
COptionsFileManagerDlg::~COptionsFileManagerDlg()
{
}
//*****************************************************************************
void COptionsFileManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_REPOSITORY_LOCATION_EDIT, m_RepositoryLocationEdit);
	DDX_Control(pDX, IDC_OPT_ON_ADD_ACTION_COMBO, m_OnAddActionCombo);
	DDX_Control(pDX, IDC_OPT_WORKSPACE_LOCATION_EDIT, m_WorkspaceLocationEdit);
	DDX_Control(pDX, IDC_OPT_ON_CHANGE_ACTION_COMBO, m_OnChangeActionCombo);
	
}

//*****************************************************************************
BOOL COptionsFileManagerDlg::ValidateAndProcess()
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	CWinApp* lApp = AfxGetApp();

	TCHAR lDrv[_MAX_DRIVE];
	TCHAR lFld[_MAX_DIR];
	TCHAR lNam[_MAX_FNAME];
	TCHAR lExt[_MAX_EXT];
	
// Check if the selected repository folder exists
// ---------------------------------------------------------
	CString lStrRepositoryPath;
	m_RepositoryLocationEdit.GetWindowText(lStrRepositoryPath);

	//check if the path is valid
	if (_wsplitpath_s(lStrRepositoryPath, lDrv, lFld, lNam, lExt) != 0 || lDrv[0] == '\0')
	{
		Alert(kAlertWarning, _T("Invalid folder"), _T("OK"));
		m_RepositoryLocationEdit.SetFocus();
		return FALSE;
	}
	CFileFind lRepositoryFinder;
	if (!lRepositoryFinder.FindFile(lStrRepositoryPath + _T("\\*.*")))
	{
		CWnd lMessage;
		if (lMessage.MessageBox(_T("The Repository folder does not exist, do you wish\nto create it?"), _T("Create Repository folder"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			m_RepositoryLocationEdit.SetFocus();
			return FALSE;
		}

		//try to create the directory
		if (::SHCreateDirectoryEx(NULL, lStrRepositoryPath, NULL) != ERROR_SUCCESS)
		{
			Alert(kAlertError, _T("Benubird was unable to create the library folder.\nPlease contact support@debenu.com"), _T("OK"));
			m_RepositoryLocationEdit.SetFocus();
			return FALSE;
		}
	}

// ---------------------------------------------------------
// Check if the selected workspace folder exists
	CString lStrWorkspacePath;
	m_WorkspaceLocationEdit.GetWindowText(lStrWorkspacePath);

	if (_wsplitpath_s(lStrWorkspacePath, lDrv, lFld, lNam, lExt) != 0 || lDrv[0] == '\0')
	{
		Alert(kAlertWarning, _T("Invalid folder"), _T("OK"));
		m_WorkspaceLocationEdit.SetFocus();
		return FALSE;
	}

	CFileFind lWorkspaceFinder;
	if (!lWorkspaceFinder.FindFile(lStrWorkspacePath + _T("\\*.*")))
	{
		CWnd lMessage;
		if (lMessage.MessageBox(_T("The workspace folder does not exist, do you wish\nto create it?"), _T("Create workspace folder"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			m_WorkspaceLocationEdit.SetFocus();
			return FALSE;
		}

		//try to create the directory
		if (::SHCreateDirectoryEx(NULL, lStrWorkspacePath, NULL) != ERROR_SUCCESS)
		{
			Alert(kAlertError, _T("Benubird was unable to create the workspace folder.\nPlease contact support@debenu.com"), _T("OK"));
			m_WorkspaceLocationEdit.SetFocus();
			return FALSE;
		}
	}

	// ---------------------------------------------------------
	// If the repository path has been updated then 
	// move the files to the a new location, and update the database
	// ---------------------------------------------------------
	if(m_strRepositoryPath != lStrRepositoryPath)
	{
		// -------------------------------------- Check for permission
		CWnd lMessage;
		bool bDeleteFiles = false;
		if (lMessage.MessageBox(_T("Do you wish to delete old files after transferring?"), _T("Move Repository"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			bDeleteFiles = true;
		}
		// -------------------------------------- 
 
		try
		{
		// Count the number of files in the database
			int iNumFiles = (int) pDoc->sFilesArray.GetCount();
//			CUtils::ClearCache(0,iNumFiles-1);

			if(iNumFiles>0)
			{
			// Setup progress bar
				CProgressDlg* lProgressDlg = NULL;
				lProgressDlg = new CProgressDlg;
				lProgressDlg->SetMax(iNumFiles);
				lProgressDlg->SetCaption(_T("Moving files..."));
				lProgressDlg->CreateProgressDlg();

			// Iterate through each row, transfer the file to the new repository 
			// and update the database
				for(int iFileId =0; iFileId<iNumFiles; iFileId++)
				{
				// Update the cache to extract the next row
//					CUtils::CTFilesArrayNeedRead(iFileId, iFileId);
//					CUtils::PrepareCache(iFileId, iFileId, false);

				// Get the full path of the file
					CString strFileAndFolder = pDoc->sFilesArray[iFileId].mPath;

				// Get the filename
					CString strFile = strFileAndFolder.Right(strFileAndFolder.GetLength() - strFileAndFolder.ReverseFind('\\')-1);

				// Get the full folder name (including drive, folder, sub-folders, etc...)
					CString strFolder = strFileAndFolder.Left(strFileAndFolder.ReverseFind('\\')+1);

				// Get the subfolder name
					CString strTemp = strFolder.Left(strFolder.ReverseFind('\\'));
					CString strSubFolder = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind('\\')-1);

				// Display updated progress bar
					CString strNewFolder = lStrRepositoryPath + CString(_T("\\")) + strSubFolder;
					CString strProgressBarMsg = _T("Moving file ") + strFile + _T(" to ") + strNewFolder;

				// Try to create the directory, just in case...
					int iCreateDirectory = ::SHCreateDirectoryEx(NULL, strNewFolder, NULL);
					if ((iCreateDirectory != ERROR_SUCCESS) && (iCreateDirectory != ERROR_ALREADY_EXISTS) && (iCreateDirectory != ERROR_FILE_EXISTS))
					{
						Alert(kAlertError, _T("Benubird was unable to create the Repository folder.\nPlease contact support@debenu.com"), _T("OK"));
						break;
					}

				// If the file moves successfully then update the database
					CString strNewPath = strNewFolder + _T('\\') + strFile;
					//if(MoveFile(CUtils::sFilesArray[iFileId].mPath, strNewPath)==TRUE)
					if(CopyFile(pDoc->sFilesArray[iFileId].mPath, strNewPath, false)==TRUE)
					{
					// Update the file name
						try
						{
							CString lSQL;
							lSQL.Format(_T("update Files set Path = '%s' where idFile = %u;"), strNewPath, pDoc->sFilesArray[iFileId].mID);
							m_pDB->mDB.execDML(lSQL);
						}
						catch(...)
						{
							strProgressBarMsg = "Benubird was unable to updating the database.\nPlease contact support@debenu.com";
						}
						if(bDeleteFiles) // Only delete old files if requested
							DeleteFile(pDoc->sFilesArray[iFileId].mPath); // Delete old file if possible
					}
					else
					{
						strProgressBarMsg = "Benubird was unable to move your files.\nPlease contact support@debenu.com";
					}

				// Update the progress bar
					lProgressDlg->SetMessage(strProgressBarMsg);
					lProgressDlg->StepIt();
					if (lProgressDlg->IsCancelled())
					{
						break;
					}
				}

			// Close progress bar
				lProgressDlg->DestroyWindow();
				delete lProgressDlg; 
			}
		}
		catch(...)
		{
		}
	}

// Hmmm... if the workspace path has been updated
// do nothing right now..............
	if(	m_strWorkspacePath != lStrWorkspacePath)
	{
//		AfxMessageBox(_T("Workspace moved"));
	}

	//int lCopyAction = lCopyFile ? m_OnAddActionCombo.GetCurSel() : 0;
	//lApp->WriteProfileInt(kOptions, kActionAfterCopy, lCopyAction);

// Write new repository + workspace to the registry
	lApp->WriteProfileString(kOptions, kRepositoryFolder, lStrRepositoryPath);
	lApp->WriteProfileString(kOptions, kWorkspaceFolder, lStrWorkspacePath);

	int nCurSel = m_OnChangeActionCombo.GetCurSel();
	lApp->WriteProfileInt(kOptions, kWorkspaceActionOnChange, nCurSel);

	return TRUE;
}
//*****************************************************************************
BOOL COptionsFileManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//case 381
	m_RepositoryLocationEdit.EnableFolderBrowseButton();
	// m_WorkspaceLocationEdit.EnableFolderBrowseButton(); DISABLE BROWSE BUTTON - KDA 01/04/2009

	CWinApp* lApp = AfxGetApp();

	CString lStrPathRep = lApp->GetProfileString(kOptions, kRepositoryFolder, _T(""));
	if (lStrPathRep.IsEmpty())
	{
		CBRepository::GetUserPath(CBRepository::kPathRepository, lStrPathRep, false);
	}
	m_RepositoryLocationEdit.SetWindowText(lStrPathRep);
	
	//order of these items is important!!!
	//see accAsk, accKeep, accDelete in the header
	m_OnAddActionCombo.AddString(_T("Prompt to delete the original file after adding to Repository"));
	m_OnAddActionCombo.AddString(_T("Keep original file after adding to Repository"));
	m_OnAddActionCombo.AddString(_T("Delete original file after adding to Repository"));

	int lCopyActionRep = lApp->GetProfileInt(kOptions, kRepositoryActionOnAdd, 0);
	m_OnAddActionCombo.SetCurSel(lCopyActionRep);


	CString lStrPathWksp = lApp->GetProfileString(kOptions, kWorkspaceFolder, _T(""));
	if (lStrPathWksp.IsEmpty())
	{
		CBRepository::GetUserPath(CBRepository::kPathWorkspace, lStrPathWksp, false);
	}
	m_WorkspaceLocationEdit.SetWindowText(lStrPathWksp);


// Keep a copy of the current repository and workspace paths
// -----------------------------------------------------------
	m_strRepositoryPath = lStrPathRep;
	m_strWorkspacePath = lStrPathWksp;
// -----------------------------------------------------------

	m_OnChangeActionCombo.AddString(_T("Prompt to update the Repository when a file is changed"));
	m_OnChangeActionCombo.AddString(_T("Update the file in Repository when a file is changed"));
	m_OnChangeActionCombo.AddString(_T("Don't update the file in Repository when a file is changed"));

	int lCopyActionWksp = lApp->GetProfileInt(kOptions, kWorkspaceActionOnChange, 0);
	m_OnChangeActionCombo.SetCurSel(lCopyActionWksp);

	return TRUE;
}
