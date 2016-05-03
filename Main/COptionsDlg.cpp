// COptionsDlg.cpp
//*****************************************************************************

#include "stdafx.h"
#include "COptionsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OPTIONS_TREE, OnTvnSelchangedOptionsTree)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
//*****************************************************************************
COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	mActivePanel= -1;
}
//*****************************************************************************
COptionsDlg::~COptionsDlg()
{
}
//*****************************************************************************
void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPTIONS_TREE, m_TreeCtrl);
}
//*****************************************************************************
BOOL COptionsDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	CWnd* lWnd;
	lWnd = GetDlgItem(IDC_OPTIONS_PANEL);
	lWnd->GetWindowRect(&mRect);
	ScreenToClient(&mRect);
	lWnd->DestroyWindow();

	// Add child dialogs
	mGeneralDlg.Create(IDD_OPTIONS_GENERAL_DLG, this);
	mGeneralDlg.ShowWindow(SW_HIDE);

	mCheckForUpdatesDlg.Create(IDD_OPTIONS_CHECK_FOR_UPDATES_DLG, this);
	mCheckForUpdatesDlg.ShowWindow(SW_HIDE);

//	mNotificationsDlg.Create(IDD_OPTIONS_NOTIFICATIONS_DLG, this);
//	mNotificationsDlg.ShowWindow(SW_HIDE);

	mInterfaceDlg.Create(IDD_OPTIONS_INTERFACE_DLG, this);
	mInterfaceDlg.ShowWindow(SW_HIDE);

	//mFileManagerDlg.Create(IDD_OPTIONS_FILE_MANAGER_DLG, this);
	//mFileManagerDlg.ShowWindow(SW_HIDE);

	mUnsortedFilterDlg.Create(IDD_OPTIONS_UNSORTED_FILTER_DLG, this);
	mUnsortedFilterDlg.ShowWindow(SW_HIDE);

	mPropertiesDlg.Create(IDD_OPTIONS_PROPERTIES_DLG, this);
	mPropertiesDlg.ShowWindow(SW_HIDE);

	mPDFDlg.Create(IDD_OPTIONS_PDF_DLG, this);
	mPDFDlg.ShowWindow(SW_HIDE);

	// Add tree items
	HTREEITEM lPanel = NULL;
	lPanel = m_TreeCtrl.InsertItem(_T("General"));
	lPanel = m_TreeCtrl.InsertItem(_T("Check for updates"));
//	lPanel = m_TreeCtrl.InsertItem(_T("Notifications"));
	lPanel = m_TreeCtrl.InsertItem(_T("Interface"));

	lPanel = m_TreeCtrl.InsertItem(_T("Incomplete properties"));
	lPanel = m_TreeCtrl.InsertItem(_T("Import"));
	lPanel = m_TreeCtrl.InsertItem(_T("PDF options"));

	//if(m_pParamCDB->nIsUseRepository == 1) // Only incorporate the repository tab if enabled
	{
	//	lPanel = m_TreeCtrl.InsertItem(_T("File repository"));
	}

	m_TreeCtrl.SetFocus();

	return TRUE;
}
//*****************************************************************************
void COptionsDlg::OnOK() 
{
	if (!mGeneralDlg.ValidateAndProcess())
		AfxMessageBox(_T("There was an error during 'General' panel validation"));
	
	if (!mCheckForUpdatesDlg.ValidateAndProcess())
		AfxMessageBox(_T("There was an error during 'Check for updates' panel validation"));
	
//	if (!mNotificationsDlg.ValidateAndProcess())
//		AfxMessageBox(_T("There was an error during 'Notification' panel validation"));
	
	if (!mInterfaceDlg.ValidateAndProcess())
		AfxMessageBox(_T("There was an error during 'Interface' panel validation"));
	
	//if (!mFileManagerDlg.ValidateAndProcess())
	//	return;

	if (!mUnsortedFilterDlg.ValidateAndProcess())
		AfxMessageBox(_T("There was an error during 'Required properties' panel validation"));

	if (!mPropertiesDlg.ValidateAndProcess())
		AfxMessageBox(_T("There was an error during 'Properties' panel validation"));

	if (!mPDFDlg.ValidateAndProcess())
		AfxMessageBox(_T("There was an error during 'PDF properties' panel validation"));
/*
	CMainFrame* lMainFrame;
	lMainFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	lMainFrame->Refresh();
*/	
	CDialog::OnOK();
}
//*****************************************************************************
void COptionsDlg::OnCancel() 
{
	CDialog::OnCancel();
}
//*****************************************************************************
void COptionsDlg::OnTvnSelchangedOptionsTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString		 lSelectedOption;
	HTREEITEM    lItem;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	lItem = m_TreeCtrl.GetSelectedItem();
	mCurrentItem = lItem;
	lSelectedOption = m_TreeCtrl.GetItemText(lItem); 

	// Hide all child dialogs
	mGeneralDlg.ShowWindow(SW_HIDE);
	mCheckForUpdatesDlg.ShowWindow(SW_HIDE);
//	mNotificationsDlg.ShowWindow(SW_HIDE);
	mInterfaceDlg.ShowWindow(SW_HIDE);
	//mFileManagerDlg.ShowWindow(SW_HIDE);
	mUnsortedFilterDlg.ShowWindow(SW_HIDE);
	mPropertiesDlg.ShowWindow(SW_HIDE);
	mPDFDlg.ShowWindow(SW_HIDE);

	// Select active panel
	if (lSelectedOption == _T("General"))
	{
		mActivePanel = 0;
		OnPaint();
	}
	else if (lSelectedOption == _T("Check for updates")) 
	{
		mActivePanel = 1;
		OnPaint();
	}
//	else if (lSelectedOption == _T("Notifications")) 
//	{
//		mActivePanel = 2;
//		OnPaint();
//	}
	else if (lSelectedOption == _T("Interface"))
	{
		mActivePanel = 3;
		OnPaint();
	}
	else if (lSelectedOption == _T("File repository"))
	{
		mActivePanel = 4;
		OnPaint();
	}
	else if (lSelectedOption == _T("Incomplete properties"))
	{
		mActivePanel = 5;
		OnPaint();
	}
	else if (lSelectedOption == _T("Import"))
	{
		mActivePanel = 6;
		OnPaint();
	}
	else if (lSelectedOption == _T("PDF options"))
	{
		mActivePanel = 7;
		OnPaint();
	}
	*pResult = 0;
}
//*****************************************************************************
void COptionsDlg::OnPaint()
{
	CPaintDC dc(this); 
	
	// Show active dialog
	switch (mActivePanel)
	{
		case 0:
			mGeneralDlg.MoveWindow(&mRect, TRUE);
			mGeneralDlg.ShowWindow(SW_SHOW);
			break;

		case 1:
			mCheckForUpdatesDlg.MoveWindow(&mRect, TRUE);
			mCheckForUpdatesDlg.ShowWindow(SW_SHOW);
			break;
	
//		case 2:
//			mNotificationsDlg.MoveWindow(&mRect, TRUE);
//			mNotificationsDlg.ShowWindow(SW_SHOW);
//			break;

		case 3:
			mInterfaceDlg.MoveWindow(&mRect, TRUE);
			mInterfaceDlg.ShowWindow(SW_SHOW);
			break;

		case 4:
			//mFileManagerDlg.MoveWindow(&mRect, TRUE);
			//mFileManagerDlg.ShowWindow(SW_SHOW);
			break;

		case 5:
			mUnsortedFilterDlg.MoveWindow(&mRect, TRUE);
			mUnsortedFilterDlg.ShowWindow(SW_SHOW);
			break;

		case 6:
			mPropertiesDlg.MoveWindow(&mRect, TRUE);
			mPropertiesDlg.ShowWindow(SW_SHOW);
			break;

		case 7:
			mPDFDlg.MoveWindow(&mRect, TRUE);
			mPDFDlg.ShowWindow(SW_SHOW);
			break;
	}
}
//*****************************************************************************
void COptionsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_TreeCtrl.SetFocus();
}
