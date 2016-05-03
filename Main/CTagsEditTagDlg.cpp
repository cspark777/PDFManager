// CTagsEditTagDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CTagsEditTagDlg.h"


// CTagsEditTagDlg dialog

IMPLEMENT_DYNAMIC(CTagsEditTagDlg, CDialog)

CTagsEditTagDlg::CTagsEditTagDlg(CString &inOutTagName, CWnd* pParent /*=NULL*/)
	: CDialog(CTagsEditTagDlg::IDD, pParent), mTagName(inOutTagName)
{

}

CTagsEditTagDlg::~CTagsEditTagDlg()
{
}

void CTagsEditTagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTagsEditTagDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTagsEditTagDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTagsEditTagDlg message handlers

BOOL CTagsEditTagDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_TAGNAME_EDIT, mTagName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTagsEditTagDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_TAGNAME_EDIT, mTagName);
	// TODO: Add your control notification handler code here
	OnOK();
}
