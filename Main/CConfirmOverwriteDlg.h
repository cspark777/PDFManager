#pragma once

#include "stdafx.h"

// CConfirmOverwriteDlg dialog

enum 
{
	kConfirmUnknown = 0,
	kConfirmOverwrite,
	kConfirmOverwriteOlder,
	kConfirmRename,
	kConfirmAutoRename,
	kConfirmSkip,
	kConfirmCancel
};

class CConfirmOverwriteDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfirmOverwriteDlg)

public:
	CConfirmOverwriteDlg(LPCTSTR inFilePath, LPCTSTR inOverwritePath, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfirmOverwriteDlg();

// Dialog Data
	enum { IDD = IDD_CONFIRM_OVERWRITE };

	int GetResultAction(bool & outApplyToAll);
	CString GetDestFilePath() {return m_OverwritePath;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	void ReadFileInfo(bool destFile);

private:
	CString m_FilePath;
	CString m_OverwritePath;

	int m_ResultAction;
	bool m_ApplyToAll;

	afx_msg void OnBnClickedButtonReplace();
	afx_msg void OnBnClickedButtonReplaceOlder();
	afx_msg void OnBnClickedButtonSkip();
	afx_msg void OnBnClickedButtonRename();
	afx_msg void OnBnClickedButtonAutoRename();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckApplyToAll();
};
