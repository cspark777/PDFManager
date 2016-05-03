// CGetInfoDlg.h: implementation file 

#include "Benubird.h"
#include "CBBTabCtrl.h"
#include "Resource.h"

#ifndef _CGetInfoDlg_H_
#define _CGetInfoDlg_H_

#pragma once

enum EGetInfoPanels
{
	kGetInfoPanels_Summary = 0,
	kGetInfiPanels_Info,
};


// CGetInfoDlg dialog
class CGetInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGetInfoDlg)

protected:
	CBBTabCtrl	 m_TabCtrl;
	CBBTabPanel* mSummaryPanel;
	CBBTabPanel* mInfoPanel;
	CBBTabPanel* mPDFPanel;

	CIntArray *m_pInSelFileIDsArray;
	
	int mActivePanel;
	int mItem;
	int mItemNum;
//	CIntArray *mSelFileIDs;
	CGetInfoData *mSelFilesData;
	bool mIsPDF;
public:
	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);
public:
// Dialog Data
	enum { IDD = IDD_GET_INFO_DLG };
	CGetInfoDlg(CIntArray *inSelFileIDsArray, int inActivePanel = 1, CWnd* pParent = NULL);

	int GetItem(){ return mItem; }
	bool IsPDF(){ return mIsPDF; }
	CBBTabPanel* GetPDFPanel(){return mPDFPanel;}
	void UpdateNavigationBtn();

	void OnChangeTab();
	
// Virtual methods
	virtual ~CGetInfoDlg();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
    virtual void OnOK();
	
protected:
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedInfoPreviousBtn();
public:
	afx_msg void OnBnClickedInfoNextBtn();
};

#endif