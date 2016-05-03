#include "stdafx.h"
#include "OutlookBar.h"
#include "CCollectionPanel.h"

#include "BCGPOutlookBarDockingPane.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COutlookBar

IMPLEMENT_DYNAMIC(COutlookBar, CBCGPOutlookBar)

COutlookBar::COutlookBar()
{
	
}

COutlookBar::~COutlookBar()
{
}

BOOL COutlookBar::Create (LPCTSTR lpszCaption, CWnd* pParentWnd, 
							const RECT& rect, UINT nID, DWORD dwStyle, 
							DWORD dwBCGStyle/* = CBRS_BCGP_RESIZE*/, 
							CCreateContext* pContext/* = NULL*/)
{
	CBCGPOutlookWnd::EnableAnimation (TRUE);

	if (!CBCGPOutlookBar::Create (lpszCaption, pParentWnd, rect, 
		nID, dwStyle, dwBCGStyle, pContext))
	{
		return FALSE;      // fail to create
	}

	CBCGPOutlookWnd* pShortcutsBarContainer = DYNAMIC_DOWNCAST (CBCGPOutlookWnd, 
							GetUnderlinedWindow ());
	if (pShortcutsBarContainer == NULL)
	{
		TRACE0("Cannot get outlook bar container\n");
		return FALSE;
	}

	if (!m_wndCollectionPanel.Create (this, ID_COLLECTION_PANEL))
	{
		return FALSE;      // fail to create
	}
	pShortcutsBarContainer->SetImageList(IDB_LIBRARY_OUTLOOK_BAR_PANEL_BMP, 16);
	pShortcutsBarContainer->AddTab (&m_wndCollectionPanel, _T("Library"), 0);

	EnableSetCaptionTextToTabName (TRUE);
	pShortcutsBarContainer->SetVisiblePageButtons(0);

	RecalcLayout ();

	return TRUE;
}


BEGIN_MESSAGE_MAP(COutlookBar, CBCGPOutlookBar)
	//{{AFX_MSG_MAP(COutlookBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutlookBar diagnostics

/////////////////////////////////////////////////////////////////////////////
// COutlookBar message handlers

int COutlookBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPBaseTabbedBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectClient (0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	ASSERT (m_pTabWnd == NULL);
	m_pTabWnd = new CBCGPOutlookWnd;

	CBCGPOutlookWnd* pTabWnd = (CBCGPOutlookWnd*) m_pTabWnd;

	// enable this before create, as it may change inside for dialog applications
	pTabWnd->m_bEnableWrapping = TRUE;
		
	// Create tabs window:
	if (!pTabWnd->Create (rectClient, this, ID_TABS_WINDOW))
	{
		TRACE0("Failed to create tab window\n");
		delete m_pTabWnd;
		m_pTabWnd = NULL;
		return -1;      // fail to create
	}
	
	pTabWnd->SetDockingBarWrapperRTC (RUNTIME_CLASS (CBCGPOutlookBarDockingPane));
	
	if (CanFloat ())
	{
		pTabWnd->HideSingleTab ();
	}
	return 0;
}

void COutlookBar::OnMoveSlider(CBCGPSlider* inSlider)
{
	CBCGPOutlookBar::OnMoveSlider(inSlider);
//?	if (inSlider->IsHorizontal())
//?		((CMainFrame*)AfxGetMainWnd())->CalculateOutlookPercent();
}

void COutlookBar::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo)
{
	m_pViewInfo = pViewInfo;

	m_pParamCDB = pParamCDB;
	m_wndCollectionPanel.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);
}