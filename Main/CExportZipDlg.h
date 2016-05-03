#pragma once

#include "stdafx.h"
#include "Define.h"
#include "CCollectionsComboBox.h"

// CExportZipDlg dialog

class CExportZipDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportZipDlg)

public:
	CExportZipDlg(int inCollectionID = FILTERTREE_MAJOR_BASE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportZipDlg();

// Dialog Data
	enum { IDD = IDD_EXPORT_ZIP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void UpdateControls();

	CCollectionsComboBox m_CollectionsCombo;
	CBCGPEdit m_BrowseEdit;
	CBCGPEdit mPasswordEdit;

	long	mCollectionID;
	CString mOutputZipFile;
	CString mPassword;

	PARAM_FOR_CDB *m_pParamCDB; 
	int m_nIsForLibrary;
	void SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary);
	
public:
	afx_msg void OnCbnSelchangeCollectionsCombo();
	afx_msg void OnBnClickedCheckEncrypt();
	afx_msg void OnEnUpdateEditOutput();
	afx_msg void OnBnClickedOk();
};
