#if !defined(AFX_SHORTCUTSBAR_H__E74449DF_CA8F_4053_95FD_96E55BE59E56__INCLUDED_)
#define AFX_SHORTCUTSBAR_H__E74449DF_CA8F_4053_95FD_96E55BE59E56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Benubird.h"
#include "CCollectionTree.h"
#include "common.h"

class CCollectionPanel : public CBCGPTasksPane
{
public:
	LIBRARY_VIEW_INFO *m_pViewInfo;

	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo);
public:
	CCollectionTree m_wndTree1;
	CCollectionTree m_wndTree2;
	CCollectionTree m_wndTree3;

	HTREEITEM m_All;

	int mGroup1, mGroup2, mGroup3;

	DWORD dwTreeStyle;
	CCollectionPanel();
	virtual ~CCollectionPanel();
	void InitializeFilters();
	void InitializeCollectionsRecursively(int inParentID, HTREEITEM inParentItem);
	void InitializeSmartCollections();

	void FilterTreeChanged();
	void ColTreeChanged();
	void SmartColTreeChanged();

	CImageList* mCollectionImages;
	long mSelectedCollID;
	long GetSelectedCollID(){ return mSelectedCollID; };
	void AddNewCollection(LPCTSTR inStr, int inParentID);
	void AddNewSmartCollection(LPCTSTR inStr);

	void SelectAll();
	void SelectCollection(UINT inID, HTREEITEM inItem);
	void AdjustGroupSize();
//	void SetAppAsDropTarget(BOOL inSet);

public:

	virtual BOOL Create(CWnd* pParentWnd, UINT nID);
	virtual BOOL OnShowControlBarMenu (CPoint /*point*/)	{	return TRUE;	}
	BOOL CreateFiltersTree();
	BOOL CreateCollectionsTree();
	BOOL CreateSmartCollectionsTree();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif // !defined(AFX_SHORTCUTSBAR_H__E74449DF_CA8F_4053_95FD_96E55BE59E56__INCLUDED_)
