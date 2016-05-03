// CCollectionTree.cpp 
//*****************************************************************************

#include "stdafx.h"
#include "Define.h"
#include "resource.h"
#include "CCollectionTree.h"
#include "CGetTextDlg.h"
#include "MainFrm.h"
#include "CSmartCollectionsDlg.h"
#include "CRenameCollectionDlg.h"
#include "CProgressDlg.h"
#include "CBRepository.h"
#include "LibraryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*****************************************************************************
// Message map
BEGIN_MESSAGE_MAP(CCollectionTree, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CCollectionTree::OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CCollectionTree::OnItemExpanded)
	ON_COMMAND(ID_DELETE_COLLECTION, OnDeleteCollection)
	ON_COMMAND(ID_RENAME_COLLECTION, OnRenameCollection)
	ON_COMMAND(ID_SMART_COLLECTION_EDIT, OnEditSmartCollection)
	ON_COMMAND(ID_SMART_COLLECTION_RENAME, OnRenameSmartCollection)
	
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CCollectionTree::OnTvnEndlabeledit)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CCollectionTree::OnTvnBegindrag)
END_MESSAGE_MAP()
//*****************************************************************************
CCollectionTree::CCollectionTree()
{
	m_dropMode.iMask = DL_ACCEPT_FILES;

	// OLE initialization 
	m_bMustUninitOLE = FALSE;
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		HRESULT hr = ::OleInitialize(NULL);
		if (FAILED(hr))
			AfxMessageBox(_T("OLE initialization failed.\n\nMake sure that the OLE libraries are the correct version."));
		else
			m_bMustUninitOLE = TRUE;
	}
}

void CCollectionTree::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	((CCollectionPanel*)GetParent())->AdjustGroupSize();
}

//*****************************************************************************
CCollectionTree::~CCollectionTree()
{
	if (m_bMustUninitOLE)
		::OleUninitialize();
}
//*****************************************************************************
void CCollectionTree::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (point.x == -1 && point.y == -1)
		point = (CPoint)GetMessagePos();

	ScreenToClient(&point);
	UINT uFlags;
	HTREEITEM htItem;
	htItem = HitTest (point, &uFlags);
	if (htItem == NULL)
		return;
	m_hActiveItem = htItem;
	SelectItem(m_hActiveItem);

	DWORD_PTR lTreeID = GetItemData(m_hActiveItem);

	CMenu lMenu;

	if (TREEITEMID_IS_COLLECTION(lTreeID))
		lMenu.LoadMenu(IDR_COLLECTION_MENU);
	else if (TREEITEMID_IS_SMARTCOLLECTION(lTreeID))
		lMenu.LoadMenu(IDR_SMART_COLLECTION_MENU);
	else 
		lMenu.LoadMenu(IDR_FILTER_MENU);

	CMenu* lPopup;
	lPopup = lMenu.GetSubMenu(0);
/*
	if (!(TREEITEMID_IS_COLLECTION(lTreeID) || TREEITEMID_IS_SMARTCOLLECTION(lTreeID)))
	{
		lPopup->DeleteMenu(ID_DELETE_COLLECTION, MF_BYCOMMAND);	//remove Delete collection
		lPopup->DeleteMenu(ID_RENAME_COLLECTION, MF_BYCOMMAND);	//remove Rename collection
		lPopup->DeleteMenu(0, MF_BYPOSITION);	//remove Separator
//		return; //no context menu for items other than Collection and SmartCollections.
	}
*/
	ClientToScreen(&point);
	UINT lCommand = lPopup->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN, point.x, point.y, this); 

	switch (lCommand)
	{
		case ID_DELETE_COLLECTION:
		case ID_RENAME_COLLECTION:
		case ID_EXPORTTO_ZIPPACKAGE:
		case ID_EXPORTTO_FOLDER:
		case ID_SMART_COLLECTION_EDIT:
		case ID_SMART_COLLECTION_RENAME:
			SendMessage(WM_COMMAND, lCommand, 0);
			break;
		default:
			AfxGetMainWnd()->SendMessage(WM_COMMAND, lCommand, 0);
	}
}
//*****************************************************************************
void CCollectionTree::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt ;
	GetCursorPos(&pt) ;
	ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = HitTest(pt, &unFlags) ;
	UINT lID;
	if (hItem != NULL)
	{
		lID = GetItemData(hItem);
		
		if (TREEITEMID_IS_COLLECTION(lID))
			((CCollectionPanel*)GetParent())->SelectCollection(ID_COLLECTIONS_TREE, hItem);
		else if (TREEITEMID_IS_SMARTCOLLECTION(lID) || (lID >= FILTERTREE_COLLECTION_UNSORTED && lID <= FILTERTREE_COLLECTION_REC_ADDED))
			((CCollectionPanel*)GetParent())->SelectCollection(ID_SMART_COLLECTIONS_TREE, hItem);
		else if (lID >= FILTERTREE_MAJOR_ALL && lID <= FILTERTREE_MAJOR_OTHER)
			((CCollectionPanel*)GetParent())->SelectCollection(ID_FILTERS_TREE, hItem);
	}
	SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos());
	*pResult = 1; 
}

//*****************************************************************************
bool CCollectionTree::DeleteCollectionRecursively(int inID)
{
	CTCollections lCol(m_pParamCDB->pDB);
	if (lCol.GetCollectionsByNames(inID))
	{
		 while (lCol.mDataIsValid)
		 {
			DeleteCollectionRecursively(lCol.mID);
			lCol.LoadNextRow();
		 }
	}
	((CCollectionPanel*)GetParent())->AdjustGroupSize();
	return lCol.DeleteByID(inID);
}

void CCollectionTree::OnDeleteCollection()
{
	CString lStr;
	CString lItem;
	lItem = GetItemText(m_hActiveItem);
	lStr.Format(_T("Are you sure you wish to delete the\n'%s' collection?"), lItem);

	CWnd lWnd;
	if (lWnd.MessageBox(lStr, _T("Delete Collection"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		bool lSomethingDeleted = false;
		int lTreeID = (int)GetItemData(m_hActiveItem);
		if (TREEITEMID_IS_COLLECTION(lTreeID))
		{
			// need to delete all subcollections too (delete recursively)
			//CTCollections lColl;
			//lSomethingDeleted = lColl.DeleteByID(lTreeID - FILTERTREE_COLLECTION_MIN);
			lSomethingDeleted = DeleteCollectionRecursively(lTreeID - FILTERTREE_COLLECTION_MIN);
		}
		else if (TREEITEMID_IS_SMARTCOLLECTION(lTreeID))
		{
			CTSmartColl lSmColl(m_pParamCDB);
			lSomethingDeleted = lSmColl.DeleteByID(lTreeID - FILTERTREE_SMART_COLLECTION_MIN);
		}
		if(lSomethingDeleted)//update the tree
		{
			HTREEITEM lNewSelItem = GetPrevSiblingItem(m_hActiveItem);
			if (!lNewSelItem) lNewSelItem = GetNextSiblingItem(m_hActiveItem);
			DeleteItem(m_hActiveItem);
			m_hActiveItem = lNewSelItem;
			if (lNewSelItem)
			{
				SelectItem(lNewSelItem);
				SetFocus();

				CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
				CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
				if (pDoc != NULL)
				{
					pDoc->UpdateLibrary ();
				}
			}
			// There will be no item selected, if all item is deleted
		}
	}
	((CCollectionPanel*)GetParent())->AdjustGroupSize();
}
//*****************************************************************************
// Edit Collection name by clicking on popup menu item
void CCollectionTree::OnRenameCollection()
{
	BOOL lResult = TRUE;
	RenameItem(m_hActiveItem);
	SortCollections();
}

void CCollectionTree::OnEditSmartCollection()
{
	HTREEITEM lItem = GetSelectedItem();
	if (lItem == NULL)
		return;

	DWORD_PTR lTreeID = GetItemData(lItem);

	CSmartCollectionsDlg lSmartColDlg(((int)lTreeID - FILTERTREE_SMART_COLLECTION_MIN));
	if (lSmartColDlg.DoModal() == IDOK)		
	{
		lSmartColDlg.mTSmartColl->UpdateTableByID();
		CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
		CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
		if (pDoc != NULL)
		{
			pDoc->UpdateLibrary ();
		}
	}
}

void CCollectionTree::OnRenameSmartCollection()
{
	HTREEITEM lItem = GetSelectedItem();
	if (lItem == NULL)
		return;

	DWORD_PTR lTreeID = GetItemData(lItem) - FILTERTREE_SMART_COLLECTION_MIN;

	CTSmartColl lSmartCol(m_pParamCDB);
	lSmartCol.GetSmartCollByID(lTreeID);

	CRenameCollectionDlg lRenameDlg(lSmartCol.mName);
	if (lRenameDlg.DoModal() == IDOK)
	{
		lSmartCol.mName = lRenameDlg.m_CollectionName;
		lSmartCol.UpdateNameByID();

		SetItemText(lItem, lSmartCol.mName);
	}
}


//*****************************************************************************
// Edit Collection name by double clicking on the item
void CCollectionTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	BOOL lResult = TRUE;
	HTREEITEM lItem = HitTest(point, &nFlags);
	
	if (lItem == NULL)
		return;

	DWORD_PTR lTreeID = GetItemData(lItem);

	if (TREEITEMID_IS_COLLECTION(lTreeID))
	{
		if (!RenameItem(lItem))
			CTreeCtrl::OnLButtonDblClk(nFlags, point);
	}
	else if (TREEITEMID_IS_SMARTCOLLECTION(lTreeID))
	{
		CSmartCollectionsDlg lSmartColDlg(((int)lTreeID - FILTERTREE_SMART_COLLECTION_MIN));
		if (lSmartColDlg.DoModal() == IDOK)		
		{
			lSmartColDlg.mTSmartColl->UpdateTableByID();
			CFrameWnd* pFrameWnd = (CFrameWnd*)GetTopLevelFrame ();
			ASSERT_VALID (pFrameWnd);

			CLibraryDoc* pDoc = 
				DYNAMIC_DOWNCAST(CLibraryDoc, pFrameWnd->GetActiveDocument ());

			if (pDoc != NULL)
			{
				pDoc->UpdateLibrary ();
			}
		}
	}
}
//*****************************************************************************
// Change text in on place edit box
void CCollectionTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	TV_DISPINFO* lTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM& lItem = lTVDispInfo->item;
	*pResult = 1;

	if (lItem.pszText) 
	{
		SetItem(&lItem);
		int lItemData = (int)GetItemData(lItem.hItem);
		if (TREEITEMID_IS_COLLECTION(lItemData))
		{
			CTCollections lTCol(m_pParamCDB->pDB);
			lTCol.mID = (lItemData - FILTERTREE_COLLECTION_MIN);
			lTCol.mName = lItem.pszText;
			lTCol.UpdateTablebyID();

			SortChildren(GetParentItem(lItem.hItem));
		}
		else if (TREEITEMID_IS_SMARTCOLLECTION(lItemData))
		{
			CTSmartColl lTSmartColl(m_pParamCDB);
			lTSmartColl.mID = (lItemData - FILTERTREE_SMART_COLLECTION_MIN);
			lTSmartColl.mName = lItem.pszText;
			lTSmartColl.UpdateNameByID();
		}
		*pResult = 0;
	}
	ModifyStyle(TVS_EDITLABELS, 0);
	SortCollections();
}
//*****************************************************************************
BOOL CCollectionTree::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:									// ESC in on place edit box   - Cancel
			case VK_RETURN:									// ENTER in on place edit box - Accept new text
				if (GetEditControl() != 0) 
				{
					GetEditControl()->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
					return true;
				}
				break;
		}
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
//*****************************************************************************
BOOL CCollectionTree::RenameItem(HTREEITEM inItem)
{
	BOOL lResult = TRUE;

	if (inItem == NULL)
		lResult = FALSE;

	DWORD_PTR lTreeID = GetItemData(inItem);
	if (!(TREEITEMID_IS_COLLECTION(lTreeID) || TREEITEMID_IS_SMARTCOLLECTION(lTreeID)))
		return FALSE;

	ModifyStyle(0, TVS_EDITLABELS);
	if (lResult)
	{
		SetFocus();
		SelectItem(inItem);
		CEdit* lEdit = EditLabel(inItem);
	}
	return lResult;
}
//*****************************************************************************
void CCollectionTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CTreeCtrl::OnLButtonDown(nFlags, point);

	CPoint pt ;
	GetCursorPos(&pt) ;
	ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = HitTest(pt, &unFlags) ;
	UINT lID;
	if (hItem != NULL)
	{
		//bool lSelected = m_hActiveItem == hItem;
		
		//m_hActiveItem = hItem;
		lID = GetItemData(hItem);

		if (lID >= FILTERTREE_COLLECTION_MIN && lID <= FILTERTREE_COLLECTION_MAX)
			((CCollectionPanel*)GetParent())->ColTreeChanged();
		else if (TREEITEMID_IS_SMARTCOLLECTION(lID) || (lID >= FILTERTREE_COLLECTION_UNSORTED && lID <= FILTERTREE_COLLECTION_REC_ADDED))
			((CCollectionPanel*)GetParent())->SmartColTreeChanged();
		else if (lID >= FILTERTREE_MAJOR_ALL && lID <= FILTERTREE_MAJOR_OTHER)
			((CCollectionPanel*)GetParent())->FilterTreeChanged();
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}
//*****************************************************************************
void CCollectionTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt ;
	GetCursorPos(&pt) ;
	ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = HitTest(pt, &unFlags) ;
	if (hItem != NULL /*&& (nFlags & TVHT_ONITEMLABEL)*/)
	{
		((CCollectionPanel*)GetParent())->mSelectedCollID = GetItemData(hItem);
		CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
		CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());
		if (pDoc != NULL)
		{
			pDoc->UpdateLibrary ();
		}

		CTreeCtrl::OnRButtonDown(nFlags, point);
	}

}
//*****************************************************************************
int CCollectionTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle( WS_HSCROLL, 0 ); 
	m_DropTarget.Register(this);
	return 0;
}


//***************************************************************************
//*                               Drag & Drop                               *
//***************************************************************************
void CCollectionTree::OnDropFiles(HDROP dropInfo)
{
	UINT nNumFilesDropped = DragQueryFile(dropInfo, 0xFFFFFFFF, NULL, 0);
	TCHAR szFilename[MAX_PATH + 1];
	CString csPathname;
	CString csExpandedFilename;
	CStringList lFilesList;

	for (UINT nFile = 0; nFile < nNumFilesDropped; nFile++)
	{
		DragQueryFile(dropInfo, nFile, szFilename, MAX_PATH + 1);

		csPathname = szFilename;
		csExpandedFilename = ExpandShortcut(csPathname);
		if (!csExpandedFilename.IsEmpty())
			csPathname = csExpandedFilename;
		
		UINT iPathType = 0;
		if (ValidatePathname(csPathname, iPathType))
		{
			/*if (m_dropMode.iMask & DL_USE_CALLBACK)
			{
				if (m_dropMode.pfnCallback)
					m_dropMode.pfnCallback(this, csPathname, iPathType);
			}
			else*/
			lFilesList.AddTail(csPathname);
		}
	}
	DragFinish(dropInfo);
	InsertFiles(lFilesList);
}
//*****************************************************************************
BOOL CCollectionTree::InsertFiles(CStringList & inFilesList)
{
	CTFiles     lTFiles(m_pParamCDB);
	CIntArray   lAddedFilesIDs;
	CIntArray	lExistingFilesIDs;
	int         lColID = -1;

	CFileProperties_Entry lEntry;
	
	CPoint pt ;
	GetCursorPos(&pt) ;
	ScreenToClient(&pt) ;

	UINT unFlags = 0 ;
	HTREEITEM hItem = HitTest(pt, &unFlags) ;
	if (hItem != NULL)
		lEntry.mCollectionID = GetItemData(hItem);

	int lNumFiles = m_pViewInfo->pRepository->AddFiles(inFilesList, &lEntry);

	// Add files to collection, if files are already exist in DB
	POSITION lPos;
	lPos = inFilesList.GetHeadPosition();

	while (lPos)
	{
		LPCTSTR lPath = inFilesList.GetAt(lPos);
		int lFileID = lTFiles.GetFileIDByPath(lPath);
		if (lFileID != -1)
		{
			m_pDB->AddFileToCollection(lEntry.mCollectionID - FILTERTREE_COLLECTION_MIN, lFileID);
		}
		inFilesList.GetNext(lPos);
	}

	return (lNumFiles > 0);
}
//*****************************************************************************
BOOL CCollectionTree::ValidatePathname(const CString& csPathname, UINT& iPathType) const
{
	BOOL bValid = FALSE;
	struct _stat buf;
	int result = _tstat(csPathname, &buf);
	if (result == 0)
	{
		if ((m_dropMode.iMask & DL_ACCEPT_FOLDERS) && ((buf.st_mode & _S_IFDIR) == _S_IFDIR)) 
		{
			bValid = TRUE;
			iPathType = DL_FOLDER_TYPE;
		} 
		else if ((m_dropMode.iMask & DL_ACCEPT_FILES) && ((buf.st_mode & _S_IFREG) == _S_IFREG)) 
		{
			iPathType = DL_FILE_TYPE;
			if(m_dropMode.iMask & DL_FILTER_EXTENSION)
			{
				LPTSTR pszFileExt = PathFindExtension(csPathname);
				if(CString(pszFileExt).CompareNoCase(m_dropMode.csFileExt) == 0)
				{
					bValid = TRUE;
				}
			}
			else
			{
				bValid = TRUE;
			}
		}
	}
	return bValid;
}

//*****************************************************************************
//*                             End Drag & Drop								  *
//*****************************************************************************

void CCollectionTree::OnMouseMove(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnMouseMove(nFlags, point);
}

DROPEFFECT CTreeDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_COPY;
}
//*****************************************************************************
DROPEFFECT CTreeDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;
	m_pDestTreeCtrl = (CCollectionTree*)pWnd;
	m_hDestItem = m_pDestTreeCtrl->HitTest(point);
	if (m_hDestItem != NULL)
	{
		int lColID = (int)m_pDestTreeCtrl->GetItemData(m_hDestItem);
		if (lColID >= FILTERTREE_COLLECTION_MIN && lColID <= FILTERTREE_COLLECTION_MAX && lColID)
		{
			if (m_pDestTreeCtrl->GetExStyle() & WS_EX_ACCEPTFILES)
			{
				dropeffectRet = DROPEFFECT_COPY;
				//m_pDestTreeCtrl->SelectDropTarget(m_hDestItem);
			}
			else
				dropeffectRet = DROPEFFECT_NONE;
		}
		else
		{
			dropeffectRet = DROPEFFECT_NONE;
		}
	}	
	return dropeffectRet;
}
//*****************************************************************************
void CTreeDropTarget::OnDragLeave( CWnd* pWnd )
{
	m_pDestTreeCtrl = (CCollectionTree *)pWnd;
	m_pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE, 0);
	COleDropTarget::OnDragLeave(pWnd);
}
//*****************************************************************************
BOOL CTreeDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	m_pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE, 0);
	return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point);
}

//*****************************************************************************
// sort items
HTREEITEM CCollectionTree::FindItem(HTREEITEM hItem, DWORD_PTR dwData)
{
	if (hItem == NULL)
		return NULL;

	DWORD_PTR lData = GetItemData(hItem);
	if (lData == dwData)
		return hItem;

	HTREEITEM lFindItem = NULL;
	if (ItemHasChildren(hItem))
		lFindItem = FindItem(GetChildItem(hItem), dwData);
	if (lFindItem != NULL)
		return lFindItem;

	lFindItem = FindItem(GetNextSiblingItem(hItem), dwData);
	if (lFindItem != NULL)
		return lFindItem;

	return NULL;
}

#define ISDEFCOL(data) \
	((data >= FILTERTREE_PREDEF_COLL_BASE) && (data <= FILTERTREE_PREDEF_COLL_BASE+10))
#define ISSMARTCOL(data) \
	((data >= FILTERTREE_SMART_COLLECTION_MIN) && (data <= FILTERTREE_SMART_COLLECTION_MAX))
#define ISCOL(data) \
	((data >= FILTERTREE_COLLECTION_MIN) && (data <= FILTERTREE_COLLECTION_MAX))

int CALLBACK CollectionCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the tree control.
	// The lParam of an item is just its handle, 

	CCollectionTree* lTreeCtrl = (CCollectionTree*) lParamSort;

	DWORD_PTR lData1 = lParam1; //lTreeCtrl->GetItemData((HTREEITEM) lParam1);
	DWORD_PTR lData2 = lParam2; //lTreeCtrl->GetItemData((HTREEITEM) lParam2);
	
	// Note: Defined collection are first
	if (ISDEFCOL(lData1) && ISDEFCOL(lData2))
		return lData1 - lData2;
	if (ISDEFCOL(lData1)) return -1;
	if (ISDEFCOL(lData2)) return 1;

	// Note: Smart Collections should always be shown before normal Collections
	if (ISSMARTCOL(lData1) && !ISSMARTCOL(lData2))
		return -1;
	if (!ISSMARTCOL(lData1) && ISSMARTCOL(lData2))
		return 1;

	// Find items
	HTREEITEM lItem1 = lTreeCtrl->FindItem(lTreeCtrl->GetRootItem(), lData1);
	HTREEITEM lItem2 = lTreeCtrl->FindItem(lTreeCtrl->GetRootItem(), lData2);
	if (lItem1 == NULL) return 0;
	if (lItem2 == NULL) return 0;

	CString	strItem1 = lTreeCtrl->GetItemText(lItem1);
	strItem1.MakeLower();
	CString	strItem2 = lTreeCtrl->GetItemText(lItem2);
	strItem2.MakeLower();
	return wcscmp(strItem1, strItem2);
}

void CCollectionTree::SortCollections()
{
	HTREEITEM lCollectionItem = FindItem(GetRootItem(), FILTERTREE_COLLECTION_MIN);
	if (lCollectionItem == NULL) return;

	TVSORTCB lSortStruct;
	lSortStruct.hParent = lCollectionItem;
	lSortStruct.lParam = (LPARAM)this;
	lSortStruct.lpfnCompare = CollectionCompareProc;

	

	SortChildren(GetParentItem(lCollectionItem));

// 	SortChildrenCB(&lSortStruct);
}

//*****************************************************************************
void CCollectionTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
		OnDeleteCollection();
	else
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCollectionTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, pChild->GetActiveDocument ());

	CStringList lDraggedFiles;
	UINT        lBuffSize = 0;
	
	if (!m_hActiveItem)
		return;

	int lCollID = GetItemData(m_hActiveItem);

	CTFiles lFiles(m_pParamCDB);

	m_pFilesSQL->Clear();
	m_pFilesSQL->mSpecCondition.RemoveAll();
	m_pFilesSQL->AddField(_T("Files.*"));
	m_pFilesSQL->AddTable(_T("Files"));
	pDoc->UpdateSQLFromCollection(lCollID,m_pFilesSQL);
	CString lSQLStr = m_pFilesSQL->GetSelect();
	if (!lSQLStr.IsEmpty()) 
	{
		lFiles.mQ = m_pDB->mDB.execQuery(lSQLStr);
		lFiles.LoadCurrentRow();
		while (lFiles.mDataIsValid)
		{
			lDraggedFiles.AddTail(lFiles.mPath);
			lBuffSize += lstrlen(lFiles.mPath) + 1;
			lFiles.LoadNextRow();
		}
	}
	
	lBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (lBuffSize + 1);

	HGLOBAL    hgDrop;
	DROPFILES* pDrop;

	// Allocate memory from the heap for the DROPFILES struct.
	hgDrop = GlobalAlloc(GHND | GMEM_SHARE, lBuffSize);
	if (hgDrop == NULL)
		return;

	pDrop = (DROPFILES*)GlobalLock(hgDrop);
	if (NULL == pDrop)
	{
		GlobalFree(hgDrop);
		return;
	}

	// Fill in the DROPFILES struct.
	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.
	pDrop->fWide = TRUE;
#endif

	TCHAR* pszBuff;
	// Copy all the filenames into memory after the end of the DROPFILES struct.
	POSITION pos;
	pos = lDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));
	while (pos != NULL)
	{
		lstrcpy(pszBuff, (LPCTSTR)lDraggedFiles.GetNext(pos));
		pszBuff = 1 + _tcschr(pszBuff, '\0');
	}
	GlobalUnlock ( hgDrop );
	COleDataSource datasrc;
	FORMATETC etc = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

	// Put the data in the data source.
	datasrc.CacheGlobalData(CF_HDROP, hgDrop, &etc);

	HGLOBAL hgBool;
	hgBool = GlobalAlloc(GHND | GMEM_SHARE, sizeof(bool));
	if (hgBool == NULL)
	{
		GlobalFree(hgDrop);
		return;
	}

	DROPEFFECT dwEffect;
	if (lDraggedFiles.GetCount() < 1)
		dwEffect = DROPEFFECT_NONE;
	else
	{
		CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
		CLibraryView* pView = DYNAMIC_DOWNCAST(CLibraryView, pChild->GetActiveView ());
		pView->mDocumentFileList.ModifyStyleEx(WS_EX_ACCEPTFILES, 0);
		pView->m_wndOutlookBar.m_wndCollectionPanel.m_wndTree2.ModifyStyleEx(WS_EX_ACCEPTFILES, 0);

		dwEffect = datasrc.DoDragDrop(DROPEFFECT_COPY/* | DROPEFFECT_MOVE*/);

		pView->mDocumentFileList.ModifyStyleEx(0, WS_EX_ACCEPTFILES);
		pView->m_wndOutlookBar.m_wndCollectionPanel.m_wndTree2.ModifyStyleEx(0, WS_EX_ACCEPTFILES);
	}

	switch ( dwEffect )
	{
		case DROPEFFECT_COPY:
		case DROPEFFECT_MOVE:
			break;
		case DROPEFFECT_NONE:
		{
			GlobalFree(hgDrop);
			GlobalFree(hgBool);
			break;
		}
	}
}

void CCollectionTree::Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo)
{
	m_pParamCDB = pParamCDB;
	m_pDB = m_pParamCDB->pDB;
	m_pFilesSQL = m_pParamCDB->pFilesSQL;

	m_pViewInfo = pViewInfo;
}