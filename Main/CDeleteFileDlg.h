#pragma once
#include "stdafx.h"
#include "resource.h"

// CDeleteFileDlg dialog

enum
{
	CDeleteFileDlg_Cancel = 0,
	CDeleteFileDlg_KeepFile,
	CDeleteFileDlg_MoveToRecycle
};

class CDeleteFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeleteFileDlg)

public:
	CDeleteFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeleteFileDlg();

// Dialog Data
	enum { IDD = IDD_DELETE_FILE_DLG };
	
	int mAction;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMoveToRecycleBinBtn();
public:
	afx_msg void OnBnClickedKeepFileBtn();
};
