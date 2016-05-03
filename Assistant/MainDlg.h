#pragma once

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////////
// CAboutDlg class
///////////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////
// CBenubirdAssistantDlg class
///////////////////////////////////////////////////////////////////////////////////
class CBenubirdAssistantDlg : public CDialog
{
public:
	CBenubirdAssistantDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_BENUBIRDASSISTANT_DIALOG };

public:
	DWORD AllowMsg(UINT msg);

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnAssistantStop( WPARAM, LPARAM );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
