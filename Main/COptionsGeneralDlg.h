// COptionsGeneralDlg.h
//*****************************************************************************

#ifndef _COptionsGeneralDlg_H_
#define _COptionsGeneralDlg_H_

#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif

//*****************************************************************************
// COptionsGeneralDlg class
class COptionsGeneralDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsGeneralDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_GENERAL_DLG };

// Member variables
private:
	CButton m_LoadOnStartupCheck;
	CButton m_ShowIconInSystemTrayCheck;
	CButton m_MinimizeToSystmTrayCheck;
	CButton m_IgnoreTmpFilesCheck;
	CButton m_IgnoreWinFilesCheck;
	CButton m_IgnoreLnkFilesCheck;
	CButton m_IgnoreNoExtFilesCheck;
	CButton m_UseRepositoryCheck;
	CButton m_IgnoreSVNFilesCheck;

// Public methods
public:
	COptionsGeneralDlg(CWnd* pParent = NULL);
	virtual ~COptionsGeneralDlg();
	BOOL ValidateAndProcess();

// Private methods
private:
	BOOL AddToStartup();
	BOOL RemoveFromStartup();
	BOOL IsInStartup();

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Messages
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOptGeneralShowIconInSystemTrayCheck();
	afx_msg void OnBnClickedOptGeneralRepositoryCheck();
	
};

#endif