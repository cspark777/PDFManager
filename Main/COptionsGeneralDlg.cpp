// COptionsGeneralDlg.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "COptionsGeneralDlg.h"
#include "COptionsDefs.h"

#include "CRegistry.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(COptionsGeneralDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsGeneralDlg, CDialog)
	ON_BN_CLICKED(IDC_OPT_GENERAL_SHOW_ICON_IN_SYSTEM_TRAY_CHECK, &COptionsGeneralDlg::OnBnClickedOptGeneralShowIconInSystemTrayCheck)
	ON_BN_CLICKED(IDC_OPT_GENERAL_REPOSITORY_CHECK, &COptionsGeneralDlg::OnBnClickedOptGeneralRepositoryCheck)
END_MESSAGE_MAP()
//*****************************************************************************
COptionsGeneralDlg::COptionsGeneralDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsGeneralDlg::IDD, pParent)
{
}
//*****************************************************************************
COptionsGeneralDlg::~COptionsGeneralDlg()
{
}
//*****************************************************************************
void COptionsGeneralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_GENERAL_LOAD_ON_STARTUP_CHECK, m_LoadOnStartupCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_SHOW_ICON_IN_SYSTEM_TRAY_CHECK, m_ShowIconInSystemTrayCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_MINIMIZE_TO_SYSTEM_TRAY_CHECK, m_MinimizeToSystmTrayCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_IGNORE_TMP_FILES_CHECK, m_IgnoreTmpFilesCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_IGNORE_WIN_FILES_CHECK, m_IgnoreWinFilesCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_IGNORE_LNK_FILES_CHECK, m_IgnoreLnkFilesCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_IGNORE_NOEXT_FILES_CHECK, m_IgnoreNoExtFilesCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_REPOSITORY_CHECK, m_UseRepositoryCheck);
	DDX_Control(pDX, IDC_OPT_GENERAL_IGNORE_SVN_CHECK, m_IgnoreSVNFilesCheck);
}
//*****************************************************************************
BOOL COptionsGeneralDlg::ValidateAndProcess()
{
	int lIntValue;
	CWinApp* lApp;

	lApp = AfxGetApp();

	lIntValue = m_LoadOnStartupCheck.GetCheck();
	if (lIntValue == BST_CHECKED)
		AddToStartup();
	else
		RemoveFromStartup();

	lIntValue = m_ShowIconInSystemTrayCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kShowIconInSystemTray, lIntValue);

	CMainFrame* lMainFrame = (CMainFrame*)theApp.GetMainWnd();
	lMainFrame->UpdateSysTrayIcon();

	lIntValue = m_MinimizeToSystmTrayCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kMinimizeToSystemTray, lIntValue);

	lIntValue = m_IgnoreTmpFilesCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kIgnoreTmpFiles, lIntValue);

	lIntValue = m_IgnoreWinFilesCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kIgnoreWinFiles, lIntValue);

	lIntValue = m_IgnoreLnkFilesCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kIgnoreLnkFiles, lIntValue);

	lIntValue = m_IgnoreNoExtFilesCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kIgnoreNoExtFiles, lIntValue);

	lIntValue = m_UseRepositoryCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kUseRepository, lIntValue);

	lIntValue = m_IgnoreSVNFilesCheck.GetCheck();
	lApp->WriteProfileInt(kOptions, kIgnoreSVNFiles, lIntValue);

	
	return TRUE;
}
//*****************************************************************************
BOOL COptionsGeneralDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int lIntValue;
	CWinApp* lApp;

	lApp = AfxGetApp();

	lIntValue = IsInStartup() ? BST_CHECKED : BST_UNCHECKED;
	m_LoadOnStartupCheck.SetCheck(lIntValue);

	lIntValue = lApp->GetProfileInt(kOptions, kShowIconInSystemTray, BST_UNCHECKED);
	m_ShowIconInSystemTrayCheck.SetCheck(lIntValue);

	lIntValue = lApp->GetProfileInt(kOptions, kMinimizeToSystemTray, BST_UNCHECKED);
	m_MinimizeToSystmTrayCheck.SetCheck(lIntValue);
	
	if (m_ShowIconInSystemTrayCheck.GetCheck() == BST_UNCHECKED)
		m_MinimizeToSystmTrayCheck.EnableWindow(FALSE);
	else
		m_MinimizeToSystmTrayCheck.EnableWindow(TRUE);

	lIntValue = lApp->GetProfileInt(kOptions, kIgnoreTmpFiles, BST_CHECKED);
	m_IgnoreTmpFilesCheck.SetCheck(lIntValue);

	lIntValue = lApp->GetProfileInt(kOptions, kIgnoreWinFiles, BST_CHECKED);
	m_IgnoreWinFilesCheck.SetCheck(lIntValue);

	lIntValue = lApp->GetProfileInt(kOptions, kIgnoreLnkFiles, BST_CHECKED);
	m_IgnoreLnkFilesCheck.SetCheck(lIntValue);

	lIntValue = lApp->GetProfileInt(kOptions, kIgnoreNoExtFiles, BST_CHECKED);
	m_IgnoreNoExtFilesCheck.SetCheck(lIntValue);

	lIntValue = lApp->GetProfileInt(kOptions, kUseRepository, BST_CHECKED);
	m_UseRepositoryCheck.SetCheck(lIntValue);
	
	lIntValue = lApp->GetProfileInt(kOptions, kIgnoreSVNFiles, BST_CHECKED);
	m_IgnoreSVNFilesCheck.SetCheck(lIntValue);

	return TRUE;
}
//*****************************************************************************
BOOL COptionsGeneralDlg::AddToStartup()
{
	TCHAR lPath[MAX_PATH];
	CRegistry lReg(FALSE);
	GetModuleFileName(AfxGetInstanceHandle(), lPath, sizeof(lPath) / sizeof(TCHAR));
	lReg.SetKey(HKEY_CURRENT_USER);
	if (!lReg.CreateKey(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))) return FALSE;
	if (!lReg.Write(AfxGetAppName(), (LPCTSTR)lPath)) return FALSE;
	return TRUE;
}
//*****************************************************************************
BOOL COptionsGeneralDlg::RemoveFromStartup()
{
	CRegistry lReg(FALSE);
	lReg.SetKey(HKEY_CURRENT_USER);
	if (!lReg.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))) return FALSE;
	if (!lReg.DeleteValue(AfxGetAppName())) return FALSE;
	return TRUE;
}
//*****************************************************************************
BOOL COptionsGeneralDlg::IsInStartup()
{
	CString lPath;
	CRegistry lReg(TRUE);
	lReg.SetKey(HKEY_CURRENT_USER);
	if (!lReg.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))) return FALSE;
	if (!lReg.Read(AfxGetAppName(), lPath)) return FALSE;
	if (lPath.IsEmpty()) return FALSE;
	return TRUE;
}
//*****************************************************************************
void COptionsGeneralDlg::OnBnClickedOptGeneralShowIconInSystemTrayCheck()
{
	if (m_ShowIconInSystemTrayCheck.GetCheck() == BST_UNCHECKED)
		m_MinimizeToSystmTrayCheck.EnableWindow(FALSE);
	else
		m_MinimizeToSystmTrayCheck.EnableWindow(TRUE);	
}

void COptionsGeneralDlg::OnBnClickedOptGeneralRepositoryCheck()
{
	CButton* pBtn = (CButton*) GetDlgItem(IDC_OPT_GENERAL_REPOSITORY_CHECK);
	if(pBtn->GetCheck()==BST_CHECKED)
	{
		// -------------------------------------- Check for permission
		CWnd lMessage;
		lMessage.MessageBox(_T("Restart Benubird to enter Repository mode."), _T("Switch modes"), MB_OK | MB_ICONINFORMATION);
		// -------------------------------------- 
	}
	else
	{
		// -------------------------------------- Check for permission
		CWnd lMessage;
		lMessage.MessageBox(_T("Restart Benubird to enter Direct mode."), _T("Switch modes"), MB_OK | MB_ICONINFORMATION);
		// -------------------------------------- 
	}
}
