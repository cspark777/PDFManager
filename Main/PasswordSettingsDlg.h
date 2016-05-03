#pragma once


// CPasswordSettingsDlg dialog

class CPasswordSettingsDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CPasswordSettingsDlg)

public:
	CPasswordSettingsDlg(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_);   // standard constructor
	virtual ~CPasswordSettingsDlg();

// Dialog Data
	enum { IDD = IDD_SECURITY_PASSWORD };

  DebenuPDFLibraryAX0912& qpdf;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

  void updateState();

  CBCGPComboBox _encryption;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedPassOpenDoc();
  afx_msg void OnBnClickedPassChangeDoc();
  afx_msg void OnBnClickedOk();
};
