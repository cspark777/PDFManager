// CGetInfoInfoTabDlg.h: implementation file 

#ifndef _CGetInfoInfoTabDlg_H_
#define _CGetInfoInfoTabDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CBBTabPanel.h"
//#include "afxwin.h"
#include "COptionsDlg.h"
#include "CDB.h"

// Struct for custom values


// CGetInfoInfoTabDlg dialog
class CGetInfoInfoTabDlg : public CBBTabPanel
{
private:

private:
	bool    ValidateAndProcess();
	CGetInfoData *mSelFilesData;
public:
	CGetInfoInfoTabDlg(CGetInfoData *inSelFilesData, CWnd* pParent = NULL);
	~CGetInfoInfoTabDlg();
	void Update(int inFileIdx);

	// Dialog Data
	enum { IDD = IDD_GET_INFO_INFO_TAB_DLG };

	// Get data
		
	// Overrides
protected:
	int mFileIdx;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedInfoEditCategoriesBtn();
public:
	afx_msg void OnBnClickedInfoEditTagsBtn();
public:
	afx_msg void OnBnClickedInfAdditionalMetadataBtn();
};

#endif
