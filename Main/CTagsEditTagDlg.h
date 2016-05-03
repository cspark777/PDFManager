#pragma once

#include "stdafx.h"
// CTagsEditTagDlg dialog

class CTagsEditTagDlg : public CDialog
{
	DECLARE_DYNAMIC(CTagsEditTagDlg)

	CString &mTagName;
public:
	CTagsEditTagDlg(CString &inOutTagName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTagsEditTagDlg();

// Dialog Data
	enum { IDD = IDD_TAGS_EDITTAG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
