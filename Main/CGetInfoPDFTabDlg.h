// CGetInfoPDFTabDlg.h: implementation file 

#ifndef _CGetInfoPDFTabDlg_H_
#define _CGetInfoPDFTabDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CBBTabPanel.h"
#include "COptionsDlg.h"
#include "CDB.h"
#include "CPDFPropertiesFrame.h"

// CGetInfoPDFTabDlg dialog
class CGetInfoPDFTabDlg : public CBBTabPanel
{
private:
	bool    ValidateAndProcess();
	CGetInfoData *mSelFilesData;
	CPDFPropertiesFrame* mFrame;
	PDF_Entry mEntry;
	CRect mRect;

public:
	CGetInfoPDFTabDlg(CGetInfoData *inSelFilesData, CWnd* pParent = NULL);
	~CGetInfoPDFTabDlg();
	void Update(int inFileIdx);
	CPDFPropertiesFrame* GetPDFPropertiesFrame(){return mFrame;}
	// Dialog Data
	enum { IDD = IDD_GET_INFO_PDF_TAB_DLG };

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

};

#endif
