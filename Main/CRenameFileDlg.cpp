#include "stdafx.h"
#include "Benubird.h"
#include "CRenameFileDlg.h"
#include "CAlertDlg.h"
#include "CUtils.h"
#include "CLog.h"
#include "LibraryView.h"

///////////////////////////////////////////////////////////////////////////////////
// CRenameFileDlg dialog
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CRenameFileDlg, CDialog)

CRenameFileDlg::CRenameFileDlg(CString inFileName, int inFileID)
	: CDialog(CRenameFileDlg::IDD, NULL)
{
	mFileName = inFileName;
	mFileID = inFileID;

	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	m_pParamCDB = pDoc->m_pParamCDB;
}

CRenameFileDlg::~CRenameFileDlg()
{
}

void CRenameFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRenameFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CRenameFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// get only the Name from the path and put it into the edit
	SetDlgItemText(IDC_FILENAME_EDIT, mFileName);
	
	return true;
}

void CRenameFileDlg::OnBnClickedOk()
{
	// change the file name (write changes to disc and update also data in db)
	
	CString lFileName;
	GetDlgItemText(IDC_FILENAME_EDIT, lFileName);

	if (lFileName.IsEmpty())
	{
		AfxMessageBox(_T("The file name cannot be empty"), MB_ICONSTOP | MB_OK);
		GetDlgItem(IDC_FILENAME_EDIT)->SetFocus();
		return;
	}

	// update the file name
	try
	{
		CString lSQL;
		lSQL.Format(_T("update Files set Name = '%s' where idFile = %u;"), lFileName, mFileID);
		m_pDB->mDB.execDML(lSQL);
	}
	catch(...)
	{
	}

	// update the item in the grid

	::PostMessage(HWND_BROADCAST, BENUBIRD_ASSISTANT_REFRESH_FILE, (WPARAM)false, (LPARAM) mFileID);

	OnOK();
}
