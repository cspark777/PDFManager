// COptionsCheckForUpdatesDlg.h
//*****************************************************************************

#ifndef _COptionsCheckForUpdatesDlg_H_
#define _COptionsCheckForUpdatesDlg_H_

#include "stdafx.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// COptionsCheckForUpdatesDlg class
class COptionsCheckForUpdatesDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsCheckForUpdatesDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_CHECK_FOR_UPDATES_DLG };

// Member variables
private:
	CButton m_CheckForUpdatesCheck;
	CComboBox m_CheckForUpdatesCombo;

// Public methods
public:
	COptionsCheckForUpdatesDlg(CWnd* pParent = NULL);
	virtual ~COptionsCheckForUpdatesDlg();
	BOOL ValidateAndProcess();
	void UpdateUI();

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Messages
protected:
	afx_msg void OnClickedCheckForUpdatesCheck(){ UpdateUI(); }
	DECLARE_MESSAGE_MAP()
};

#endif