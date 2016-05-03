#pragma once

#include "stdafx.h"

// CRenameFile1Dlg dialog

class CRenameFile1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameFile1Dlg)

public:
	CRenameFile1Dlg(CString & inFileName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRenameFile1Dlg();

// Dialog Data
	enum { IDD = IDD_RENAME_FILE_DLG1 };

	CString mFileName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
