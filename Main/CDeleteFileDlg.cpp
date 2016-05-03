// CDeleteFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CDeleteFileDlg.h"


// CDeleteFileDlg dialog

IMPLEMENT_DYNAMIC(CDeleteFileDlg, CDialog)

CDeleteFileDlg::CDeleteFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteFileDlg::IDD, pParent)
{
	mAction = CDeleteFileDlg_Cancel;
}

CDeleteFileDlg::~CDeleteFileDlg()
{
}

void CDeleteFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeleteFileDlg, CDialog)
	ON_BN_CLICKED(IDC_MOVE_TO_RECYCLE_BIN_BTN, &CDeleteFileDlg::OnBnClickedMoveToRecycleBinBtn)
	ON_BN_CLICKED(IDC_KEEP_FILE_BTN, &CDeleteFileDlg::OnBnClickedKeepFileBtn)
END_MESSAGE_MAP()

void CDeleteFileDlg::OnBnClickedMoveToRecycleBinBtn()
{
	mAction = CDeleteFileDlg_MoveToRecycle;
	OnOK();
}

void CDeleteFileDlg::OnBnClickedKeepFileBtn()
{
	mAction = CDeleteFileDlg_KeepFile;
	OnOK();
}
