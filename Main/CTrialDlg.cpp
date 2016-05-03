// CLicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CTrialDlg.h"


// CLicenseDlg dialog

IMPLEMENT_DYNAMIC(CTrialDlg, CDialog)

CTrialDlg::CTrialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrialDlg::IDD, pParent)
{
	AfxInitRichEdit2();
}

CTrialDlg::~CTrialDlg()
{
}

void CTrialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
 

BEGIN_MESSAGE_MAP(CTrialDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTrialDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLicenseDlg message handlers 

BOOL CTrialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_TRIAL_MESSAGE, L"Trial period will be expired in 5 days");
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTrialDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
