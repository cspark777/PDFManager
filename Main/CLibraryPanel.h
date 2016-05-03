//#include "CCollectionTree.h"
#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLibraryPanel : public CBCGPDockingControlBar
{
public:
	CLibraryPanel();

// Attributes
//protected:
	CTreeCtrl m_wndTree;
	DWORD_PTR mSelectedFilterID;
protected:

	//HTREEITEM mVault;
	//HTREEITEM mUnsorted;
	//HTREEITEM mCollections;
	HTREEITEM mLibrary;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibraryPanel)
	//}}AFX_VIRTUAL
	//void AddNewCollection(LPCTSTR inStr);

private:
	void InitializeLibrary();
	//void InitializeVault();
	//void InitializeCollections();

// Implementation
public:
	virtual ~CLibraryPanel();

// Generated message map functions
protected:
	//{{AFX_MSG(CLibraryPanel)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	void OnSelectTree(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

/////////////////////////////////////////////////////////////////////////////
