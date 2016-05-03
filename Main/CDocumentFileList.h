// CDocumentFileList.h
//*****************************************************************************	

#ifndef _CDocumentFileList_H_
#define _CDocumentFileList_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "LibraryDoc.h"
#include "CListSort.h"

#include "CUtils.h"

#include "common.h"

//*****************************************************************************
// CDocumentFileList class
class CDocumentFileList : public CBCGPListCtrl
{
private:
	COleDropTarget m_DropTarget;
	DROPLISTMODE m_dropMode;
	BOOL m_bMustUninitOLE;
	
	bool mRefresh;

	CImageList sThmbImageList;
	CImageList sSmallImageList;
	CImageList sLargeImageList;
	CImageList sStateImageList16;
	CImageList sStateImageList24;
	CImageList sStateImageList32;
	
	// variables for keydown method
	CTime mKeyDownTime;
	CString mKeyDownStr;

	LPCTSTR UFileSize2Str(CString &outStr, DWORD inHi, DWORD inLo);

	 //Prevent default compiler generation of these copy constructors (no bitwise copy)
	CDocumentFileList& operator=(const CDocumentFileList& x);
	CDocumentFileList(const CDocumentFileList& x);

	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:

	void Wrapper_OnLButtonDblClk(UINT nFlags, CPoint point);
	void Wrapper_OnLButtonDown(UINT nFlags, CPoint point);

	bool PrepareOleData(bool inType);
	BOOL InsertFiles(CStringList& inFilesList);
	BOOL ValidatePathname(const CString& csPathname, UINT& iPathType, DROPLISTMODE dropMode);
	void DropFiles(HDROP dropInfo, DROPLISTMODE dropMode);
	bool PasteFiles();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);	
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);//	- moze sa pouzit - na rychlejsie zobrazenie - robilo problemy pri vykreslovani
	virtual void Sort (int iColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE);

	afx_msg void OnDropFiles(HDROP dropInfo);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDBSQL *m_pFilesSQL;
	CDB *m_pDB;
	LIBRARY_VIEW_INFO *m_pViewInfo;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB,LIBRARY_VIEW_INFO *pViewInfo);
public:
	int m_iColumn;
	BOOL m_bAscending;
	BOOL m_HeaderOff;

	CLibraryDoc* m_pDoc;

	CDocumentFileList();
	~CDocumentFileList();

	// CDocumentFileGrid/List methods
	bool m_bRunning;
	bool m_bTerminate;
	HANDLE m_hWaitingThread;
	HANDLE m_hThread;
	int nStartItem, nEndItem;
	void StopPrepareThumbnail();
	void StartPrepareThumbnail();
	static UINT __stdcall LoadThumbnail(LPVOID lpParam);

	bool IsPreparedCache(int inFrom, int inTo);
	bool ClearCache(int inFrom, int inTo);
	bool PrepareCache(int inFrom, int inTo, int nType, bool bPrepareAll = true);

	void SetupListCtrl(int nStyle);
	void PrepareImageLists(int inCount);

	int GetSortedColumn(){ return m_iSortedColumn; }
	BOOL IsAscendingSort(){ return m_bAscending; }
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// selection
	void SelectAll();
	void SelectItems(CIntArray& inSelItemsArray);

	void RowsFromIDs(CIntArray* inIDs, CIntArray& outRows);


	CBCGPHeaderCtrl* GetHeaderCtrl_MY();

	DECLARE_MESSAGE_MAP()
};

#endif