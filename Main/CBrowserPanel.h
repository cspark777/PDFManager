#pragma once

#include "CBBDialogBar.h"
#include "CToolbarEdit.h"
#include "CDB.h"

#define NUM_PANELS 4

struct Months
{
	UINT    ID;
	CString Name;
};

class CBBListCtrl : public CBCGPListCtrl
{
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStringList mStringList;

	CBBListCtrl();
	~CBBListCtrl();
	void SetSel(int inPos, BOOL inState = TRUE);
	int GetSel(int inPos);
	int GetMaxTextSize();
	BOOL IsVertScrollBarVisible();

	afx_msg void OnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	BOOL SelectItemStartingWith(int inASCIICode);

	
};

class CBrowserPanel : public CBBDialogBar
{
	DECLARE_SERIAL(CBrowserPanel)

	CBCGPToolBarImages mStopBtnImages;
		
	CComboBox mPanelCombo[4];
	CBBListCtrl mPanelListBox[4];

	Months mMonths[12];

public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB* m_pDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);
public:
	CBrowserPanel(CWnd* pParent = NULL);
	
	virtual ~CBrowserPanel();
	virtual void Resize (BOOL bFull);
	void GetStringForSelect(CString& outStr);
	void GetStringFromPanel(int index, CString& outStr);
	void Refresh();
	void ResetSelection();
	void ComboBoxChanged(int index);
	void SelchangeBrowserPanelList(int index);
	int  GetMonthNumFromStr(LPCTSTR inStr);
	
protected:
	void GetDatas(int index, UINT inID);
	void FillComboBox(CComboBox* inComboBox);
		
protected:
	//virtual BOOL LoadImages();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog ();
	//virtual void DrawButton(CBCGPButton& btn, CDC* pDC, const CRect& rect, UINT uiState);
	//virtual void PaintMemDC ();

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeBrowserPanel1Combo();
	afx_msg void OnCbnSelchangeBrowserPanel2Combo();
	afx_msg void OnCbnSelchangeBrowserPanel3Combo();
	afx_msg void OnCbnSelchangeBrowserPanel4Combo();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdownBrowserPanel1Combo();
	afx_msg void OnCbnDropdownBrowserPanel2Combo();
	afx_msg void OnCbnDropdownBrowserPanel3Combo();
	afx_msg void OnCbnDropdownBrowserPanel4Combo();

	afx_msg void OnLbnSelchangeBrowserPanel1List(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeBrowserPanel2List(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeBrowserPanel3List(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeBrowserPanel4List(NMHDR *pNMHDR, LRESULT *pResult);
};