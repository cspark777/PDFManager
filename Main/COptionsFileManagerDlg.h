// COptionsFileManagerDlg.h
//*****************************************************************************

#ifndef _COptionsFileManagerDlg_H_
#define _COptionsFileManagerDlg_H_

#include "stdafx.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif

#include "CDB.h"
//*****************************************************************************
// COptionsFileManagerDlg class
class COptionsFileManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsFileManagerDlg)

// Dialog data
public:
	enum { IDD = IDD_OPTIONS_FILE_MANAGER_DLG };

// Member variables
private:
	CBCGPEdit m_RepositoryLocationEdit;
	CBCGPEdit m_WorkspaceLocationEdit;
	CComboBox m_OnAddActionCombo;
	CComboBox m_OnChangeActionCombo;
	CString m_strRepositoryPath;
	CString m_strWorkspacePath;

// Public methods
public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
public:
	COptionsFileManagerDlg(CWnd* pParent = NULL);
	COptionsFileManagerDlg(PARAM_FOR_CDB *pParamCDB,CWnd* pParent = NULL);
	virtual ~COptionsFileManagerDlg();
	BOOL ValidateAndProcess();

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// Messages
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

#endif
