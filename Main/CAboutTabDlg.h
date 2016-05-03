// CAboutTabDlg.h

#ifndef _CAboutTabDlg_H_
#define _CAboutTabDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CBBTabPanel.h"
#include "CVersion.h"
#include "resource.h"

class CAboutTabDlg : public CBBTabPanel
{
private:
	CFont m_BoldFont;
	CBCGPURLLinkButton m_DebenuURLBtn;

public:
	enum { IDD = IDD_ABOUT_TAB_ABOUT };
	CAboutTabDlg(CWnd* pParent = NULL);
private:
	bool ValidateAndProcess();	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnStnClickedAboutVersionStatic();
};

#endif
