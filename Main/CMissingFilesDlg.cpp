// CMissingFilesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CMissingFilesDlg.h"


// CMissingFilesDlg dialog

IMPLEMENT_DYNAMIC(CMissingFilesDlg, CDialog)

CMissingFilesDlg::CMissingFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMissingFilesDlg::IDD, pParent)
{

}

CMissingFilesDlg::~CMissingFilesDlg()
{
}

void CMissingFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMissingFilesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMissingFilesDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMissingFilesDlg message handlers

void CMissingFilesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
