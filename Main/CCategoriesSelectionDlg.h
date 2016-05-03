#pragma once
#include "stdafx.h"

// CCategoriesSelectionDlg dialog

class CCategoriesSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CCategoriesSelectionDlg)

public:
	CCategoriesSelectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCategoriesSelectionDlg();

// Dialog Data
	enum { IDD = IDD_CATEGORIES_SELECTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCsManageCategoriesBtn();
};
