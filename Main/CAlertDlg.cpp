// CAlertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CAlertDlg.h"
#include "resource.h"

extern HWND gHWND;

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CAlertDlg, CDialog)
CAlertDlg::CAlertDlg(int inIconType, LPCTSTR inMessageStr, LPCTSTR inBtn1Str, LPCTSTR inBtn2Str, LPCTSTR inBtn3Str, LPCTSTR inCheckBoxStr /*NULL*/, LPCTSTR inCaptionStr /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CAlertDlg::IDD, pParent)
{
	mIconType = inIconType;
	mMessageStr = inMessageStr;
	mBtn1Str = inBtn1Str;
	mBtn2Str = inBtn2Str;
	mBtn3Str = inBtn3Str;
	mCaptionStr = inCaptionStr;
	mCheckBoxStr = inCheckBoxStr;
	mChecked = FALSE;
}

CAlertDlg::~CAlertDlg()
{
}

void CAlertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALERT_MESSAGE, m_MessageCtrl);
	DDX_Control(pDX, IDC_ALERT_BTN1, m_Btn1Ctrl);
	DDX_Control(pDX, IDC_ALERT_BTN2, m_Btn2Ctrl);
	DDX_Control(pDX, IDC_ALERT_BTN3, m_Btn3Ctrl);
	DDX_Control(pDX, IDC_ALERT_ICON, m_IconCtrl);
	DDX_Control(pDX, IDC_ALERT_CHECK, m_CheckCtrl);
}

BEGIN_MESSAGE_MAP(CAlertDlg, CDialog)
	ON_BN_CLICKED(IDC_ALERT_BTN1, OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_ALERT_BTN2, OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_ALERT_BTN3, OnBnClickedBtn3)
END_MESSAGE_MAP()


// CAlertDlg message handlers

BOOL CAlertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Caption
	if (mCaptionStr.IsEmpty())
		mCaptionStr = AfxGetAppName();
	SetWindowText(mCaptionStr);

	// Icon
	HICON lIcon = NULL;
	switch (mIconType)
	{
		case kAlertInfo:
			lIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ALERT_INFO_ICON), IMAGE_ICON, 32, 32, 0);
			break;
		case kAlertWarning:
			lIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ALERT_WARNING_ICON), IMAGE_ICON, 32, 32, 0);
			break;
		case kAlertQuestion:
			lIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ALERT_QUESTION_ICON), IMAGE_ICON, 32, 32, 0);
			break;
		case kAlertError:
			lIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ALERT_ERROR_ICON), IMAGE_ICON, 32, 32, 0);
			break;
	}
	if (lIcon)
		m_IconCtrl.SetIcon(lIcon);
	
	// Message
	m_MessageCtrl.SetWindowText(mMessageStr);

	// Button1
	if (!mBtn1Str.IsEmpty())
	{
		m_Btn1Ctrl.ShowWindow(SW_SHOW);
		m_Btn1Ctrl.SetWindowText(mBtn1Str);
	}
	else
	{
		m_Btn1Ctrl.ShowWindow(SW_HIDE);
	}

	// Button2
	if (!mBtn2Str.IsEmpty())
	{
		m_Btn2Ctrl.ShowWindow(SW_SHOW);
		m_Btn2Ctrl.SetWindowText(mBtn2Str);
	}
	else
	{
		m_Btn2Ctrl.ShowWindow(SW_HIDE);
	}

	// Button3
	if (!mBtn3Str.IsEmpty())
	{
		m_Btn3Ctrl.ShowWindow(SW_SHOW);
		m_Btn3Ctrl.SetWindowText(mBtn3Str);
	}
	else
	{
		m_Btn3Ctrl.ShowWindow(SW_HIDE);
	}

	// Set positions;
	RECT lBtn1Rect;
	RECT lBtn2Rect;
	RECT lBtn3Rect;

	m_Btn1Ctrl.GetWindowRect(&lBtn1Rect);
	m_Btn2Ctrl.GetWindowRect(&lBtn2Rect);
	m_Btn3Ctrl.GetWindowRect(&lBtn3Rect);

	ScreenToClient(&lBtn1Rect);
	ScreenToClient(&lBtn2Rect);
	ScreenToClient(&lBtn3Rect);

	int lBtn1XPos = lBtn1Rect.left;
	int lBtn2XPos = lBtn2Rect.left;
	int lBtn3XPos = lBtn3Rect.left;
	int lYPos = lBtn1Rect.top;

	if (!mBtn3Str.IsEmpty()  && mBtn2Str.IsEmpty() && mBtn1Str.IsEmpty())
	{
		m_Btn3Ctrl.SetWindowPos(&CWnd::wndTop, lBtn1XPos, lYPos, 0, 0, SWP_NOSIZE);
	}

	if (!mBtn3Str.IsEmpty() &&  !mBtn2Str.IsEmpty() && mBtn1Str.IsEmpty())
	{

		m_Btn3Ctrl.SetWindowPos(&CWnd::wndTop, lBtn2XPos, lYPos, 0, 0, SWP_NOSIZE);
		m_Btn2Ctrl.SetWindowPos(&CWnd::wndTop, lBtn1XPos, lYPos, 0, 0, SWP_NOSIZE);
	}

	if (!mBtn3Str.IsEmpty() &&  mBtn2Str.IsEmpty() && !mBtn1Str.IsEmpty())
	{

		m_Btn3Ctrl.SetWindowPos(&CWnd::wndTop, lBtn2XPos, lYPos, 0, 0, SWP_NOSIZE);
	}

	// CheckBox
	if (!mCheckBoxStr.IsEmpty())
	{
		m_CheckCtrl.ShowWindow(SW_SHOW);
		m_CheckCtrl.SetWindowText(mCheckBoxStr);
	}	
	else
		m_CheckCtrl.ShowWindow(SW_HIDE);
	
	return TRUE;
}

void CAlertDlg::OnBnClickedBtn1()
{
	mBtnPressed = 0;
	mChecked = m_CheckCtrl.GetCheck();
	CDialog::OnOK();
}

void CAlertDlg::OnBnClickedBtn2()
{
	mBtnPressed = 1;
	mChecked = m_CheckCtrl.GetCheck();
	CDialog::OnOK();
}

void CAlertDlg::OnBnClickedBtn3()
{
	mBtnPressed = 2;
	mChecked = m_CheckCtrl.GetCheck();
	CDialog::OnOK();
}

int Alert(int inIconType, LPCTSTR inMessageStr, LPCTSTR inBtn1Str, LPCTSTR inBtn2Str, LPCTSTR inBtn3Str, BOOL& outCheckBox, LPCTSTR inCheckBoxStr, LPCTSTR inCaptionStr)
{
	CAlertDlg lAlertDlg(inIconType, inMessageStr, inBtn1Str, inBtn2Str, inBtn3Str, inCheckBoxStr, inCaptionStr);
	lAlertDlg.DoModal();
	outCheckBox = lAlertDlg.mChecked;
	return lAlertDlg.mBtnPressed;
}

int Alert(int inIconType, LPCTSTR inMessageStr, LPCTSTR inBtn1Str, LPCTSTR inBtn2Str, LPCTSTR inBtn3Str, LPCTSTR inCaptionStr)
{
	CAlertDlg lAlertDlg(inIconType, inMessageStr, inBtn1Str, inBtn2Str, inBtn3Str, NULL, inCaptionStr);
	lAlertDlg.DoModal();
	return lAlertDlg.mBtnPressed;
}

int Alert(LPCTSTR inMessageStr)
{
	CAlertDlg lAlertDlg(kAlertWarning, inMessageStr, _T("OK"), NULL, NULL, NULL);
	lAlertDlg.DoModal();
	return lAlertDlg.mBtnPressed;
}