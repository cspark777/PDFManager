// CLicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CLicenseDlg.h"


// CLicenseDlg dialog

IMPLEMENT_DYNAMIC(CLicenseDlg, CDialog)

CLicenseDlg::CLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseDlg::IDD, pParent)
{
	AfxInitRichEdit2();
}

CLicenseDlg::~CLicenseDlg()
{
}

void CLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
 

BEGIN_MESSAGE_MAP(CLicenseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLicenseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLicenseDlg message handlers 

BOOL CLicenseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HANDLE hFilter = NULL;
	HRSRC lRes;
	HINSTANCE hRes = AfxGetResourceHandle();
	lRes = FindResource(hRes, MAKEINTRESOURCE(IDR_LICENSE_HTML), RT_HTML);
	hFilter = LoadResource(hRes, lRes);
	SetDlgItemText(IDC_LICENSE_EDIT, CA2CT((const char *)LockResource(hFilter)));
	if (hFilter) 
		UnlockResource(hFilter);  
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLicenseDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
