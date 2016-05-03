#pragma once

#include "stdafx.h"
// CSplashScreenDlg dialog

class CSplashScreenDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplashScreenDlg)

public:
	CSplashScreenDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashScreenDlg();

// Dialog Data
	enum { IDD = IDD_SPLASH_SCREEN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSsBuyNowBtn();
public:
	afx_msg void OnBnClickedSsEnterSerialBtn();
public:
	afx_msg void OnBnClickedSsTryNowBtn();
public:
	virtual BOOL OnInitDialog();
};
