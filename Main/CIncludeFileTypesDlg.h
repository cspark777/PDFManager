#pragma once

#include "stdafx.h"
//#include "CIncludeFileTypesGridCtrl.h"


// CIncludeFileTypesDlg dialog
 
class CIncludeFileTypesDlg : public CDialog
{
	DECLARE_DYNAMIC(CIncludeFileTypesDlg)
	CBCGPListCtrl m_IncludeFileTypesListCtrl;
	//CListCtrl m_IncludeFileTypesListCtrl;
	CIDData *mIDData;
public:
	CIncludeFileTypesDlg(CIDData *ioIDData, CWnd* pParent = NULL);   // standard constructor
	virtual ~CIncludeFileTypesDlg();
	virtual int OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn);

	LPCTSTR GetFileTypes(){ return mFileTypes; }

// Dialog Data
	enum { IDD = IDD_INCLUDE_FILE_TYPES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	PARAM_FOR_CDB *m_pParamCDB;

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	CString mFileTypes;
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnBnClickedDeselectAll();
};
