// CBrowseFolderDlg.h
//*****************************************************************************

#ifndef _CBrowseFolderDlg_H_
#define _CBrowseFolderDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "stdafx.h"

//*****************************************************************************
// CBrowseFolderDlg class
class CBrowseFolderDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrowseFolderDlg)

// Dialog Data
public:
	enum { IDD = IDD_BROWSE_FOLDER_DLG };

// Member variables
private:
	CString mPath;
	bool mIncludeSubfolders;

	CBCGPShellTree m_ShellTreeCtrl;
	CButton m_IncludeSubfoldersCheckCtrl;
	BOOL mShowCheck;

// Public methods
public:
	CBrowseFolderDlg(BOOL inShowCheck, CWnd* pParent = NULL);
	virtual ~CBrowseFolderDlg();
	bool AreSubfoldersIncluded(){ return mIncludeSubfolders; }
	LPCTSTR GetPath(){ return mPath; }

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Messages
protected:
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
};

#endif