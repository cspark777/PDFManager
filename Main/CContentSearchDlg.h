#pragma once

#include "stdafx.h"
#include "Define.h"
#include "resource.h"

#include "CCollectionsComboBox.h"

// CContentSearchDlg dialog

class CContentSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CContentSearchDlg)

public:
	CContentSearchDlg(int inCollectionID = FILTERTREE_MAJOR_BASE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CContentSearchDlg();

// Dialog Data
	enum { IDD = IDD_CONTENT_SEARCH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	void UpdateControls();

	CCollectionsComboBox m_CollectionsCombo;
	CBCGPEdit m_BrowseEdit;
	CBCGPEdit m_Keyword;

	long	mCollectionID;
	CString mKeyword;
	CString mOutFolder;

	PARAM_FOR_CDB *m_pParamCDB; 
	int m_nIsForLibrary;
	void SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary);

public:
	afx_msg void OnCbnSelchangeCollectionsCombo();
	afx_msg void OnEnUpdateEditOutput();
	afx_msg void OnEnUpdateEditWord();
	afx_msg void OnBnClickedOk();
};
