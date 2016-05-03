#pragma once
#include "stdafx.h"
#include "afxwin.h"


// CWatchedFolderSettingsDlg dialog

class CWatchedFolderSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CWatchedFolderSettingsDlg)
	CFont mBoldFont;
	CString mPath;
	DWORD_PTR mWFolderID; //the watched folder ID
	CIDData mSelectedExtensions;
	CIDData mSelectedTags;

public:
	PARAM_FOR_CDB *m_pParamCDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);
public:
	CWatchedFolderSettingsDlg(DWORD_PTR inWFolderID, CWnd* pParent = NULL);   // standard constructor
	virtual ~CWatchedFolderSettingsDlg();
	LPCTSTR GetPath(){ return mPath; }

// Dialog Data
	enum { IDD = IDD_WATCHED_FOLDER_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWfEditTagsBtn();
	afx_msg void OnBnClickedWfEditCategoriesBtn();
	afx_msg void OnBnClickedWfChooseFileTypesBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWfsBrowseBtn();
	afx_msg void OnBnClickedWfAllFileTypesRadio();
	CButton m_ChooseFileTypeBtn;
	afx_msg void OnBnClickedWfOnlyTheseFileTypesRadio();
	afx_msg void OnBnClickedOk();
	CComboBox mCollectionCombo;
	CButton m_InclSubfoldersCheckBtn;
	CComboBox m_FrequencyCombo;
	afx_msg void OnEnChangeEdit7();
public:
	afx_msg void OnBnClickedWfTitleCheck();
public:
	afx_msg void OnBnClickedWfAuthorCheck();
public: 
	afx_msg void OnBnClickedWfSubjectCheck();
public:
	afx_msg void OnBnClickedWfCatCheck();
public:
	afx_msg void OnBnClickedWfTagsCheck();
public:
	afx_msg void OnBnClickedWfCommentsCheck();
};
