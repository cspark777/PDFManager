// CBurnToDiscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CBurnToDiscDlg.h"


// CBurnToDiscDlg dialog

IMPLEMENT_DYNAMIC(CBurnToDiscDlg, CDialog)

CBurnToDiscDlg::CBurnToDiscDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBurnToDiscDlg::IDD, pParent)
{

}

CBurnToDiscDlg::~CBurnToDiscDlg()
{
}

void CBurnToDiscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBurnToDiscDlg, CDialog)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CBurnToDiscDlg message handlers

void CBurnToDiscDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	//lpMMI->ptMinTrackSize = CPoint(250, 50);

	CDialog::OnGetMinMaxInfo(lpMMI);
}
