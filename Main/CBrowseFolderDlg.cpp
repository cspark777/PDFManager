// CBrowseFolderDlg.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "CBrowseFolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBrowseFolderDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(CBrowseFolderDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBrowseFolderDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//*****************************************************************************
CBrowseFolderDlg::CBrowseFolderDlg(BOOL inShowCheck, CWnd* pParent /*=NULL*/)
	: CDialog(CBrowseFolderDlg::IDD, pParent)
{
	mPath.Empty();
	mIncludeSubfolders = TRUE;
	mShowCheck = inShowCheck;
}
//*****************************************************************************
CBrowseFolderDlg::~CBrowseFolderDlg()
{
}
//*****************************************************************************
void CBrowseFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BF_SHELL_TREE, m_ShellTreeCtrl);
	DDX_Control(pDX, IDC_BF_INCLUDE_SUBFOLDERS_CHECK, m_IncludeSubfoldersCheckCtrl);
}
//*****************************************************************************
BOOL CBrowseFolderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int lCheck;

	m_ShellTreeCtrl.EnableShellContextMenu(TRUE);
	m_ShellTreeCtrl.Expand(m_ShellTreeCtrl.GetRootItem (), TVE_EXPAND);

	lCheck = mIncludeSubfolders ? BST_CHECKED : BST_UNCHECKED;
	
	m_IncludeSubfoldersCheckCtrl.SetCheck(lCheck);

	int lShow;
	lShow = mShowCheck ? SW_SHOW : SW_HIDE;
	m_IncludeSubfoldersCheckCtrl.ShowWindow(lShow); 

	return TRUE;
}
//*****************************************************************************
void CBrowseFolderDlg::OnBnClickedOk()
{
	HTREEITEM lSelectedItem;
	lSelectedItem = m_ShellTreeCtrl.GetSelectedItem();
	m_ShellTreeCtrl.GetItemPath(mPath);
	mIncludeSubfolders = m_IncludeSubfoldersCheckCtrl.GetCheck() == BST_CHECKED ? TRUE : FALSE;
	OnOK();
}