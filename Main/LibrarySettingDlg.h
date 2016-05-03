#pragma once
#include "afxwin.h"


// CLibrarySettingDlg dialog

struct LIBRARY_SETTING_INFO
{
	int nIsUseRepository;
	CString strRepositoryPath;
	CString strWorkspacePath;
	int nWorkspaceType;
	int nIsDefaultLibrary;
};

class CLibrarySettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLibrarySettingDlg)

public:
	CLibrarySettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLibrarySettingDlg();

// Dialog Data
	enum { IDD = IDD_LIBRARY_SETTING };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetInitVarible(LIBRARY_SETTING_INFO info);

	int     m_nIsUseRepository;
	CString m_StrRepositoryPath;
	CString m_StrWorkspacePath;
	int		m_nWorkspaceType;
	int		m_nIsDefaultLibrary;

	CButton m_btnDirectory;
	CButton m_chkRepository;
	CEdit m_txtRepositoryPath;
	afx_msg void OnBnClickedButtonLibrarysettingSave();
	afx_msg void OnBnClickedCheckLibrarysettingUserepository();
	afx_msg void OnBnClickedOk();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CEdit m_txtWorkspacePath;
	CComboBox m_cmbWorkspaceType;
	CButton m_chkDefault;
};
