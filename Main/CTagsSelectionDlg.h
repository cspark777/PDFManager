#pragma once

#include "stdafx.h"
#include "afxwin.h"
#include "CDB.h"
#include "Benubird.h"

// CTagsSelectionDlg dialog	

class CTagsSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CTagsSelectionDlg)

public:
	PARAM_FOR_CDB *m_pParamCDB;
public:
//	CTagsSelectionDlg(int inFileID, int inTableID = TABLEID_FILES, CWnd* pParent = NULL);
	CTagsSelectionDlg(CIDData *inIDData, CWnd* pParent = NULL); 
	virtual ~CTagsSelectionDlg();

// Dialog Data
	enum { IDD = IDD_TAGS_SELECTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int mMainEntryID; //the idFile or idWFolder
	int mTableID;

	CIDData *mIDData;
	DECLARE_MESSAGE_MAP()

	BOOL InitControls(bool inKeepSelectedTagsList  = false);
public:
	afx_msg void OnBnClickedTsManageTagsBtn();
public:
	virtual BOOL OnInitDialog();
public:
	CListBox mAvailableTags;
public:
	CListBox mSelectedTags;
public:
	afx_msg void OnBnClickedTsAddBtn();
public:
	afx_msg void OnBnClickedTsRemoveBtn();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnLbnDblclkTsAvailableTagsList();
public:
	afx_msg void OnLbnDblclkTsSelectedTagsList();
};
