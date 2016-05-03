// CSplashScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CSplashScreenDlg.h"
#include "CRegisterBenubirdProDlg.h"
#include "sn.h"


// CSplashScreenDlg dialog

IMPLEMENT_DYNAMIC(CSplashScreenDlg, CDialog)

CSplashScreenDlg::CSplashScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashScreenDlg::IDD, pParent)
{

}

CSplashScreenDlg::~CSplashScreenDlg()
{
}

void CSplashScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashScreenDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SS_BUY_NOW_BTN, &CSplashScreenDlg::OnBnClickedSsBuyNowBtn)
	ON_BN_CLICKED(IDC_SS_ENTER_SERIAL_BTN, &CSplashScreenDlg::OnBnClickedSsEnterSerialBtn)
	ON_BN_CLICKED(IDC_SS_TRY_NOW_BTN, &CSplashScreenDlg::OnBnClickedSsTryNowBtn)
END_MESSAGE_MAP()


// CSplashScreenDlg message handlers


HBRUSH CSplashScreenDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH lBrush;
	int nId = pWnd->GetDlgCtrlID();
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC: 
		{    
			pDC->SetBkMode(TRANSPARENT);        
			lBrush = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
			break;
		}
	default: 
		lBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return lBrush;
}

void CSplashScreenDlg::OnBnClickedSsBuyNowBtn()
{
	CString lStr;
	lStr = _T("http://www.debenu.com/store/index.html");
	::ShellExecute(NULL, NULL, lStr, NULL, NULL, NULL);	
}

void CSplashScreenDlg::OnBnClickedSsEnterSerialBtn()
{
	CRegisterBenubirdProDlg lDlg;
	if (lDlg.DoModal() == IDOK)
		CDialog::OnOK();
}

void CSplashScreenDlg::OnBnClickedSsTryNowBtn()
{
	CDialog::OnOK();
}

BOOL CSplashScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString lText1;
	CString lText2;
	CString lStr;

	int lDaysLeft = 0;
	int lState = sn::state(&lDaysLeft);

	if (lState == sn::eStateTrial)
	{
		lStr = _T("There are %d days left of your fully functional trial period.");
		if (lDaysLeft == 1)
			lStr = _T("There is %d day left of your fully functional trial period.");
		lText1.Format(lStr, lDaysLeft);

		lStr = _T("Trial period: %d days left");
		if (lDaysLeft == 1)
			lStr = _T("Trial period: %d day left");
		lText2.Format(lStr, lDaysLeft);

		SetDlgItemText(IDC_SS_TEXT1_STATIC, lText1);
		SetDlgItemText(IDC_SS_TEXT2_STATIC, lText2);
	}
	else
	{
		lText1 = _T("Your 14 day fully functional trial period of Benubird has expired.\n\nTo continue using Benubird you need to buy a license.");
		lText2 = _T("Trial period: 14 day preview completed."); // Szabi, case 207, Text changed
		((CButton*)GetDlgItem(IDC_SS_TRY_NOW_BTN))->EnableWindow(FALSE);
		SetDlgItemText(IDC_SS_TEXT1_STATIC, lText1);
		SetDlgItemText(IDC_SS_TEXT2_STATIC, lText2);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
