// CBBTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CBBTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBBTabCtrl

CBBTabCtrl::CBBTabCtrl()
{
	//basic initializations
	for(int i=0; i < gMaxNumPanels; i++)
		mTabPanels[i] = NULL;
	
	mNofPanels = 0;
}

CBBTabCtrl::~CBBTabCtrl()
{
}

int CBBTabCtrl::AddPanel(CBBTabPanel *inPanel, CString inName)
{
	int lPos = InsertItem(mNofPanels, inName);		//add into the tabs header

	inPanel->SetWindowText(inName);

	mTabPanels[mNofPanels++] = inPanel;	//remember the dialog
	return lPos;
}

BEGIN_MESSAGE_MAP(CBBTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CImpositionTabCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImpositionTabCtrl message handlers


//each tab panel is asked for Validating and processing its job, usually called from OnOK
bool CBBTabCtrl::ValidateAndProcess() 
{
	for(int i = 0; i<gMaxNumPanels; i++) 
	{
		if(mTabPanels[i] == NULL)
			continue;
		if(false == ((CBBTabPanel*) mTabPanels[i])->ValidateAndProcess())
			return false;	//validation failed, return with false
	}		

	return true;
}

//cals the Init method of each tab
void CBBTabCtrl::InitPanels()
{
	for(int i = 0; i<gMaxNumPanels; i++) {
		if(mTabPanels[i] == NULL)
			continue;

		((CBBTabPanel*) mTabPanels[i])->Init();
	}
}

//sets the right placing of tab panels
void CBBTabCtrl::SetRectangle()
{
	int nSel = GetCurSel();
	if(mTabPanels[nSel]->m_hWnd)
		mTabPanels[nSel]->ShowWindow(SW_HIDE);

	CRect l_rectClient;
	CRect l_rectWnd;

	GetClientRect(l_rectClient);
	AdjustRect(FALSE,l_rectClient);
	GetWindowRect(l_rectWnd);
	GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
	for(int nCount=0; nCount < mNofPanels; nCount++){
	 mTabPanels[nCount]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
	}
	mTabPanels[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);

	mTabPanels[nSel]->ShowWindow(SW_SHOW);
}

//select the panel with an given index
void CBBTabCtrl::SelTab(int inIndex)
{
	int curSel = GetCurSel();
	for(int i = 0; i<mNofPanels; i++) {
		if(mTabPanels[i] == NULL)
			continue;

		if(i == inIndex)
			mTabPanels[i]->ShowWindow(SW_SHOW);
		else
			mTabPanels[i]->ShowWindow(SW_HIDE);
	}
}

void CBBTabCtrl::RemoveAll()
{
	for(int i = 0; i<mNofPanels; i++) 
	{
		if(mTabPanels[i] == NULL)
			continue;
		mTabPanels[i]->ShowWindow(false);
		mTabPanels[i] = NULL;
	}

	DeleteAllItems();
	mNofPanels = 0;

}

CBBTabPanel* CBBTabCtrl::GetCurrentPanel()
{
	int nSel = GetCurSel();
	return mTabPanels[nSel];
}

void CBBTabCtrl::SetEnable(BOOL Enabled)
{
	for(int i = 0; i<mNofPanels; i++) 
	{
		if(mTabPanels[i] == NULL)
			continue;
		mTabPanels[i]->EnableChanges(Enabled);
	}
}