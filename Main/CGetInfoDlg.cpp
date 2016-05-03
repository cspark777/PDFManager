// CGetInfoDlg.cpp: implementation file

#include "stdafx.h"
#include "CGetInfoDlg.h"
#include "CGetInfoSummaryTabDlg.h"
#include "CGetInfoInfoTabDlg.h"
#include "CGetInfoPDFTabDlg.h"
#include "MainFrm.h"
#include "COptionsDefs.h"
#include "LibraryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Messages
BEGIN_MESSAGE_MAP(CGetInfoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_GET_INFO_TAB, OnSelchangeTab)
	ON_NOTIFY(NM_CLICK, IDC_GET_INFO_TAB, OnClickTab)
	ON_BN_CLICKED(IDC_INFO_PREVIOUS_BTN, &CGetInfoDlg::OnBnClickedInfoPreviousBtn)
	ON_BN_CLICKED(IDC_INFO_NEXT_BTN, &CGetInfoDlg::OnBnClickedInfoNextBtn)
END_MESSAGE_MAP()

// CGetInfoDlg dialog
IMPLEMENT_DYNAMIC(CGetInfoDlg, CDialog)
CGetInfoDlg::CGetInfoDlg(CIntArray *inSelFileIDsArray, int inActivePanel, CWnd* pParent /*=NULL*/)
	: CDialog(CGetInfoDlg::IDD, pParent)
{
	mSummaryPanel = NULL;
	mInfoPanel = NULL;
	mPDFPanel = NULL;
	mActivePanel = inActivePanel;
	mItem = 0;
	mItemNum = (int) inSelFileIDsArray->GetSize();
	
	mIsPDF = false;

	m_pInSelFileIDsArray = inSelFileIDsArray;
}

CGetInfoDlg::~CGetInfoDlg()
{
	if (mPDFPanel != NULL)
	{
		delete mPDFPanel;
	}
	if (mSummaryPanel != NULL)
	{
		delete mSummaryPanel;
	}
	if (mInfoPanel != NULL)
	{
		delete mInfoPanel;
	}
	if( mSelFilesData != NULL)
	{
		delete mSelFilesData;
	}
}
void CGetInfoDlg::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB)
{
	m_pParamCDB = pParamCDB;
	mSelFilesData = new CGetInfoData(m_pInSelFileIDsArray,m_pParamCDB);
}
void CGetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFCTabDialogDlg)
	DDX_Control(pDX, IDC_GET_INFO_TAB, m_TabCtrl);
	//}}AFX_DATA_MAP
}

// CGetInfoDlg message handlers
BOOL CGetInfoDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	CWinApp* lApp = AfxGetApp();
	int lIntValue = lApp->GetProfileInt(kOptions, kPDFDisplayInfoTab, BST_CHECKED);
	if ((lIntValue == 1) && (mSelFilesData->mFileIDs->GetCount() == 1))
	{
		CTFiles* lTFiles = mSelFilesData->GetDataTFiles(0);
		CTFExtensions lTExt(m_pParamCDB->pDB);
		lTExt.GetExtensionsByID(lTFiles->mExtID);
		CString lExt = lTExt.mName;
		lExt.MakeLower();
		if (lExt == _T("pdf"))
		{
			mIsPDF = true;
		}
	}

	mSummaryPanel = (CBBTabPanel*) new CGetInfoSummaryTabDlg(mSelFilesData, this);
	mInfoPanel = (CBBTabPanel*) new CGetInfoInfoTabDlg(mSelFilesData, this);
	if (mIsPDF)
	{
		mPDFPanel = (CBBTabPanel*) new CGetInfoPDFTabDlg(mSelFilesData, this);
	}
	
	mSummaryPanel->Create(CGetInfoSummaryTabDlg::IDD, this);
	mInfoPanel->Create(CGetInfoInfoTabDlg::IDD, this);
	if (mIsPDF)
	{
		mPDFPanel->Create(CGetInfoPDFTabDlg::IDD, this);
	}
		
	m_TabCtrl.AddPanel(mSummaryPanel, _T("General"));
	m_TabCtrl.AddPanel(mInfoPanel, _T("Summary"));
	if (mIsPDF)
	{
		m_TabCtrl.AddPanel(mPDFPanel, _T("PDF"));
	}

	m_TabCtrl.SetRectangle();
	if (mActivePanel > m_TabCtrl.GetNumOfPanels()) mActivePanel = m_TabCtrl.GetNumOfPanels() - 1;
	m_TabCtrl.SelTab(mActivePanel);
	m_TabCtrl.SetCurSel(mActivePanel);

	((CGetInfoSummaryTabDlg*)mSummaryPanel)->Update(mItem);
	((CGetInfoInfoTabDlg*)mInfoPanel)->Update(mItem);
	if (mIsPDF)
	{
		((CGetInfoPDFTabDlg*)mPDFPanel)->Update(mItem);
	}
	UpdateNavigationBtn();
	
	// Szabi, Case 275
	CString lTitle;
	CTFiles *lTFiles = mSelFilesData->GetDataTFiles(mItem);
	if (lTFiles->mDataIsValid)
	{
		lTitle.Format(_T("%s Properties"), lTFiles->mName);
		SetWindowText(lTitle);
	}
	
	return TRUE;
}

void CGetInfoDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_TabCtrl.SelTab(m_TabCtrl.GetCurFocus());
	*pResult = 0;
}

void CGetInfoDlg::OnClickTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CGetInfoDlg::OnOK() 
{
	CMainFrame* lMainFrame;
	if(m_TabCtrl.ValidateAndProcess() != true)
	{
		return;
	}

	mSelFilesData->WriteChangesToDB();

// ----------------------- Direct only
	if(!(m_pParamCDB->nIsUseRepository == 1))
	{
		if(mSelFilesData->WriteChangesToDiskAndUpdateBenu())
		{
		}
	}
// ------------------------

	// Out of 'if', because it's not called, when tag removed
	//?lMainFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	//lMainFrame->GetView()->RefreshFileList();
	
	CDialog::OnOK();
}

void CGetInfoDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CGetInfoDlg::OnChangeTab() 
{
	int lActiveTab;
	lActiveTab = m_TabCtrl.GetCurSel();
	lActiveTab++;
	lActiveTab = lActiveTab % m_TabCtrl.GetNumOfPanels();
	m_TabCtrl.SetCurSel(lActiveTab);
	m_TabCtrl.SelTab(lActiveTab);
	m_TabCtrl.SetFocus();
}

BOOL CGetInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 'T' && GetKeyState(VK_CONTROL) < 0)
		{
			OnChangeTab();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CGetInfoDlg::OnBnClickedInfoPreviousBtn()
{
	if (mItem > 0)
	{
		m_TabCtrl.ValidateAndProcess();
		mItem--;
		((CGetInfoSummaryTabDlg*)mSummaryPanel)->Update(mItem);
		((CGetInfoInfoTabDlg*)mInfoPanel)->Update(mItem);
		if (mIsPDF)
		{
			((CGetInfoPDFTabDlg*)mPDFPanel)->Update(mItem);
		}
		UpdateNavigationBtn();
	}
}

void CGetInfoDlg::OnBnClickedInfoNextBtn()
{
	if (mItem < mItemNum - 1)
	{
		m_TabCtrl.ValidateAndProcess();
		mItem++;
		((CGetInfoSummaryTabDlg*)mSummaryPanel)->Update(mItem);
		((CGetInfoInfoTabDlg*)mInfoPanel)->Update(mItem);
		if (mIsPDF)
		{
			((CGetInfoPDFTabDlg*)mPDFPanel)->Update(mItem);
		}
		UpdateNavigationBtn();
	}
}

void CGetInfoDlg::UpdateNavigationBtn()
{
	BOOL lEnablePrev;
	if (mItem == 0)
	{
		lEnablePrev = FALSE;
	}
	else
	{
		lEnablePrev = TRUE;
	}

	BOOL lEnableNext;
	if (mItem == mItemNum - 1)
	{
		lEnableNext = FALSE;
	}
	else
	{
		lEnableNext = TRUE;
	}

	GetDlgItem(IDC_INFO_PREVIOUS_BTN)->EnableWindow(lEnablePrev);
	GetDlgItem(IDC_INFO_NEXT_BTN)->EnableWindow(lEnableNext);
}