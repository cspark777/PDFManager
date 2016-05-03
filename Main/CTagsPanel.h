#pragma once

#include "CBBDialogBar.h"
#include "CToolbarEdit.h"
#include "CListSort.h"
#include "CDB.h"

typedef struct _CTTags_Entry
{
	int mID;
	CString mName;
	int mCount;
}CTTags_Entry, FAR * LPCTTags_Entry;

typedef CArray<CTTags_Entry,CTTags_Entry> CTTagsArray;

//*****************************************************************************
// CTagsListCtrl
//*****************************************************************************
class CTagsListCtrl : public CBCGPListCtrl
{
	DECLARE_DYNAMIC(CTagsListCtrl)

public:
	CTTagsArray mTagsArray;

	CTagsListCtrl();
	virtual ~CTagsListCtrl();

	int GetSortedColumn(){ return m_iSortedColumn; }
	BOOL IsAscendingSort(){ return m_bAscending; }
	virtual void Sort (int iColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE);

protected:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
};


class CTagsPanel : public CBBDialogBar
{
	DECLARE_SERIAL(CTagsPanel)

	CTagsListCtrl mTagsListBox;

	CButton mMatchAnyRadio;
	CButton mMatchAllRadio;
	CStatic mSelectedTagsStatic;
public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);
public:
	CTagsPanel(CWnd* pParent = NULL);
	
	virtual ~CTagsPanel();
	virtual void Resize (BOOL bFull);
	void GetStringForSelect(CString& outStr);
	BOOL IsEmpty(){ return mTagsListBox.GetItemCount() > 0 ? TRUE : FALSE; }
	int GetMaxSize(){ return 400; };

	void Refresh();	
	void ResetSelection();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog ();
	//virtual BOOL IsResizable () const { return FALSE; }

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedMatchAllRadio();
	afx_msg void OnBnClickedMatchAnyRadio();
	afx_msg void OnNMClickTagsList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

};