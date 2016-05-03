// COptionsDlg.h
//*****************************************************************************

#ifndef _COptionsDlg_H_
#define _COptionsDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "stdafx.h"
#include "resource.h"
#include "COptionsGeneralDlg.h"
#include "COptionsCheckForUpdatesDlg.h"
#include "COptionsNotificationsDlg.h"
#include "COptionsInterfaceDlg.h"
#include "COptionsFileManagerDlg.h"
#include "COptionsUnsortedFilterDlg.h"
#include "COptionsPropertiesDlg.h"
#include "COptionsPDFDlg.h"
#include "afxcmn.h"


//*****************************************************************************
// COptionsDlg dialog class
class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_DLG };

// Member variables
private:
	CTreeCtrl m_TreeCtrl;
	
	COptionsGeneralDlg			mGeneralDlg;
	COptionsCheckForUpdatesDlg	mCheckForUpdatesDlg;
	COptionsNotificationsDlg	mNotificationsDlg;
	COptionsInterfaceDlg		mInterfaceDlg;
	//COptionsFileManagerDlg		mFileManagerDlg;
	COptionsUnsortedFilterDlg   mUnsortedFilterDlg;
	COptionsPropertiesDlg		mPropertiesDlg;
	COptionsPDFDlg				mPDFDlg;

	HTREEITEM mCurrentItem;
	CRect mRect;
	int	mActivePanel;
	int mNumPanels;

// Public methods

public:
	PARAM_FOR_CDB *m_pParamCDB;
public:
	COptionsDlg(CWnd* pParent = NULL);
	virtual ~COptionsDlg();

// Overrides
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
    virtual void OnOK();
	
// Messages
protected:
	afx_msg void OnTvnSelchangedOptionsTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
};

#endif