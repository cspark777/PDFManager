#pragma once

#include "stdafx.h"
#include "afxwin.h"
#include "CCollectionsComboBox.h"

// CImportZipDlg dialog

class CImportZipDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportZipDlg)

public:
	CImportZipDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportZipDlg();

// Dialog Data
	enum { IDD = IDD_IMPORT_FROM_ZIP_DLG };

	void UpdateControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString m_Password;
	CString m_SourcePath;
	CFileProperties_Entry mZipImportProperties;
	CIDData mIDDataTags;

	PARAM_FOR_CDB *m_pParamCDB;
	int m_nIsForLibrary;
	void SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary=0);


	CBCGPEdit m_BrowseEdit;
	CButton m_PasswordCheck;
	CEdit m_PasswordEdit;
	afx_msg void OnBnClickedCheckPassword();
	afx_msg void OnEnChangeEditInput();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioAllFiles();
	afx_msg void OnBnClickedRadioSomeFiles();
	afx_msg void OnBnClickedButtonFileTypes();
	afx_msg void OnBnClickedButtonOptions();

	CCollectionsComboBox m_CollectionCombo;

};
