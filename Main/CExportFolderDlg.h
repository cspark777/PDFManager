#pragma once

#include "stdafx.h"
#include "Define.h"
#include "resource.h"

#include "CCollectionsComboBox.h"

// CExportFolderDlg dialog

class CExportFolderDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportFolderDlg)

public:
	CExportFolderDlg(int inCollectionID = FILTERTREE_MAJOR_BASE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportFolderDlg();

// Dialog Data
	enum { IDD = IDD_EXPORT_FOLDER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	void UpdateControls();

	CCollectionsComboBox m_CollectionsCombo;
	CBCGPEdit m_BrowseEdit;

	long	mCollectionID;
	CString mOutputFolder;

	PARAM_FOR_CDB *m_pParamCDB; 
	int m_nIsForLibrary;
	void SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary);

public:
	afx_msg void OnCbnSelchangeCollectionsCombo();
	afx_msg void OnEnUpdateEditOutput();
	afx_msg void OnBnClickedOk();
};
