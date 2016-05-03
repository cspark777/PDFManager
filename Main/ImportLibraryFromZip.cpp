// ImportLibraryFromZip.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "ImportLibraryFromZip.h"
#include "afxdialogex.h"
#include "FolderDlg.h"

// CImportLibraryFromZip dialog

IMPLEMENT_DYNAMIC(CImportLibraryFromZip, CDialogEx)

CImportLibraryFromZip::CImportLibraryFromZip(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportLibraryFromZip::IDD, pParent)
{

}

CImportLibraryFromZip::~CImportLibraryFromZip()
{
}

BOOL CImportLibraryFromZip::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString lStrFilter;
	lStrFilter.LoadString(IDS_ZIP_FILES_FILTER);

	m_txtZipFilePath.EnableFileBrowseButton(_T(".zip"), lStrFilter);

	m_txtPassword.EnableWindow(0);

	return TRUE;
}

void CImportLibraryFromZip::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PASSWORD, m_chkIsPassword);
	DDX_Control(pDX, IDC_EDIT_DEST_DIRECTORY, m_txtDestDirectory);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_txtZipFilePath);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_txtPassword);
	DDX_Control(pDX, IDC_BUTTON_BROWSER, m_btnBrowser);
}


BEGIN_MESSAGE_MAP(CImportLibraryFromZip, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_PASSWORD, &CImportLibraryFromZip::OnBnClickedCheckPassword)
	ON_BN_CLICKED(IDOK, &CImportLibraryFromZip::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CImportLibraryFromZip::OnBnClickedButtonBrowser)
END_MESSAGE_MAP()


// CImportLibraryFromZip message handlers


void CImportLibraryFromZip::OnBnClickedCheckPassword()
{
	// TODO: Add your control notification handler code here
	if(m_chkIsPassword.GetCheck()==1)
	{
		m_txtPassword.EnableWindow(1);
	}
	else
	{
		m_txtPassword.EnableWindow(0);
	}
}


void CImportLibraryFromZip::OnBnClickedOk()
{
	m_txtDestDirectory.GetWindowText(m_StrDestDirectory);

	if(m_StrDestDirectory.IsEmpty())
	{
		MessageBox(_T("Invalid dest directory"));
		m_txtDestDirectory.SetFocus();
		return;
	}

	m_txtZipFilePath.GetWindowTextW(m_StrZipFilePath);
	CFileFind lFinder;
	if (m_StrZipFilePath.IsEmpty() || lFinder.FindFile(m_StrZipFilePath) == FALSE)
	{
		MessageBox(_T("Invalid archive file path"));
		m_txtZipFilePath.SetFocus();
		return;
	}

	m_StrPassword.Empty();

	if (m_chkIsPassword.GetCheck() != 0)
		m_txtPassword.GetWindowText(m_StrPassword);

	OnOK();

}


void CImportLibraryFromZip::OnBnClickedButtonBrowser()
{
	LPCTSTR lpszTitle = _T( "Select the root folder for the browse dialog:" );
	UINT	uFlags	  = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	
	CFolderDialog dlgRoot( lpszTitle, m_StrDestDirectory, this, uFlags );
		
	if( dlgRoot.DoModal() == IDOK )
	{
		m_StrDestDirectory = dlgRoot.GetFolderPath();		
		m_txtDestDirectory.SetWindowTextW(m_StrDestDirectory);
	}
}
