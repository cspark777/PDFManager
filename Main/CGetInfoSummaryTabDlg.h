// CGetInfoSummaryTabDlg.h: implementation file 

#ifndef _CGetInfoSummaryTabDlg_H_
#define _CGetInfoSummaryTabDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CBBTabPanel.h"
//#include "afxwin.h"
#include "COptionsDlg.h"
#include "afxwin.h"
#include "CDB.h"

// Struct for custom values


// CGetInfoSummaryTabDlg dialog
class CGetInfoSummaryTabDlg : public CBBTabPanel
{
private:

private:
	bool    ValidateAndProcess();
	int mFileID;
	CGetInfoData *mSelFilesData;

public:
	CGetInfoSummaryTabDlg(CGetInfoData *inSelFilesData, CWnd* pParent = NULL);
	~CGetInfoSummaryTabDlg();
	void Update(int inFileID);

	long GetSizeOnDisk(LPCTSTR inPath);
	long GetFileSize(LPCTSTR inPath);

	// Dialog Data
	enum { IDD = IDD_GET_INFO_SUMMARY_TAB_DLG };

	// Get data
		
	// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedInfoChangeOpenWithBtn();
public:
	CStatic m_SizeStatic;
public:
	CStatic m_SizeOnDiskStatic;
public:
	CStatic m_CreatedStatic;
public:
	CStatic m_ModifiedStatic;
public:
	CStatic m_AccessedStatic;
public:
	CStatic m_OpensWithStatic;
public:
	CStatic m_FileTypeStatic;
};

#endif
