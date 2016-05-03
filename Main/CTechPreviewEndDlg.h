#pragma once


// CTechPreviewEndDlg dialog

class CTechPreviewEndDlg : public CDialog
{
	DECLARE_DYNAMIC(CTechPreviewEndDlg)

public:
	CTechPreviewEndDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTechPreviewEndDlg();

// Dialog Data
	enum { IDD = IDD_TECH_PREVIEW_END_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
