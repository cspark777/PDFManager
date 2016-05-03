// CConfirmOverwriteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CConfirmOverwriteDlg.h"
//#include "COrganizer.h"


// CConfirmOverwriteDlg dialog

IMPLEMENT_DYNAMIC(CConfirmOverwriteDlg, CDialog)

CConfirmOverwriteDlg::CConfirmOverwriteDlg(LPCTSTR inFilePath, LPCTSTR inOverwritePath, CWnd* pParent /*=NULL*/)
	: CDialog(CConfirmOverwriteDlg::IDD, pParent)
{
	m_FilePath = inFilePath;
	m_OverwritePath = inOverwritePath;

	m_ApplyToAll = false;
	m_ResultAction = kConfirmCancel;
}

CConfirmOverwriteDlg::~CConfirmOverwriteDlg()
{
}

void CConfirmOverwriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfirmOverwriteDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, &CConfirmOverwriteDlg::OnBnClickedButtonReplace)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_OLDER, &CConfirmOverwriteDlg::OnBnClickedButtonReplaceOlder)
	ON_BN_CLICKED(IDC_BUTTON_SKIP, &CConfirmOverwriteDlg::OnBnClickedButtonSkip)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, &CConfirmOverwriteDlg::OnBnClickedButtonRename)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_RENAME, &CConfirmOverwriteDlg::OnBnClickedButtonAutoRename)
	ON_BN_CLICKED(IDCANCEL, &CConfirmOverwriteDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_APPLY_TO_ALL, &CConfirmOverwriteDlg::OnBnClickedCheckApplyToAll)
END_MESSAGE_MAP()


BOOL CConfirmOverwriteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//layout buttons
	CWnd * lItem;
	CRect lRect, lDlgRect;

	GetWindowRect(&lDlgRect);

	lItem = GetDlgItem(IDC_BUTTON_REPLACE);
	lItem->GetWindowRect(&lRect);
	ScreenToClient(&lRect);

	int lGap = 10;
	int lLeft = lRect.left;
	int lWidth = (lDlgRect.Width() - 2*lLeft - 2*lGap)/3 -1 ;
	

	int lLeftPos = lLeft;

	lRect.right = lLeft + lWidth;
	lItem->MoveWindow(&lRect);
	lLeftPos += (lWidth + lGap);

	lItem = GetDlgItem(IDC_BUTTON_REPLACE_OLDER);
	lItem->GetWindowRect(&lRect);
	ScreenToClient(&lRect);
	lRect.left = lLeftPos;
	lRect.right = lRect.left + lWidth;
	lItem->MoveWindow(&lRect);
	lLeftPos += (lWidth + lGap);

	lItem = GetDlgItem(IDC_BUTTON_SKIP);
	lItem->GetWindowRect(&lRect);
	ScreenToClient(&lRect);
	lRect.left = lLeftPos;
	lRect.right = lRect.left + lWidth;
	lItem->MoveWindow(&lRect);
	
	lLeftPos = lLeft;

	lItem = GetDlgItem(IDC_BUTTON_RENAME);
	lItem->GetWindowRect(&lRect);
	ScreenToClient(&lRect);
	lRect.left = lLeftPos;
	lRect.right = lRect.left + lWidth;
	lItem->MoveWindow(&lRect);
	lLeftPos += (lWidth + lGap);

	lItem = GetDlgItem(IDC_BUTTON_AUTO_RENAME);
	lItem->GetWindowRect(&lRect);
	ScreenToClient(&lRect);
	lRect.left = lLeftPos;
	lRect.right = lRect.left + lWidth;
	lItem->MoveWindow(&lRect);
	lLeftPos += (lWidth + lGap);

	lItem = GetDlgItem(IDCANCEL);
	lItem->GetWindowRect(&lRect);
	ScreenToClient(&lRect);
	lRect.left = lLeftPos;
	lRect.right = lRect.left + lWidth;
	lItem->MoveWindow(&lRect);

	//read the file info
	SetDlgItemText(IDC_STATIC_FILENAME2, m_FilePath);
	ReadFileInfo(false);

	SetDlgItemText(IDC_STATIC_FILENAME1, m_OverwritePath);
	ReadFileInfo(true);

	return TRUE;
}

void CConfirmOverwriteDlg::ReadFileInfo(bool destFile)
{
	CString lPath = destFile ? m_OverwritePath : m_FilePath;

	CString lInfo;
	HANDLE f = CreateFile(lPath, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f) 
	{
		CString lSzInfo, lTmInfo;

		LARGE_INTEGER sz;
		BOOL lRet = GetFileSizeEx(f, &sz);
		lSzInfo.Format(_T("Size: %d bytes, "), sz.LowPart);

		lInfo += lSzInfo;

		FILETIME ftCreate, ftAccess, ftWrite;

		GetFileTime(f, &ftCreate, &ftAccess, &ftWrite);

		SYSTEMTIME stUTC, stLocal;

		// Convert the last-write time to local time.
		FileTimeToSystemTime(&ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

		lTmInfo.Format(_T("Last modified: %02d/%02d/%d  %02d:%02d:%02d"), stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
		lInfo += lTmInfo;
		CloseHandle(f);
	}

	SetDlgItemText(destFile ? IDC_STATIC_FILE_DETAILS1 : IDC_STATIC_FILE_DETAILS2, lInfo);
}

// CConfirmOverwriteDlg message handlers


int CConfirmOverwriteDlg::GetResultAction(bool & outApplyToAll)
{
	outApplyToAll = m_ApplyToAll;
	return m_ResultAction;
}
void CConfirmOverwriteDlg::OnBnClickedButtonReplace()
{
	m_ResultAction = kConfirmOverwrite;
	OnOK();
}

void CConfirmOverwriteDlg::OnBnClickedButtonReplaceOlder()
{
	m_ResultAction = kConfirmOverwriteOlder;
	OnOK();
}

void CConfirmOverwriteDlg::OnBnClickedButtonSkip()
{
	m_ResultAction = kConfirmSkip;
	OnOK();
}

void CConfirmOverwriteDlg::OnBnClickedButtonRename()
{
	m_ResultAction = kConfirmRename;
	OnOK();
}

void CConfirmOverwriteDlg::OnBnClickedButtonAutoRename()
{
	m_ResultAction = kConfirmAutoRename;
	OnOK();
}

void CConfirmOverwriteDlg::OnBnClickedCancel()
{
	m_ResultAction = kConfirmCancel;
	OnCancel();
}

void CConfirmOverwriteDlg::OnBnClickedCheckApplyToAll()
{
	m_ApplyToAll = (IsDlgButtonChecked(IDC_CHECK_APPLY_TO_ALL) != 0);
}
