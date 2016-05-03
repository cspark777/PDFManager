// LibrarySettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "LibrarySettingDlg.h"
#include "afxdialogex.h"
#include "FolderDlg.h"

// CLibrarySettingDlg dialog

IMPLEMENT_DYNAMIC(CLibrarySettingDlg, CDialogEx)

CLibrarySettingDlg::CLibrarySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLibrarySettingDlg::IDD, pParent)
{

}

CLibrarySettingDlg::~CLibrarySettingDlg()
{
}

void CLibrarySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_LIBRARYSETTING_SAVE, m_btnDirectory);
	DDX_Control(pDX, IDC_CHECK_LIBRARYSETTING_USEREPOSITORY, m_chkRepository);
	DDX_Control(pDX, IDC_EDIT_LIBRARYSETTING_REPOSITORY, m_txtRepositoryPath);
	DDX_Control(pDX, IDC_EDIT_LIBRARYSETTING_WORKSPACE, m_txtWorkspacePath);
	DDX_Control(pDX, IDC_COMBO_WORKSPACE_TYPE, m_cmbWorkspaceType);
	DDX_Control(pDX, IDC_CHECK_IS_DEFAULT, m_chkDefault);
}


BEGIN_MESSAGE_MAP(CLibrarySettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LIBRARYSETTING_SAVE, &CLibrarySettingDlg::OnBnClickedButtonLibrarysettingSave)
	ON_BN_CLICKED(IDC_CHECK_LIBRARYSETTING_USEREPOSITORY, &CLibrarySettingDlg::OnBnClickedCheckLibrarysettingUserepository)
	ON_BN_CLICKED(IDOK, &CLibrarySettingDlg::OnBnClickedOk)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CLibrarySettingDlg message handlers


void CLibrarySettingDlg::OnBnClickedButtonLibrarysettingSave()
{
	LPCTSTR lpszTitle = _T( "Select the root folder for the browse dialog:" );
	UINT	uFlags	  = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	
	CFolderDialog dlgRoot( lpszTitle, m_StrRepositoryPath, this, uFlags );
		
	if( dlgRoot.DoModal() == IDOK )
	{
		m_StrRepositoryPath = dlgRoot.GetFolderPath();
		m_StrRepositoryPath = m_StrRepositoryPath + _T("\\");		
		m_txtRepositoryPath.SetWindowTextW(m_StrRepositoryPath);
	}
}

void CLibrarySettingDlg::SetInitVarible(LIBRARY_SETTING_INFO info)
{
	m_nIsUseRepository = info.nIsUseRepository;
	m_StrRepositoryPath = info.strRepositoryPath;
	m_StrWorkspacePath = info.strWorkspacePath;
	m_nWorkspaceType = info.nWorkspaceType;

	m_nIsDefaultLibrary = info.nIsDefaultLibrary;
}

void CLibrarySettingDlg::OnBnClickedCheckLibrarysettingUserepository()
{
	// TODO: Add your control notification handler code here
	if(m_chkRepository.GetCheck()==1)
	{
		m_nIsUseRepository = 1;
		m_txtRepositoryPath.EnableWindow(1);
		m_btnDirectory.EnableWindow(1);
	}
	else
	{
		m_nIsUseRepository = 0;
		m_txtRepositoryPath.EnableWindow(0);
		m_btnDirectory.EnableWindow(0);
	}
}


void CLibrarySettingDlg::OnBnClickedOk()
{
	m_txtRepositoryPath.GetWindowText(m_StrRepositoryPath);
	m_txtWorkspacePath.GetWindowTextW(m_StrWorkspacePath);
	m_nWorkspaceType = m_cmbWorkspaceType.GetCurSel();
	m_nIsDefaultLibrary = m_chkDefault.GetCheck();
	if(m_StrRepositoryPath.IsEmpty()) 
	{
		MessageBox(_T("Invalid dest directory"));
		m_txtRepositoryPath.SetFocus();
		return;
	}
	CDialogEx::OnOK();
}

void CLibrarySettingDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	m_cmbWorkspaceType.SetCurSel(m_nWorkspaceType);

	m_chkRepository.SetCheck(m_nIsUseRepository);
	m_txtRepositoryPath.EnableWindow(m_nIsUseRepository);
	m_btnDirectory.EnableWindow(m_nIsUseRepository);

	m_txtRepositoryPath.SetWindowTextW(m_StrRepositoryPath);
	
	m_txtWorkspacePath.SetWindowTextW(m_StrWorkspacePath);

	m_chkDefault.SetCheck(m_nIsDefaultLibrary);
}
BOOL CLibrarySettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbWorkspaceType.AddString(_T("Keep original file after adding to Repository"));
	m_cmbWorkspaceType.AddString(_T("Delete original file after adding to Repository"));
	m_cmbWorkspaceType.AddString(_T("Prompt to delete the original file after adding to Repository"));

	return true;
}
