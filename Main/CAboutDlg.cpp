// CAboutDlg.cpp

#include "stdafx.h"
#include "CAboutDlg.h"
#include "CAboutTabDlg.h"
#include "CLicenseTabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern bool gIsPro;


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ABOUT_TAB, OnSelchangeTab)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(int inVisibleTab, CWnd* pParent)
: CDialog(CAboutDlg::IDD, pParent)
{
	mTabPanel1 = NULL;
	mTabPanel2 = NULL;
	mVisibleTab = inVisibleTab;
}

CAboutDlg::~CAboutDlg()
{
	if(mTabPanel1 != NULL)
		delete mTabPanel1;
	if(mTabPanel2 != NULL)
		delete mTabPanel2;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ABOUT_TAB, m_TabCtrl);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	mTabPanel1 = (CBBTabPanel*) new CAboutTabDlg(this);
	mTabPanel2 = (CBBTabPanel*) new CLicenseTabDlg(this);

	mTabPanel1->Create(CAboutTabDlg::IDD, this);
	mTabPanel2->Create(CLicenseTabDlg::IDD, this);

	CString lTabLabel;
	//lTabLabel.LoadString(IDS_ABOUT_ABOUTTAB);

	if(gIsPro)
		lTabLabel = _T("About Benubird PDF Pro");
	else
		lTabLabel = _T("About Benubird PDF");


	m_TabCtrl.AddPanel(mTabPanel1, lTabLabel);
	//lTabLabel.LoadString(IDS_ABOUT_LICENSETAB);
	lTabLabel = _T("License Information");
	m_TabCtrl.AddPanel(mTabPanel2, lTabLabel);

	m_TabCtrl.SetMinTabWidth(120);
	//lTabLabel.LoadString(IDS_ABOUT_CAPTION);
	//SetWindowText(lTabLabel);	

	m_TabCtrl.SetRectangle();
	m_TabCtrl.SelTab(mVisibleTab);
	m_TabCtrl.SetCurSel(mVisibleTab);
	return TRUE;
}

void CAboutDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_TabCtrl.SelTab(m_TabCtrl.GetCurFocus());
	*pResult = 0;
}

void CAboutDlg::OnOK() 
{
	if (m_TabCtrl.ValidateAndProcess() != true)
	{
		CString lStr;
		//lStr.LoadString(IDS_ABOUT_ERROR_TAB_VALIDATION);
		//AVAlertNote(CT2CA(lStr));
//		AfxMessageBox(_T("At least one tab has problem with validation!"));
		return;
	}
	UpdateData(true);
	CDialog::OnOK();
}

void CAboutDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CAboutDlg::OnChangeTab() 
{
	int lActiveTab;
	lActiveTab = m_TabCtrl.GetCurSel();
	lActiveTab++;
	lActiveTab = lActiveTab % m_TabCtrl.GetNumOfPanels();
	m_TabCtrl.SetCurSel(lActiveTab);
	m_TabCtrl.SelTab(lActiveTab);
	m_TabCtrl.SetFocus();
}
BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000)
		{
			if (pMsg->wParam == VK_TAB)
			{
				OnChangeTab();
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
