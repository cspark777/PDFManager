#pragma once


// CRequestPasswordDlg dialog

class CRequestPasswordDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CRequestPasswordDlg)

public:
	CRequestPasswordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRequestPasswordDlg();

// Dialog Data
	enum { IDD = IDD_REQUEST_PASSWORD };

  String pdfPath;
  String password;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
};
