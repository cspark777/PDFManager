// CRegisterBenubirdProDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "CRegisterBenubirdProDlg.h"
#include "sn.h"


// CRegisterBenubirdProDlg dialog

IMPLEMENT_DYNAMIC(CRegisterBenubirdProDlg, CDialog)

CRegisterBenubirdProDlg::CRegisterBenubirdProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterBenubirdProDlg::IDD, pParent)
{

}

CRegisterBenubirdProDlg::~CRegisterBenubirdProDlg()
{
}

void CRegisterBenubirdProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterBenubirdProDlg, CDialog)
	ON_BN_CLICKED(ID_ACTIVATE_BTN, &CRegisterBenubirdProDlg::OnBnClickedActivateBtn)
	ON_BN_CLICKED(IDC_VISIT_DEBENU_BTN, &CRegisterBenubirdProDlg::OnBnClickedVisitDebenuBtn)
	ON_EN_CHANGE(IDC_NAME_EDIT, &CRegisterBenubirdProDlg::OnEnChangeNameEdit)
END_MESSAGE_MAP()

void CRegisterBenubirdProDlg::OnBnClickedActivateBtn()
{
	CString lSerialNumber;
	GetDlgItemText(IDC_SERIAL_NUMBER_EDIT, lSerialNumber);

	int lDays;
	BOOL lRegisteredBefore = FALSE;
	if (sn::state(&lDays) == sn::eStateRegistered)
		lRegisteredBefore = TRUE;

	if (sn::check(lSerialNumber))
	{
		//sn::write(lStr);
		CWinApp* lApp;
		lApp = AfxGetApp();

		GetDlgItemText(IDC_SERIAL_NUMBER_EDIT, lSerialNumber);
		sn::set(lSerialNumber);
//		lApp->WriteProfileString(_T(""), _T("serialNumber"), lSerialNumber);

		CString lName;
		GetDlgItemText(IDC_NAME_EDIT, lName);
		lApp->WriteProfileString(_T(""), _T("name"), lName);

		CString lCompany;
		GetDlgItemText(IDC_COMPANY_EDIT, lCompany);
		lApp->WriteProfileString(_T(""), _T("company"), lCompany);

		if (!lRegisteredBefore)
			::MessageBox(NULL, _T("Benubird has been successfully registered."), _T("Register Benubird"), MB_OK | MB_ICONASTERISK); //case 221
//			AfxMessageBox(_T("Benubird Pro has been successfully registered."), MB_OK | MB_ICONASTERISK);

		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(_T("Invalid serial number. Please try again."), MB_OK | MB_ICONERROR);
		((CEdit*)GetDlgItem(IDC_SERIAL_NUMBER_EDIT))->SetFocus();
		((CEdit*)GetDlgItem(IDC_SERIAL_NUMBER_EDIT))->SetSel(0, -1);
	}
}

void CRegisterBenubirdProDlg::OnBnClickedVisitDebenuBtn()
{
	CString lStr;
	lStr = _T("http://www.debenu.com/store/index.html");
	::ShellExecute(NULL, NULL, lStr, NULL, NULL, NULL);	
}

BOOL CRegisterBenubirdProDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWinApp* lApp;
	lApp = AfxGetApp();

	int lState;
	int lDays;

	CString lName = _T(""), lCompany = _T(""), lSerialNum = _T("");

	lState = sn::state(&lDays);

	if (lState == sn::eStateRegistered)	
	{
		lName      = lApp->GetProfileString(_T(""), _T("name"), lName);
		lCompany   = lApp->GetProfileString(_T(""), _T("company"), lCompany);
		GetDlgItem(IDC_SERIAL_NUMBER_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_VISIT_DEBENU_BTN)->ShowWindow(SW_HIDE);
		SetWindowText(_T("License Details"));
	}
	else
	{
		GetDlgItem(IDC_SERIAL_NUMBER_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_VISIT_DEBENU_BTN)->ShowWindow(SW_SHOW);

#ifndef _FREE	
		SetWindowText(_T("Register Benubird PDF Pro"));
#else
		SetWindowText(_T("Register Benubird PDF"));
#endif
	}
	lSerialNum = sn::get();
	SetDlgItemText(IDC_NAME_EDIT, lName);
	SetDlgItemText(IDC_COMPANY_EDIT, lCompany);
	SetDlgItemText(IDC_SERIAL_NUMBER_EDIT, lSerialNum);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRegisterBenubirdProDlg::OnEnChangeNameEdit()
{
	CString lStr;
	GetDlgItemText(IDC_NAME_EDIT, lStr);
	if (lStr.IsEmpty())
		GetDlgItem(ID_ACTIVATE_BTN)->EnableWindow(FALSE);
	else
		GetDlgItem(ID_ACTIVATE_BTN)->EnableWindow(TRUE);
}
