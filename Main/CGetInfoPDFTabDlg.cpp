// CGetInfoPDFTabDlg.cpp: implementation file

#include "stdafx.h"
#include "CGetInfoPDFTabDlg.h"
#include "CBBTabPanel.h"
#include "CGetInfoDlg.h"
#include "CCategoriesSelectionDlg.h"
#include "CTagsSelectionDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Messages
BEGIN_MESSAGE_MAP(CGetInfoPDFTabDlg, CBBTabPanel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// Constructor
CGetInfoPDFTabDlg::CGetInfoPDFTabDlg(CGetInfoData *inSelFilesData, CWnd* pParent /*=NULL*/)
: CBBTabPanel(pParent)
{
	mSelFilesData = inSelFilesData;
	mFrame = new CPDFPropertiesFrame(&mEntry);
	//{{AFX_DATA_INIT(CGetInfoPDFTabDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//////////////////////////////////////////////////////////////////////////
// Destructor
CGetInfoPDFTabDlg::~CGetInfoPDFTabDlg()
{
	delete(mFrame);
	mFileIdx = -1;
}

//////////////////////////////////////////////////////////////////////////
// Data exchange
void CGetInfoPDFTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CBBTabPanel::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetInfoPDFTabDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////////
// CGetInfoPDFTabDlg message handlers
bool CGetInfoPDFTabDlg::ValidateAndProcess()
{
	if (!mFrame->ValidateAndProcess())
	{ // do not need to save
		return true;
	}
	if (CPDFPropertiesFrame::SavePDFMetadata(mEntry.mPath, &mEntry) == FALSE)
	{
		AfxMessageBox(_T("There was an error during PDF Info saving"));
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Initialization
BOOL CGetInfoPDFTabDlg::OnInitDialog() 
{
	CBBTabPanel::OnInitDialog();
	
	CWnd* lWnd;
	lWnd = GetDlgItem(IDC_OPTIONS_PANEL);
	lWnd->GetWindowRect(&mRect);
	ScreenToClient(&mRect);
	lWnd->DestroyWindow();

	// Add frame
	mFrame->Create(IDD_PDF_PROPERTIES_FRAME, this);
	mFrame->MoveWindow(&mRect, TRUE);
	mFrame->ShowWindow(SW_SHOW);

	Update(((CGetInfoDlg*)GetParent())->GetItem());
	
	return TRUE;
}

// Cancel
void CGetInfoPDFTabDlg::OnCancel() 
{
	((CGetInfoDlg*)GetParent())->OnCancel();
}

BOOL CGetInfoPDFTabDlg::PreTranslateMessage(MSG* pMsg)
{
	return CBBTabPanel::PreTranslateMessage(pMsg);
}

void CGetInfoPDFTabDlg::Update(int inFileIdx)
{
	mFileIdx = inFileIdx;
	CTFiles *lTFiles = mSelFilesData->GetDataTFiles(inFileIdx);
	if (lTFiles->mDataIsValid)	
	{
		mEntry.mPath = lTFiles->mPath;
		mEntry.mName = lTFiles->mName;
		if (CPDFPropertiesFrame::LoadPDFMetadata(lTFiles->mPath, &mEntry))
			mFrame->Update();
	}
}