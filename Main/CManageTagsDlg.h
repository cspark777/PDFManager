#pragma once

#include "Resource.h"
#include "afxwin.h"

// CManageTagsDlg dialog

class CManageTagsDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageTagsDlg)

public:
	PARAM_FOR_CDB *m_pParamCDB;
public:
	CManageTagsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageTagsDlg();

// Dialog Data
	enum { IDD = IDD_MANAGE_TAGS_DLG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
public:
	
public:
	CListBox m_ManageTagsList;
	CString mAddedTags;
public:
	virtual BOOL OnInitDialog();

private:
	void UpdateUI();

private:
//	CDBTransaction lDBTransaction;
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkManageTagsList();
	afx_msg void OnLbnSelchangeManageTagsList();
	afx_msg void OnBnClickedManageTagsEditBtn();
	afx_msg void OnBnClickedManageTagsDeleteBtn();
	afx_msg void OnBnClickedManageTagsImportBtn();
	afx_msg void OnBnClickedManageTagsExportBtn();
	afx_msg void OnBnClickedManageTagsAddBtn();
};
