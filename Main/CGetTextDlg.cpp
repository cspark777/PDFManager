// CGetTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CGetTextDlg.h"


// CGetTextDlg dialog

IMPLEMENT_DYNAMIC(CGetTextDlg, CDialog)

CGetTextDlg::CGetTextDlg(LPCTSTR inCaptionStr, LPCTSTR inText, CWnd* pParent /*=NULL*/)
	: CDialog(CGetTextDlg::IDD, pParent)
{
	mEnteredText = inText;
	mCaptionStr = inCaptionStr;
}

CGetTextDlg::~CGetTextDlg()
{
}

void CGetTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGetTextDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CGetTextDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGetTextDlg message handlers

void CGetTextDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	((CEdit*)GetDlgItem(IDC_NEW_COLLECTION_EDIT))->SetFocus();
	((CEdit*)GetDlgItem(IDC_NEW_COLLECTION_EDIT))->SetSel(0, -1);
}

void CGetTextDlg::OnBnClickedOk()
{
	((CEdit*)GetDlgItem(IDC_NEW_COLLECTION_EDIT))->GetWindowText(mEnteredText);
	OnOK();
}

BOOL CGetTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(mCaptionStr);
	GetDlgItem(IDC_NEW_COLLECTION_EDIT)->SetWindowText(mEnteredText);
	return true;
}