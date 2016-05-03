#pragma once

#include "stdafx.h"

// CGetTextDlg dialog

class CGetTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CGetTextDlg)

	CString mEnteredText;
	LPCTSTR mCaptionStr;

public:
	CGetTextDlg(LPCTSTR inCaptionStr, LPCTSTR inText = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetTextDlg();
	LPCTSTR GetEnteredText(){ return mEnteredText.IsEmpty() ? _T("") : mEnteredText; };
	
protected:
	virtual BOOL OnInitDialog();
	
// Dialog Data
	enum { IDD = IDD_GET_TEXT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedOk();
};
