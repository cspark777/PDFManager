#pragma once

#include "CDB.h"
// CMultipleProfileInformationDlg dialog

class CMultipleProfileInformationDlg : public CDialog
{
	DECLARE_DYNAMIC(CMultipleProfileInformationDlg)

public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
	void Set_ParamForCDB(PARAM_FOR_CDB *pParamCDB);
public:
	CMultipleProfileInformationDlg(CIntArray *inFileIDsArray, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMultipleProfileInformationDlg();

// Dialog Data
	enum { IDD = IDD_MULTIPLE_PROFILE_INFORMATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CIntArray	*mFileIDs;
	CIDData		mIDDataTags;
public:
	afx_msg void OnBnClickedMpiTitleCheck();
public:
	afx_msg void OnBnClickedMpiAuthorCheck();
public:
	afx_msg void OnBnClickedMpiSubjectCheck();
public:
	afx_msg void OnBnClickedMpiCategoryCheck();
public:
	afx_msg void OnBnClickedMpiTagsCheck();
public:
	afx_msg void OnBnClickedMpiEditTagsBtn();
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedMpiCommentsCheck();
};
