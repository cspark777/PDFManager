// CConfirmPasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CConfirmPasswordDlg.h"


// CConfirmPasswordDlg dialog

IMPLEMENT_DYNAMIC(CConfirmPasswordDlg, CDialog)

CConfirmPasswordDlg::CConfirmPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfirmPasswordDlg::IDD, pParent)
{

}

CConfirmPasswordDlg::~CConfirmPasswordDlg()
{
}

void CConfirmPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfirmPasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConfirmPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CConfirmPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// CConfirmPasswordDlg message handlers

void CConfirmPasswordDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_PASSWORD, mPassword);
	OnOK();
}

