#pragma once

#include "stdafx.h"
#include "afxwin.h"


// CChangedFilesDlg dialog

class CChangedFilesDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangedFilesDlg)

public:
	CChangedFilesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChangedFilesDlg();

// Dialog Data
	enum { IDD = IDD_CHANGED_FILES_DLG };

	int mFileID;

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ChangedFileList;


	//virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedSelectAll();
	//afx_msg void OnBnClickedDeselectAll();
	//afx_msg void OnBnClickedOk();
};
