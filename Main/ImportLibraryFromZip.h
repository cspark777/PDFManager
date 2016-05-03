#pragma once


// CImportLibraryFromZip dialog

class CImportLibraryFromZip : public CDialogEx
{
	DECLARE_DYNAMIC(CImportLibraryFromZip)

public:
	CImportLibraryFromZip(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportLibraryFromZip();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_IMPORTLIBRARY_FROM_ZIP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CString m_StrPassword;
	CString m_StrDestDirectory;
	CString m_StrZipFilePath;

	CButton m_chkIsPassword;
	CEdit m_txtDestDirectory;
	CBCGPEdit m_txtZipFilePath;
	CEdit m_txtPassword;
	afx_msg void OnBnClickedCheckPassword();
	afx_msg void OnBnClickedOk();
	CButton m_btnBrowser;
	afx_msg void OnBnClickedButtonBrowser();
};
