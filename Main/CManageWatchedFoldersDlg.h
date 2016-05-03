#pragma once
#include "stdafx.h"
#include "afxcmn.h"

// CManageWatchedFoldersDlg dialog

class CManageWatchedFoldersDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageWatchedFoldersDlg)
public:
	CManageWatchedFoldersDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageWatchedFoldersDlg();

	void UpdateControls();

// Dialog Data
	enum { IDD = IDD_MANAGE_WATCHED_FOLDERS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void Refresh();

	DECLARE_MESSAGE_MAP()
public:
	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);

public:
	CBCGPListCtrl m_ListCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedMwfRemoveBtn();
	afx_msg void OnBnClickedMwfEditBtn();
	afx_msg void OnLvnItemchangedMwfList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkMwfList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMwfAddBtn();
	afx_msg void OnBnClickedOk();
};
