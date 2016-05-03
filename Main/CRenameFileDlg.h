#pragma once
#include "stdafx.h"
#include "Path.h"
#include "CDB.h"
///////////////////////////////////////////////////////////////////////////////////
// CRenameFileDlg dialog
///////////////////////////////////////////////////////////////////////////////////
class CRenameFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameFileDlg)

protected:
	CString mFileName;
	int mFileID;

	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	PARAM_FOR_CDB *m_pParamCDB;
	CDB *m_pDB;
public:

	CRenameFileDlg(CString inFileName, int inFileID);
	
	virtual ~CRenameFileDlg();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_RENAME_FILE_DLG };
	afx_msg void OnBnClickedOk();
};
