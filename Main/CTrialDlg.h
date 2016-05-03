#pragma once

#include "stdafx.h"
#include "afxcmn.h"
// CLicenseDlg dialog

class CTrialDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrialDlg)

public:
	CTrialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrialDlg();

// Dialog Data
	enum { IDD = IDD_TRIAL_NOTIFICATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
