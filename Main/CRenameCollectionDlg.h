#pragma once

#include "stdafx.h"
// CRenameCollectionDlg dialog

class CRenameCollectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameCollectionDlg)

public:
	CRenameCollectionDlg(CString & inCollectionName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRenameCollectionDlg();

// Dialog Data
	enum { IDD = IDD_RENAME_COLLECTION_DLG };

	CString m_CollectionName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
