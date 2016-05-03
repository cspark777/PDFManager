// CRenameFile1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CRenameFile1Dlg.h"


// CRenameFile1Dlg dialog

IMPLEMENT_DYNAMIC(CRenameFile1Dlg, CDialog)

CRenameFile1Dlg::CRenameFile1Dlg(CString & inFileName, CWnd* pParent /*=NULL*/)
	: CDialog(CRenameFile1Dlg::IDD, pParent)
{
	mFileName = inFileName;
}

CRenameFile1Dlg::~CRenameFile1Dlg()
{
}

void CRenameFile1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRenameFile1Dlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameFile1Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRenameFile1Dlg message handlers

BOOL CRenameFile1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_FILENAME_EDIT, mFileName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRenameFile1Dlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_FILENAME_EDIT, mFileName);
	OnOK();
}
