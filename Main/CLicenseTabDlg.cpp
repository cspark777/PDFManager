// CLicenseTabDlg.cpp

#include "stdafx.h"
#include "CLicenseTabDlg.h"
#include "CBBTabPanel.h"
#include "CAboutDlg.h"
#include "CRegistry.h"
#include "CLicenseDlg.h"
#include "CRegisterBenubirdProDlg.h"
#include "sn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool gIsPro;
BEGIN_MESSAGE_MAP(CLicenseTabDlg, CDialog)
	ON_BN_CLICKED(IDC_ABOUT_LICENSE_AGREEMENT_BTN, &CLicenseTabDlg::OnBnClickedAboutLicenseAgreementBtn)
	ON_BN_CLICKED(IDC_ABOUT_ENTER_SERIAL_NUMBER_BTN, &CLicenseTabDlg::OnBnClickedAboutEnterSerialNumberBtn)
END_MESSAGE_MAP()

CLicenseTabDlg::CLicenseTabDlg(CWnd* pParent)
: CBBTabPanel(pParent)
{
}

void CLicenseTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CBBTabPanel::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ABOUT_PURCHASE_LICENSE_ONLINE_BTN, m_PurchaseLicenseOnlineURLBtn);
}


bool CLicenseTabDlg::ValidateAndProcess()
{
	return true;
}

BOOL CLicenseTabDlg::OnInitDialog()
{
	CBBTabPanel::OnInitDialog();

	LOGFONT lFont;
	GetFont()->GetLogFont(&lFont);
	lFont.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&lFont);    

	((CStatic*)GetDlgItem(IDC_ABOUT_TRIAL_VERSION_STATIC))->SetFont(&m_BoldFont);

	m_PurchaseLicenseOnlineURLBtn.SizeToContent();
	m_PurchaseLicenseOnlineURLBtn.SetURL(_T("http://www.debenu.com/store/index.html"));

	ChangeDialogStatus();

	return true;
}

void CLicenseTabDlg::ChangeDialogStatus()
{
	int lDays;
	int lState;
	CString lStr;
	CString lTrialStr;

	CWinApp* lApp;
	lApp = AfxGetApp();

	CString lName = _T(""), lCompany = _T(""), lSerialNum = _T("");

	lState = sn::state(&lDays);

	if(gIsPro)
	{
		if (lState == sn::eStateRegistered)
		{
			GetDlgItem(IDC_ABOUT_TRIAL_TEXT_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ABOUT_TRIAL_VERSION_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ABOUT_PURCHASE_LICENSE_ONLINE_BTN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ABOUT_ENTER_SERIAL_NUMBER_BTN)->SetWindowText(_T("Edit Details"));
			lName      = lApp->GetProfileString(_T(""), _T("name"), lName);
			lCompany   = lApp->GetProfileString(_T(""), _T("company"), lCompany);
		}
		else
		{
			GetDlgItemText(IDC_ABOUT_TRIAL_TEXT_STATIC, lStr);
			lTrialStr.Format(lStr, lDays);
			SetDlgItemText(IDC_ABOUT_TRIAL_TEXT_STATIC, lTrialStr);	

			GetDlgItem(IDC_ABOUT_TRIAL_TEXT_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ABOUT_TRIAL_VERSION_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ABOUT_PURCHASE_LICENSE_ONLINE_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ABOUT_ENTER_SERIAL_NUMBER_BTN)->SetWindowText(_T("Enter Serial Number"));
		}
	
		lSerialNum = sn::get();
		lStr.Format(_T("Name:\t\t%s\r\nCompany:\t%s\r\nSerial number:\t%s\r\n"), lName, lCompany, lSerialNum);
		SetDlgItemText(IDC_ABOUT_LICENSED_TO_EDIT, lStr);
	}
	else
	{
		GetDlgItem(IDC_ABOUT_TRIAL_TEXT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ABOUT_TRIAL_VERSION_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ABOUT_PURCHASE_LICENSE_ONLINE_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ABOUT_ENTER_SERIAL_NUMBER_BTN)->ShowWindow(SW_HIDE); // Hide buttom
		//GetDlgItem(IDC_ABOUT_ENTER_SERIAL_NUMBER_BTN)->SetWindowText(_T("Edit Details"));
		//lName      = lApp->GetProfileString(_T(""), _T("name"), lName);
		//lCompany   = lApp->GetProfileString(_T(""), _T("company"), lCompany);
		lStr.Format(_T("The user under the terms of the\r\nEnd User's License Agreement (EULA), \r\nnot sold, by Debenu."));
		SetDlgItemText(IDC_ABOUT_LICENSED_TO_EDIT, lStr);
	}
}

void CLicenseTabDlg::OnCancel() 
{
	((CAboutDlg*)GetParent())->OnCancel();
}

BOOL CLicenseTabDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			OnCancel();
			return TRUE;
		}
		else if (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000)
		{
			if (pMsg->wParam == VK_TAB)
			{
				((CAboutDlg*)GetParent())->OnChangeTab();
				return TRUE;
			}
		}
	}
	return CBBTabPanel::PreTranslateMessage(pMsg);
}

void CLicenseTabDlg::OnBnClickedAboutLicenseAgreementBtn()
{
	CLicenseDlg lDlg;
	lDlg.DoModal();
}

void CLicenseTabDlg::OnBnClickedAboutEnterSerialNumberBtn()
{
	CRegisterBenubirdProDlg lDlg;
	lDlg.DoModal();
	ChangeDialogStatus();
}
