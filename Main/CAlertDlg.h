// CAlertDlg.h : interface of the CAlertDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CAlertDlg_H__
#define __CAlertDlg_H__

#pragma once

#include "stdafx.h"
#include "afxwin.h"
#include "resource.h"
#include "Benubird.h"

enum kAlertType
{
	kAlertWarning,
	kAlertError,
	kAlertQuestion,
	kAlertInfo,
};

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg
/////////////////////////////////////////////////////////////////////////////
class CAlertDlg : public CDialog
{
	DECLARE_DYNAMIC(CAlertDlg)

public:
	CAlertDlg(int inIconType, LPCTSTR inMessageStr, LPCTSTR inBtn1Str, LPCTSTR inBtn2Str, LPCTSTR inBtn3Str, LPCTSTR inCheckBoxStr = NULL, LPCTSTR inCaptionStr = NULL, CWnd* pParent = NULL);
	virtual ~CAlertDlg();
	int mBtnPressed;
	BOOL    mChecked;

	enum { IDD = IDD_ALERT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_IconCtrl;
	CStatic m_MessageCtrl;
	CButton m_ChecboxCtrl;
	CButton m_Btn1Ctrl;
	CButton m_Btn2Ctrl;
	CButton m_Btn3Ctrl;
	CButton m_CheckCtrl;
	CString mMessageStr;
	CString mBtn1Str;
	CString mBtn2Str;
	CString mBtn3Str;
	CString mCaptionStr;
	CString mCheckBoxStr;
	int		mIconType;

public:
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();
};

int Alert(int inIconType, LPCTSTR inMessageStr, LPCTSTR inBtn1Str, LPCTSTR inBtn2Str, LPCTSTR inBtn3Str, BOOL& outCheckBox, LPCTSTR inCheckBoxStr = NULL, LPCTSTR inCaptionStr = NULL);
int Alert(int inIconType, LPCTSTR inMessageStr, LPCTSTR inBtn1Str, LPCTSTR inBtn2Str = NULL, LPCTSTR inBtn3Str = NULL, LPCTSTR inCaptionStr = NULL);
int Alert(LPCTSTR inMessageStr);

#endif