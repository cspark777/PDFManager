#pragma once

#include "stdafx.h"
#include "afxcmn.h"
// CLicenseDlg dialog

class CLicenseDlg : public CDialog
{
	DECLARE_DYNAMIC(CLicenseDlg)

public:
	CLicenseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLicenseDlg();

// Dialog Data
	enum { IDD = IDD_LICENSE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
