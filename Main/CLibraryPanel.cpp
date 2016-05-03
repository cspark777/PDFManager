#include "stdafx.h"
#include "Benubird.h"
#include "CLibraryPanel.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;
/////////////////////////////////////////////////////////////////////////////
// CLibraryPanel

BEGIN_MESSAGE_MAP(CLibraryPanel, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CLibraryPanel)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, 1, OnSelectTree)
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibraryPanel construction/destruction

CLibraryPanel::CLibraryPanel()
{
}
//*****************************************************************************
CLibraryPanel::~CLibraryPanel()
{
}
//*****************************************************************************
int CLibraryPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE /*| TVS_HASLINES*/ | 
								TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_FULLROWSELECT;
	
	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup trees content
	InitializeLibrary();
	return 0;
}
//*****************************************************************************
void CLibraryPanel::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CLibraryPanel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

void CLibraryPanel::InitializeLibrary()
{
	mLibrary = m_wndTree.InsertItem (_T("Library"));
	HTREEITEM lTreeItem;
	lTreeItem = m_wndTree.InsertItem (_T("All"), mLibrary);
	m_wndTree.SetItemData(lTreeItem, MAJOR_FILTER_ID_ALL);
	m_wndTree.SelectItem(lTreeItem);

	lTreeItem = m_wndTree.InsertItem (_T("Documents"), mLibrary);
	m_wndTree.SetItemData(lTreeItem, MAJOR_FILTER_ID_DOC);

	lTreeItem = m_wndTree.InsertItem (_T("Presentations"), mLibrary);
	m_wndTree.SetItemData(lTreeItem, MAJOR_FILTER_ID_PRES);

	lTreeItem = m_wndTree.InsertItem (_T("Spreadsheets"), mLibrary);
	m_wndTree.SetItemData(lTreeItem, MAJOR_FILTER_ID_SPREAD);

	lTreeItem = m_wndTree.InsertItem (_T("Images"), mLibrary);
	m_wndTree.SetItemData(lTreeItem, MAJOR_FILTER_ID_IMAGES);

	m_wndTree.Expand(mLibrary, TVE_EXPAND);
}

void CLibraryPanel::OnSelectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM lHTreeItem = m_wndTree.GetSelectedItem ();
	if (lHTreeItem != NULL)
	{
		mSelectedFilterID = m_wndTree.GetItemData(lHTreeItem);
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->RefreshFileList();
	}
	*pResult = 0;
}
void CLibraryPanel::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	
}
