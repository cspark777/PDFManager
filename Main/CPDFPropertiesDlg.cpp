#include "stdafx.h"
#include "Define.h"
#include "CPDFPropertiesDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPDFPropertiesDlg, CDialog)

/////////////////////////////////////////////////////////////////////////
// CPDFPropertiesDlg.cpp
/////////////////////////////////////////////////////////////////////////

// Message map
BEGIN_MESSAGE_MAP(CPDFPropertiesDlg, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

CPDFPropertiesDlg::CPDFPropertiesDlg(PDF_Entry* inEntry)
	: CDialog(CPDFPropertiesDlg::IDD, NULL)
{
	mEntry = inEntry;
	mFrame = new CPDFPropertiesFrame(mEntry);
}

CPDFPropertiesDlg::~CPDFPropertiesDlg()
{
	delete(mFrame);
}

void CPDFPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CPDFPropertiesDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	CWnd* lWnd;
	lWnd = GetDlgItem(IDC_OPTIONS_PANEL);
	lWnd->GetWindowRect(&mRect);
	ScreenToClient(&mRect);
	lWnd->DestroyWindow();

	// Add frame
	mFrame->Create(IDD_PDF_PROPERTIES_FRAME, this);
	mFrame->MoveWindow(&mRect, TRUE);
	mFrame->ShowWindow(SW_SHOW);

	return TRUE;
}

void CPDFPropertiesDlg::OnOK() 
{
	if (!mFrame->ValidateAndProcess())
		AfxMessageBox(_T("There was an error during validation"));
	CDialog::OnOK();
}

void CPDFPropertiesDlg::OnCancel() 
{
	CDialog::OnCancel();
}

/*void CPDFPropertiesDlg::OnPaint()
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
			mFileManagerDlg.MoveWindow(&mRect, TRUE);
			mFileManagerDlg.ShowWindow(SW_SHOW);
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
}*/