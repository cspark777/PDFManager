#pragma once

#include "stdafx.h"
#include "Resource.h"

// CManageCategoriesDlg dialog

class CManageCategoriesDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageCategoriesDlg)

public:
	CManageCategoriesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageCategoriesDlg();

// Dialog Data
	enum { IDD = IDD_MANAGE_CATEGORIES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedManageTagsAddBtn();
public:
	afx_msg void OnBnClickedManageTagsEditBtn();
public:
	afx_msg void OnBnClickedManageTagsDeleteBtn();
public:
	afx_msg void OnBnClickedManageTagsImportBtn();
public:
	afx_msg void OnBnClickedManageTagsExportBtn();
};
