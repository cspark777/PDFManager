// CLicenseTabDlg.h

#ifndef _CLicenseTabDlg_H_
#define _CLicenseTabDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CBBTabPanel.h"
#include "resource.h"

class CLicenseTabDlg : public CBBTabPanel
{
private:
	CFont m_BoldFont;
	CBCGPURLLinkButton m_PurchaseLicenseOnlineURLBtn;

private:
	bool    ValidateAndProcess();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	enum { IDD = IDD_ABOUT_TAB_LICENSE };
	CLicenseTabDlg(CWnd* pParent = NULL);
	void ChangeDialogStatus();
	
protected:
	

protected:
	afx_msg void OnPurchase();
	afx_msg void OnRegister();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAboutLicenseAgreementBtn();
public:
	afx_msg void OnBnClickedAboutEnterSerialNumberBtn();
};

#endif
