// CAboutDlg.h

#include "stdafx.h"
#include "CBBTabCtrl.h"
#include "resource.h"

#ifndef _CAboutDlg_H_
#define _CAboutDlg_H_

#pragma once

//#include "stdafx.h"
//#include "CNxAboutTab.h"

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)
protected:
	CBBTabCtrl	 m_TabCtrl;
	CBBTabPanel* mTabPanel1;
	CBBTabPanel* mTabPanel2;
	int mVisibleTab;
	
public:
	enum { IDD = IDD_ABOUT_DLG };
	CAboutDlg(int inVisibleTab, CWnd* pParent = NULL);
	void OnChangeTab();
	
	virtual ~CAboutDlg();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual void OnCancel();
	virtual void OnOK();

protected:
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
#endif 
