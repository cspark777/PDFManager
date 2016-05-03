//*****************************************************************************
// CAddMultipleTagsDlg.cpp : implementation file
//*****************************************************************************

#include "stdafx.h"
#include "Benubird.h"
#include "CAddMultipleTagsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPTagMaskEdit
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CBCGPTagMaskEdit, CBCGPMaskEdit)

BEGIN_MESSAGE_MAP(CBCGPTagMaskEdit, CBCGPMaskEdit)
	//{{AFX_MSG_MAP(CBCGPMaskEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBCGPTagMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_END:
	case VK_HOME:
	case VK_UP:
	case VK_LEFT:
	case VK_DOWN:
	case VK_RIGHT:
	case VK_BACK:
	case VK_DELETE:
	case VK_INSERT:
		CBCGPEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	default:
		CBCGPMaskEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAddMultipleTagsDlg
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CAddMultipleTagsDlg, CDialog)

CAddMultipleTagsDlg::CAddMultipleTagsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMultipleTagsDlg::IDD, pParent)
{
}

CAddMultipleTagsDlg::~CAddMultipleTagsDlg()
{
}

void CAddMultipleTagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_MULTIPLE_TAGS_EDIT, m_AddMultipleTagsEdit);
}

BEGIN_MESSAGE_MAP(CAddMultipleTagsDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CAddMultipleTagsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CAddMultipleTagsDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CAddMultipleTagsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_AddMultipleTagsEdit.SetFocus();
}

void CAddMultipleTagsDlg::OnBnClickedOk()
{
	m_AddMultipleTagsEdit.GetWindowText(mText);
	OnOK();
}

BOOL CAddMultipleTagsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_AddMultipleTagsEdit.SetValidChars(kValidChars);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}