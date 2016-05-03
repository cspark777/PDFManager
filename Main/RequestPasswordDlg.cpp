// RequestPasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "RequestPasswordDlg.h"


// CRequestPasswordDlg dialog

IMPLEMENT_DYNAMIC(CRequestPasswordDlg, CBCGPDialog)

CRequestPasswordDlg::CRequestPasswordDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CRequestPasswordDlg::IDD, pParent)
{

}

CRequestPasswordDlg::~CRequestPasswordDlg()
{
}

void CRequestPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BOOL CRequestPasswordDlg::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

//  SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_PDFPREVIEWICON)), TRUE);

  return TRUE;
}

BEGIN_MESSAGE_MAP(CRequestPasswordDlg, CBCGPDialog)
  ON_BN_CLICKED(IDOK, &CRequestPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CRequestPasswordDlg::OnBnClickedOk()
{
  CString str;
  GetDlgItemText(IDC_PASSWORD, str);
  password = str.GetString();

  OnOK();
}
