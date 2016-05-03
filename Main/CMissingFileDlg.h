#pragma once

#include "stdafx.h"
// CMissingFileDlg dialog

class CMissingFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CMissingFileDlg)

public:
	CString mPath;


	CMissingFileDlg(LPCTSTR inPath, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMissingFileDlg();

// Dialog Data
	enum { IDD = IDD_MISSING_FILE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedUpdateLocationBtn();
public:
	virtual BOOL OnInitDialog();
};
