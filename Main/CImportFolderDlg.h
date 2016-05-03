#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "CCollectionsComboBox.h"

// CImportFolderDlg dialog

class CImportFolderDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportFolderDlg)

public:
	CImportFolderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportFolderDlg();

// Dialog Data
	enum { IDD = IDD_IMPORT_FROM_FOLDER_DLG };

	void UpdateControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString mPath;
	bool mIncludeSubfolders;
	CFileProperties_Entry mFolderImportProperties;
	CIDData mIDDataTags;
	
	CBCGPEdit m_BrowseEdit;
	afx_msg void OnEnChangeEditInput();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioAllFiles();
	afx_msg void OnBnClickedRadioSomeFiles();
	afx_msg void OnBnClickedButtonOptions();
	afx_msg void OnBnClickedButtonFileTypes();
	CCollectionsComboBox m_CollectionCombo;

	PARAM_FOR_CDB *m_pParamCDB; 
	int m_nIsForLibrary;
	void SetData_FromParent(PARAM_FOR_CDB *pParamCDB,int nIsForLibrary);
};
