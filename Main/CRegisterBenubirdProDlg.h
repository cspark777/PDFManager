#pragma once

#include "stdafx.h"

// CRegisterBenubirdProDlg dialog

class CRegisterBenubirdProDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterBenubirdProDlg)

public:
	CRegisterBenubirdProDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterBenubirdProDlg();

// Dialog Data
	enum { IDD = IDD_REGISTER_BENUBIRD_PRO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedActivateBtn();
public:
	afx_msg void OnBnClickedVisitDebenuBtn();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnEnChangeNameEdit();
};
