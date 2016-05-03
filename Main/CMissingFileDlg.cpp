// CMissingFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CMissingFileDlg.h"


// CMissingFileDlg dialog

IMPLEMENT_DYNAMIC(CMissingFileDlg, CDialog)

CMissingFileDlg::CMissingFileDlg(LPCTSTR inPath, CWnd* pParent /*=NULL*/)
	: CDialog(CMissingFileDlg::IDD, pParent)
{
	mPath = inPath;
}

CMissingFileDlg::~CMissingFileDlg()
{
}

void CMissingFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMissingFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMissingFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_UPDATE_LOCATION_BTN, &CMissingFileDlg::OnBnClickedUpdateLocationBtn)
END_MESSAGE_MAP()


// CMissingFileDlg message handlers

void CMissingFileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CMissingFileDlg::OnBnClickedUpdateLocationBtn()
{
	OnClose();
}

BOOL CMissingFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString lStr;
	lStr.Format(_T("Missing file - %s"), mPath);
	SetWindowText(lStr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
