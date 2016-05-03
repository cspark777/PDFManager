#pragma once


// CDocPropertiesCustom dialog

class CDocPropertiesCustom : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDocPropertiesCustom)

public:
	CDocPropertiesCustom(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_);   // standard constructor
	virtual ~CDocPropertiesCustom();

// Dialog Data
	enum { IDD = IDD_PROPERTIES_CUSTOM };

  DebenuPDFLibraryAX0912& qpdf;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  void loadProperties();
  void updateButtonsState();

  CBCGPListCtrl _propsList;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnLvnItemchangedFilesList(NMHDR *pNMHDR, LRESULT *pResult);
};
