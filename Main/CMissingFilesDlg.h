#pragma once

#include "stdafx.h"
// CMissingFilesDlg dialog

class CMissingFilesDlg : public CDialog
{
	DECLARE_DYNAMIC(CMissingFilesDlg)

public:
	CMissingFilesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMissingFilesDlg();

// Dialog Data
	enum { IDD = IDD_MISSING_FILES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
