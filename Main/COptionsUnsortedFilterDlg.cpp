// COptionsUnsortedFilterDlg.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "COptionsUnsortedFilterDlg.h"
#include "COptionsDlg.h"
#include "CRegistry.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constants
//const CString kLoadOnStartup        = _T("LoadOnStartup");
//const CString kShowIconInSystemTray = _T("ShowIconInSystemTray");
//const CString kMinimizeToSystemTray = _T("MinimizeToSystemTray");

IMPLEMENT_DYNAMIC(COptionsUnsortedFilterDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsUnsortedFilterDlg, CDialog)
END_MESSAGE_MAP()
//*****************************************************************************
COptionsUnsortedFilterDlg::COptionsUnsortedFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsUnsortedFilterDlg::IDD, pParent)
{
}
//*****************************************************************************
COptionsUnsortedFilterDlg::~COptionsUnsortedFilterDlg()
{
}
//*****************************************************************************
void COptionsUnsortedFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPT_UNSORTED_FILTER_TITLE_CHECK, m_TitleCheck);
	DDX_Control(pDX, IDC_OPT_UNSORTED_FILTER_AUTHOR_CHECK, m_AuthorCheck);
	DDX_Control(pDX, IDC_OPT_UNSORTED_FILTER_SUBJECT_CHECK, m_SubjectCheck);
	DDX_Control(pDX, IDC_OPT_UNSORTED_FILTER_TAGS_CHECK, m_TagsCheck);
	DDX_Control(pDX, IDC_OPT_UNSORTED_FILTER_CATEGORIES_CHECK, m_CategoriesCheck);
	DDX_Control(pDX, IDC_OPT_UNSORTED_FILTER_COLLECTIONS_CHECK, m_CollectionsCheck);
}
//*****************************************************************************
BOOL COptionsUnsortedFilterDlg::ValidateAndProcess()
{
	CWinApp* lApp;
	int lCheck;

	lApp = AfxGetApp();

	lCheck = m_TitleCheck.GetCheck();
	lApp->WriteProfileInt(_T("Options"), _T("UFTitle"), lCheck);

	lCheck = m_AuthorCheck.GetCheck();
	lApp->WriteProfileInt(_T("Options"), _T("UFAuthor"), lCheck);

	lCheck = m_SubjectCheck.GetCheck();
	lApp->WriteProfileInt(_T("Options"), _T("UFSubject"), lCheck);

	lCheck = m_TagsCheck.GetCheck();
	lApp->WriteProfileInt(_T("Options"), _T("UFTags"), lCheck);

	lCheck = m_CategoriesCheck.GetCheck();
	lApp->WriteProfileInt(_T("Options"), _T("UFCategories"), lCheck);

	lCheck = m_CollectionsCheck.GetCheck();
	lApp->WriteProfileInt(_T("Options"), _T("UFCollections"), lCheck);

	return TRUE;
}
//*****************************************************************************
BOOL COptionsUnsortedFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWinApp* lApp;
	int lCheck;

	lApp = AfxGetApp();

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFTitle"), BST_CHECKED);
	m_TitleCheck.SetCheck(lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFAuthor"), BST_CHECKED);
	m_AuthorCheck.SetCheck(lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFSubject"), BST_UNCHECKED);
	m_SubjectCheck.SetCheck(lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFTags"), BST_CHECKED);
	m_TagsCheck.SetCheck(lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFCategories"), BST_UNCHECKED);
	m_CategoriesCheck.SetCheck(lCheck);

	lCheck = lApp->GetProfileInt(_T("Options"), _T("UFCollections"), BST_UNCHECKED);
	m_CollectionsCheck.SetCheck(lCheck);

	return TRUE;
}
//*****************************************************************************
BOOL COptionsUnsortedFilterDlg::AddToStartup()
{
	/*TCHAR lPath[MAX_PATH];
	CRegistry lReg(FALSE);
	GetModuleFileName(AfxGetInstanceHandle(), lPath, sizeof(lPath) / sizeof(TCHAR));
	lReg.SetKey(HKEY_CURRENT_USER);
	if (!lReg.CreateKey(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))) return FALSE;
	if (!lReg.Write(AfxGetAppName(), (LPCTSTR)lPath)) return FALSE;*/
	return TRUE;
}
//*****************************************************************************
BOOL COptionsUnsortedFilterDlg::RemoveFromStartup()
{
	/*CRegistry lReg(FALSE);
	lReg.SetKey(HKEY_CURRENT_USER);
	if (!lReg.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))) return FALSE;
	if (!lReg.DeleteValue(AfxGetAppName())) return FALSE;*/
	return TRUE;
}
//*****************************************************************************
BOOL COptionsUnsortedFilterDlg::IsInStartup()
{
/*	CString lPath;
	CRegistry lReg(TRUE);
	lReg.SetKey(HKEY_CURRENT_USER);
	if (!lReg.Open(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))) return FALSE;
	if (!lReg.Read(AfxGetAppName(), lPath)) return FALSE;
	if (lPath.IsEmpty()) return FALSE;*/
	return TRUE;
}