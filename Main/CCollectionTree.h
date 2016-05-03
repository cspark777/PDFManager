// CCollectionTree.h
//*****************************************************************************	

#ifndef _CCollectionTree_H_
#define _CCollectionTree_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "CUtils.h"
#include "common.h"

//#define TREEITEMID_IS_COLLECTION(lTreeID) (lTreeID >= FILTERTREE_COLLECTION_MIN && lTreeID <= FILTERTREE_COLLECTION_MAX)
#define TREEITEMID_IS_COLLECTION(lTreeID) (lTreeID >= FILTERTREE_COLLECTION_MIN && lTreeID <= FILTERTREE_COLLECTION_MAX)
#define TREEITEMID_IS_SMARTCOLLECTION(lTreeID) (lTreeID >= FILTERTREE_SMART_COLLECTION_MIN && lTreeID <= FILTERTREE_SMART_COLLECTION_MAX)

//typedef HRESULT (CALLBACK FAR * TREE_DROP_FILES_CALLBACK)(CTreeCtrl*, const CString&, const UINT&);

/*struct DROPTREEMODE { 
	UINT iMask;								// Specifies what type of items to accept - a combination of the above flags
	CString csFileExt;						// The file extension on which to filter. Use the format ".extension". Ignored unless DL_FILTER_EXTENSION is specified
	TREE_DROP_FILES_CALLBACK pfnCallback;	// Address of your callback function. Ignored unless DL_USE_CALLBACK is specified
};*/

class CCollectionTree;

class CTreeDropTarget :public COleDropTarget	
{
	// Overrides
public:
	virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave( CWnd* pWnd );
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	// Members
public:
	//Static variable to store the window handle of source tree control
	static HWND m_shWndTreeCtrl;
private:
	// Pointer to source tree control
	CCollectionTree* m_pSourceTreeCtrl;
	// Pointer to destination tree  control
	CCollectionTree* m_pDestTreeCtrl;
	// Recursive Function for adding items from source tree to destination tree
	//void AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem );
	// Function to get the index of the item image and its 
	// selected image within the tree control image list
	//void GetItemImages(HTREEITEM hSrcTItem, int &nSelItem, int &nNonSelItem);
	HTREEITEM m_hDestItem;
};

//*****************************************************************************
// CDocumentFileList class
class CCollectionTree : public CTreeCtrl
{
// Member variables
private:
//	CCollectionPanel m_ParentPanel;
	CTreeDropTarget m_DropTarget;
	DROPLISTMODE   m_dropMode;
	BOOL           m_bMustUninitOLE;

	HCURSOR        mDragEnableCur;
	HCURSOR        mDragDisableCur;
// Public methods
public:
	LIBRARY_VIEW_INFO *m_pViewInfo;

	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
	CDBSQL * m_pFilesSQL;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo);
public:
	CCollectionTree();
	~CCollectionTree();

	HTREEITEM      m_hActiveItem;

// Private methods
private:
	BOOL RenameItem(HTREEITEM inItem);
	BOOL ValidatePathname(const CString& csPathname, UINT& iPathType) const;
	bool DeleteCollectionRecursively(int inID);
		
// Overrides
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL InsertFiles(CStringList & inFilesList);

	
	afx_msg void OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteCollection();
	afx_msg void OnRenameCollection();
	afx_msg void OnEditSmartCollection();
	afx_msg void OnRenameSmartCollection();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP dropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	HTREEITEM FindItem(HTREEITEM hItem, DWORD_PTR dwData);
	void SortCollections();
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif