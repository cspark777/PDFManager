#pragma once
#include "afxwin.h"


// CDocPropertiesSecurity dialog

class CDocPropertiesSecurity : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDocPropertiesSecurity)

public:
	CDocPropertiesSecurity(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_);   // standard constructor
	virtual ~CDocPropertiesSecurity();

// Dialog Data
	enum { IDD = IDD_PROPERTIES_SECURITY };

  DebenuPDFLibraryAX0912& qpdf;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  CString getSecurityInfo(int code);

  CBCGPComboBox _securityMethod;

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedChangesettings();
  afx_msg void OnCbnSelchangeSecuritymethod();
};
