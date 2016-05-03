//*****************************************************************************
// CAddTagsDlg.cpp : implementation file
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "CAddTagsDlg.h"
#include "CAddMultipleTagsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAddTagsDlg, CDialog)

//****************************************************************************
// Message Map
BEGIN_MESSAGE_MAP(CAddTagsDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CAddTagsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD_MULTIPLE_BTN, &CAddTagsDlg::OnBnClickedAddMultipleBtn)
END_MESSAGE_MAP()

//*****************************************************************************
CAddTagsDlg::CAddTagsDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAddTagsDlg::IDD, pParent)
{
	mText.Empty();
	mDefaultEditText = _T("Tag to add...");
	mShowAddMultipleTagsDlg = FALSE;
}
//*****************************************************************************
CAddTagsDlg::~CAddTagsDlg()
{
}
//*****************************************************************************
void CAddTagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_TAGS_EDIT, m_AddTagsEdit);
}
//*****************************************************************************
void CAddTagsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_AddTagsEdit.SetWindowText(mDefaultEditText);
	m_AddTagsEdit.SetSel(0, -1);
	m_AddTagsEdit.SetFocus();
}
//*****************************************************************************
void CAddTagsDlg::OnBnClickedOk()
{
	m_AddTagsEdit.GetWindowText(mText);
	if (mText == mDefaultEditText)
	{
		mText.Empty();
	}
	OnOK();
}
//*****************************************************************************
BOOL CAddTagsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;
}
//*****************************************************************************
void CAddTagsDlg::OnBnClickedAddMultipleBtn()
{
	mShowAddMultipleTagsDlg = TRUE;
	OnOK();
}
