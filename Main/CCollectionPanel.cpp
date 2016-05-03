#include "stdafx.h"
#include "Define.h"
#include "CCollectionPanel.h"
#include "MainFrm.h"
#include "LibraryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCollectionPanel::CCollectionPanel()
{
	mCollectionImages = NULL;
	mSelectedCollID = FILTERTREE_MAJOR_ALL;
	dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_TRACKSELECT | TVS_FULLROWSELECT | TVS_NOSCROLL | TVS_SHOWSELALWAYS;	
}

CCollectionPanel::~CCollectionPanel()
{
	int lCount = mCollectionImages->GetImageCount();
	if (mCollectionImages != NULL)
	{
		for (int i = 0; i < lCount; i++)
			mCollectionImages->Remove(0);	
		mCollectionImages->DeleteImageList();
		delete mCollectionImages;
	}
}

BOOL CCollectionPanel::Create(CWnd* pParentWnd, UINT nID)
{
	CRect lRect;
	lRect.SetRectEmpty();
	return CBCGPTasksPane::Create (_T("Library"), pParentWnd, lRect, FALSE, nID, WS_CHILD | WS_VISIBLE);
}

BEGIN_MESSAGE_MAP(CCollectionPanel, CBCGPTasksPane)
	ON_WM_CREATE()
	//ON_NOTIFY(NM_CLICK, ID_FILTERS_TREE, OnSelectTree1)
	//ON_NOTIFY(NM_CLICK, ID_COLLECTIONS_TREE, OnSelectTree2)
	//ON_NOTIFY(NM_CLICK, ID_SMART_COLLECTIONS_TREE, OnSelectTree3)
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CCollectionPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPToolBarImages lImages;
	mCollectionImages = new CImageList;	
	lImages.SetImageSize(CSize(16, 16));
	lImages.Load(IDB_COLLECTIONS_BMP);
	mCollectionImages->Create(16, 16, ILC_COLOR32, lImages.GetCount(), 1);
	for (int i = 0; i < lImages.GetCount(); i++)
		mCollectionImages->Add(lImages.ExtractIcon(i));

	mGroup1 = AddGroup (0, _T("Filters"));
	if (!CreateFiltersTree())
		return -1;
	AddWindow(mGroup1, m_wndTree1.GetSafeHwnd (), 130); //Szabi, case 190, size changed from 120 to 130

	mGroup2 = AddGroup (0, _T("Collections"));
	if (!CreateCollectionsTree())
		return -1;
	AddWindow(mGroup2, m_wndTree2.GetSafeHwnd (), 250);


// --------------------------------------------------------
//#ifndef _FREE	// Only display smart collections in professional version
// --------------------------------------------------------
	mGroup3 = AddGroup (0, _T("Smart Collections"));
	if (!CreateSmartCollectionsTree())
		return -1;
	AddWindow(mGroup3, m_wndTree3.GetSafeHwnd (), 300);
//#endif
	// RemoveSelection(ID_FILTERS_TREE);

	AdjustGroupSize();

	SetHorzMargin(0);
	SetVertMargin(0);
	SetGroupVertOffset(0);
	EnableScrollButtons(TRUE);
	
	return 0;
}

BOOL CCollectionPanel::CreateFiltersTree()
{
	CRect lRect(0, 0, 0, 0);
	if (!m_wndTree1.Create(dwTreeStyle, lRect, this, ID_FILTERS_TREE))
		return FALSE;
	m_wndTree1.SetImageList(mCollectionImages, LVSIL_NORMAL);
	InitializeFilters();
	m_wndTree1.RedrawWindow();
	return TRUE;
}

BOOL CCollectionPanel::CreateCollectionsTree()
{
	CRect lRect(0, 0, 0, 0);
	if (!m_wndTree2.CreateEx(WS_EX_ACCEPTFILES, dwTreeStyle, lRect, this, ID_COLLECTIONS_TREE))
		return FALSE;

	m_wndTree2.SetImageList(mCollectionImages, LVSIL_NORMAL);
	InitializeCollectionsRecursively(-1, TVI_ROOT);
	m_wndTree2.RedrawWindow ();
	return TRUE;
}

BOOL CCollectionPanel::CreateSmartCollectionsTree()
{
	CRect lRect(0, 0, 0, 0);
	if (!m_wndTree3.Create(dwTreeStyle, lRect, this, ID_SMART_COLLECTIONS_TREE))
		return FALSE;
	m_wndTree3.SetImageList(mCollectionImages, LVSIL_NORMAL);
	InitializeSmartCollections();
	m_wndTree3.RedrawWindow ();
	return TRUE;
}

void CCollectionPanel::InitializeFilters()
{
	HTREEITEM lTreeItem;

	m_All = lTreeItem = m_wndTree1.InsertItem (_T("All"), 3, 3);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_ALL);
	
	lTreeItem = m_wndTree1.InsertItem (_T("Documents"), 5, 5);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_DOC);

	lTreeItem = m_wndTree1.InsertItem (_T("Spreadsheets"), 6, 6);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_SPREAD);

	lTreeItem = m_wndTree1.InsertItem (_T("Presentations"), 7, 7);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_PRES);

	lTreeItem = m_wndTree1.InsertItem (_T("Images"), 8, 8);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_IMAGES);

	lTreeItem = m_wndTree1.InsertItem (_T("Videos"), 9, 9);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_VIDEOS);

	lTreeItem = m_wndTree1.InsertItem (_T("Other"), 10, 10);
	m_wndTree1.SetItemData(lTreeItem, FILTERTREE_MAJOR_OTHER);

	m_wndTree1.SetItemState(m_All, TVIS_SELECTED, TVIS_SELECTED);
	m_wndTree1.m_hActiveItem = m_All;
}

void CCollectionPanel::InitializeCollectionsRecursively(int inParentID, HTREEITEM inParentItem)
{
	int k = 0;
	HTREEITEM lTreeItem;

	CTCollections lCol(m_pParamCDB->pDB);
	if (lCol.GetCollectionsByNames(inParentID))
	{
		while (lCol.mDataIsValid)
		{
			int lImageIndex;
			inParentItem == TVI_ROOT ? lImageIndex = 14+k : lImageIndex = 2;
			k++;

			if(wcscmp(lCol.mName,_T("Finances"))==0)
			{
				lTreeItem = m_wndTree2.InsertItem(lCol.mName, 14, 14, inParentItem);
			}
			else if(wcscmp(lCol.mName,_T("General"))==0)
			{
				lTreeItem = m_wndTree2.InsertItem(lCol.mName, 15, 15, inParentItem);
			}	
			else if(wcscmp(lCol.mName,_T("Personal"))==0)
			{
				lTreeItem = m_wndTree2.InsertItem(lCol.mName, 16, 16, inParentItem);
			}
			else if(wcscmp(lCol.mName,_T("Work"))==0)
			{
				lTreeItem = m_wndTree2.InsertItem(lCol.mName, 17, 17, inParentItem);
			}
			else
			{
				lTreeItem = m_wndTree2.InsertItem(lCol.mName, 18, 18, inParentItem);
			}
			
		
			
			m_wndTree2.SetItemData(lTreeItem, FILTERTREE_COLLECTION_MIN + lCol.mID);
			InitializeCollectionsRecursively(lCol.mID, lTreeItem);
			lCol.LoadNextRow();
		}
	}
	m_wndTree2.SortChildren(inParentItem);
}

void CCollectionPanel::InitializeSmartCollections()
{
	HTREEITEM lTreeItem;

	lTreeItem = m_wndTree3.InsertItem(_T("Incomplete Properties"), 11, 11);
	m_wndTree3.SetItemData(lTreeItem, FILTERTREE_COLLECTION_UNSORTED);

	lTreeItem = m_wndTree3.InsertItem(_T("Last modified"), 12, 12);
	m_wndTree3.SetItemData(lTreeItem, FILTERTREE_COLLECTION_REC_MODIF);

	lTreeItem = m_wndTree3.InsertItem(_T("Last added"), 13, 13);
	m_wndTree3.SetItemData(lTreeItem, FILTERTREE_COLLECTION_REC_ADDED);

	CTSmartColl lSmartColl(m_pParamCDB);
	if (lSmartColl.GetSmartCollections())
	{
		while (lSmartColl.mDataIsValid)
		{
			lTreeItem = m_wndTree3.InsertItem(lSmartColl.mName, 4, 4);
			m_wndTree3.SetItemData(lTreeItem, FILTERTREE_SMART_COLLECTION_MIN + lSmartColl.mID);
			lSmartColl.LoadNextRow();
		}
	}
}

// name - the name of the item that is searched for  
// tree - a reference to the tree control  
// hRoot - the handle to the item where the search begins  
int FindItem(CTreeCtrl& tree, HTREEITEM hRoot)  
{  
	int nRetCount = 1;

    // check whether the current item is the searched one  
	if(tree.GetItemState(hRoot,TVIS_EXPANDED) & TVIS_EXPANDED)
	{  
		// get a handle to the first child item  
		HTREEITEM hSub = tree.GetChildItem(hRoot);  
		// iterate as long a new item is found  
		while (hSub)
		{  
			// check the children of the current item  
			nRetCount += FindItem(tree, hSub);  
  
			// get the next sibling of the current item  
			hSub = tree.GetNextSiblingItem(hSub);  
		}   
	}
    // return NULL if nothing was found  
    return nRetCount;  
}

int FindAllItem(CTreeCtrl& tree)
{  
	int nRetCount = 0;

	HTREEITEM root = tree.GetRootItem();  
	while(root != NULL)  
	{  
      nRetCount += FindItem(tree, root);  
      root = tree.GetNextSiblingItem(root);  
   }
  
   return nRetCount;  
}

void CCollectionPanel::AdjustGroupSize()
{
	SetWindowHeight(m_wndTree1.GetSafeHwnd(), FindAllItem(m_wndTree1) * m_wndTree1.GetItemHeight());
	SetWindowHeight(m_wndTree2.GetSafeHwnd(), FindAllItem(m_wndTree2) * m_wndTree1.GetItemHeight());
	SetWindowHeight(m_wndTree3.GetSafeHwnd(), FindAllItem(m_wndTree3) * m_wndTree1.GetItemHeight());
}

void CCollectionPanel::FilterTreeChanged()
{
	if (m_wndTree1.m_hWnd == NULL)
		return;

	CPoint pt ;
	GetCursorPos(&pt) ;
	m_wndTree1.ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = m_wndTree1.HitTest(pt, &unFlags) ;

	const BOOL bCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

	if (hItem != NULL)
	{
		CString lStr;
		lStr = m_wndTree1.GetItemText(hItem);
		if (m_wndTree1.m_hActiveItem == hItem && bCtrl)
		{
			SelectCollection(ID_FILTERS_TREE, m_All);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree1.SetFocus();
		}
		else
		{
			/*m_wndTree1.m_hActiveItem = hItem;
			m_wndTree1.SetItemState(m_wndTree1.m_hActiveItem, TVIS_SELECTED, TVIS_SELECTED);
			m_wndTree2.SetItemState(m_wndTree2.m_hActiveItem, 0, TVIS_SELECTED);
			m_wndTree3.SetItemState(m_wndTree3.m_hActiveItem, 0, TVIS_SELECTED);
			mSelectedCollID = m_wndTree1.GetItemData(hItem);*/
			SelectCollection(ID_FILTERS_TREE, hItem);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree1.SetFocus();
		}
	}
}

void CCollectionPanel::ColTreeChanged() 
{
	if (m_wndTree2.m_hWnd == NULL)
		return;

	CPoint pt ;
	GetCursorPos(&pt) ;
	m_wndTree2.ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = m_wndTree2.HitTest(pt, &unFlags) ;

	const BOOL bCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

	if (hItem != NULL)
	{
		if (m_wndTree2.m_hActiveItem == hItem  && bCtrl)
		{
			SelectCollection(ID_FILTERS_TREE, m_All);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree1.SetFocus();
		}
		else
		{
			/*m_wndTree2.m_hActiveItem = hItem;
			m_wndTree2.SetItemState(m_wndTree2.m_hActiveItem, TVIS_SELECTED, TVIS_SELECTED);
			m_wndTree1.SetItemState(m_wndTree1.m_hActiveItem, 0, TVIS_SELECTED);
			m_wndTree3.SetItemState(m_wndTree3.m_hActiveItem, 0, TVIS_SELECTED);
			mSelectedCollID = m_wndTree2.GetItemData(hItem);*/
			SelectCollection(ID_COLLECTIONS_TREE, hItem);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree2.SetFocus();
		}
	}
}

void CCollectionPanel::SmartColTreeChanged() 
{

	if (m_wndTree3.m_hWnd == NULL)
		return;

	CPoint pt ;
	GetCursorPos(&pt) ;
	m_wndTree3.ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = m_wndTree3.HitTest(pt, &unFlags) ;

	const BOOL bCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

	if (hItem != NULL)
	{
		if (m_wndTree3.m_hActiveItem == hItem && bCtrl)
		{
			SelectCollection(ID_FILTERS_TREE, m_All);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree1.SetFocus();
		}
		else
		{
			/*m_wndTree3.m_hActiveItem = hItem;
			m_wndTree3.SetItemState(m_wndTree3.m_hActiveItem, TVIS_SELECTED, TVIS_SELECTED);
			m_wndTree1.SetItemState(m_wndTree1.m_hActiveItem, 0, TVIS_SELECTED);
			m_wndTree2.SetItemState(m_wndTree2.m_hActiveItem, 0, TVIS_SELECTED);
			mSelectedCollID = m_wndTree3.GetItemData(hItem);*/
			SelectCollection(ID_SMART_COLLECTIONS_TREE, hItem);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree3.SetFocus();
		}
	}
}

void CCollectionPanel::AddNewCollection(LPCTSTR inStr, int inParentID)
{
	CollapseGroup(mGroup2, FALSE);

	HTREEITEM lCurrentTreeItem = TVI_ROOT;

	if (TREEITEMID_IS_COLLECTION(inParentID))
	{
		lCurrentTreeItem = m_wndTree2.GetSelectedItem();
	}

	CTCollections lCol(m_pParamCDB->pDB);
	if (lCol.GetCollectionsByName(inStr))
	{
		//must be just one row			 
		if (lCol.mDataIsValid)
		{
			int lImageIndex;
			inParentID == -1 ? lImageIndex = 18 : lImageIndex = 2;

			HTREEITEM lTreeItem = m_wndTree2.InsertItem(inStr, lImageIndex, lImageIndex, lCurrentTreeItem);
	
			//RemoveSelection(ID_COLLECTIONS_TREE);
			//mSelectedCollID = FILTERTREE_COLLECTION_MIN + lCol.mID; //Szabi: FILTERTREE_COLLECTION_MIN must be added!!!
			mSelectedCollID = FILTERTREE_COLLECTION_MIN + lCol.mID; //Szabi: FILTERTREE_COLLECTION_MIN must be added!!!
			m_wndTree2.SetItemData(lTreeItem, mSelectedCollID);
			m_wndTree1.SetItemState(m_wndTree1.m_hActiveItem, 0, TVIS_SELECTED);
			m_wndTree2.SetItemState(m_wndTree2.m_hActiveItem, 0, TVIS_SELECTED);

//#ifndef _FREE
			m_wndTree3.SetItemState(m_wndTree3.m_hActiveItem, 0, TVIS_SELECTED);
//#endif
			m_wndTree2.SetFocus();
			m_wndTree2.SelectItem(lTreeItem);
			//SelectCollection(ID_COLLECTIONS_TREE, lTreeItem);
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
			m_wndTree2.SetFocus();
			m_wndTree2.ModifyStyle(0, TVS_EDITLABELS);
			CEdit* lEdit = m_wndTree2.EditLabel(lTreeItem);

		}
	}
	AdjustGroupSize();
}

void CCollectionPanel::AddNewSmartCollection(LPCTSTR inStr)
{
	CollapseGroup(mGroup3, FALSE);
	HTREEITEM lTreeItem = m_wndTree3.InsertItem(inStr, 4, 4, TVI_ROOT);
	CTSmartColl lTSmartCol(m_pParamCDB);
	if (lTSmartCol.GetSmartCollByName(inStr))
	{
		if(lTSmartCol.mDataIsValid)
		{
			m_wndTree3.SetItemData(lTreeItem, FILTERTREE_SMART_COLLECTION_MIN + lTSmartCol.mID);
			m_wndTree3.SetFocus();
			m_wndTree3.SelectItem(lTreeItem);
			CEdit* lEdit = m_wndTree3.EditLabel(lTreeItem);
		}
	}
	AdjustGroupSize();
}
void CCollectionPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPTasksPane::OnSize(nType, cx, cy);
/*
	CRect lWindowRect;
	GetWindowRect(lWindowRect);
	ScreenToClient(lWindowRect);
	CSize lSize = lWindowRect.Size();
	int lPanel2Size = ((lSize.cy - 140) / 2) - 39;
	int lPanel3Size = lSize.cy - 140 - lPanel2Size - 78;

//#ifndef _FREE
	SetWindowHeight(mGroup2, m_wndTree2.GetSafeHwnd(), lPanel2Size);
	SetWindowHeight(mGroup3, m_wndTree3.GetSafeHwnd(), lPanel3Size);
//#else
//	SetWindowHeight(mGroup2, m_wndTree2.GetSafeHwnd(), lPanel2Size + lPanel3Size + 30);
//#endif
	
//	mPanelCombo[0].SetWindowPos(NULL, pt.x, pt.y, lPanel1ComboSize.cx, lPanel1ComboSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);*/
}

void CCollectionPanel::SelectAll()
{
	SelectCollection(ID_FILTERS_TREE, m_All);
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
	if (pDoc != NULL)
	{
		pDoc->UpdateLibrary ();
	}
	m_wndTree1.SetFocus();
}

void CCollectionPanel::SelectCollection(UINT inID, HTREEITEM inItem)
{
	if (!inItem)
		return;

	m_wndTree1.SetItemState(m_wndTree1.m_hActiveItem, 0, TVIS_SELECTED);
	m_wndTree2.SetItemState(m_wndTree2.m_hActiveItem, 0, TVIS_SELECTED);

//#ifndef _FREE
	m_wndTree3.SetItemState(m_wndTree3.m_hActiveItem, 0, TVIS_SELECTED);
//#endif

	switch (inID)
	{
		case ID_FILTERS_TREE:
		{
			m_wndTree1.m_hActiveItem = inItem;
			m_wndTree1.SetItemState(m_wndTree1.m_hActiveItem, TVIS_SELECTED, TVIS_SELECTED);
			mSelectedCollID = m_wndTree1.GetItemData(inItem);
			m_wndTree1.SetFocus();
			break;
		}
		case ID_COLLECTIONS_TREE:
		{
			m_wndTree2.m_hActiveItem = inItem;
			m_wndTree2.SetItemState(m_wndTree2.m_hActiveItem, TVIS_SELECTED, TVIS_SELECTED);
			mSelectedCollID = m_wndTree2.GetItemData(inItem);
			m_wndTree2.SetFocus();
			break;
		}
//#ifndef _FREE
		case ID_SMART_COLLECTIONS_TREE:
		{
			m_wndTree3.m_hActiveItem = inItem;
			m_wndTree3.SetItemState(m_wndTree3.m_hActiveItem, TVIS_SELECTED, TVIS_SELECTED);
			mSelectedCollID = m_wndTree3.GetItemData(inItem);
			m_wndTree3.SetFocus();
			break;
		}
//#endif
	}
}

void CCollectionPanel::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo)
{
	m_pParamCDB = pParamCDB;
	m_pViewInfo = pViewInfo;
	
	m_wndTree1.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);
	m_wndTree2.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);
	m_wndTree3.Set_ParamForCDB(m_pParamCDB,m_pViewInfo);
}
