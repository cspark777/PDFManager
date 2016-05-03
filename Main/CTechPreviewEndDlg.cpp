// CTechPreviewEndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CTechPreviewEndDlg.h"


// CTechPreviewEndDlg dialog

IMPLEMENT_DYNAMIC(CTechPreviewEndDlg, CDialog)

CTechPreviewEndDlg::CTechPreviewEndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTechPreviewEndDlg::IDD, pParent)
{

}

CTechPreviewEndDlg::~CTechPreviewEndDlg()
{
}

void CTechPreviewEndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTechPreviewEndDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTechPreviewEndDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTechPreviewEndDlg message handlers

void CTechPreviewEndDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
