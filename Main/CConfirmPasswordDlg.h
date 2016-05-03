#pragma once
#include "stdafx.h"

// CConfirmPasswordDlg dialog

class CConfirmPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfirmPasswordDlg)

public:
	CConfirmPasswordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfirmPasswordDlg();

// Dialog Data
	enum { IDD = IDD_EXPORT_ZIP_CONFIRM_PASSWORD_DLG };

	CString mPassword;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedOk();
};
