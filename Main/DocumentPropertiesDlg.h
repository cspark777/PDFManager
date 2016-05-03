#pragma once

#include "DocPropertiesDesc.h"
#include "DocPropertiesSecurity.h"
#include "DocPropertiesFonts.h"
#include "DocPropertiesInitView.h"
#include "DocPropertiesCustom.h"

// CDocumentPropertiesDlg dialog

class CDocumentPropertiesDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDocumentPropertiesDlg)

public:
	CDocumentPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDocumentPropertiesDlg();

// Dialog Data
	enum { IDD = IDD_DOCUMENTPROPERTIES };

  String      pdfPath;
  String      cmd;
  //
  DebenuPDFLibraryAX0912 qpdf;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

  CBCGPTabWnd         _wndTab;

  CDocPropertiesDesc      _descDlg;
  CDocPropertiesSecurity  _securityDlg;
  CDocPropertiesFonts     _fontsDlg;
  CDocPropertiesInitView  _initViewDlg;
  CDocPropertiesCustom    _customDlg;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
};
