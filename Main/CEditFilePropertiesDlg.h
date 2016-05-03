#pragma once

#include "stdafx.h"
#include "CImportUtils.h"
#include "afxwin.h"
#include "CCollectionsComboBox.h"


// CEditFilePropertiesDlg dialog

class CEditFilePropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditFilePropertiesDlg)

public:
	CEditFilePropertiesDlg(CFileProperties_Entry * inFileInfo, CIDData* inIDDataTags, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditFilePropertiesDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SET_FILE_PROPERTIES };

	CIDData* mIDDataTags;
	CIDData  mIDDataTagsOrig;
	CFileProperties_Entry* mFileInfo;
	bool mOwnUserInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedWfEditTagsBtn();
	afx_msg void OnBnClickedWfTitleCheck();
	afx_msg void OnBnClickedWfAuthorCheck();
	afx_msg void OnBnClickedWfSubjectCheck();
	afx_msg void OnBnClickedWfCatCheck();
	afx_msg void OnBnClickedWfTagsCheck();
	afx_msg void OnBnClickedWfCommentsCheck();
	afx_msg void OnBnClickedCancel();
};
