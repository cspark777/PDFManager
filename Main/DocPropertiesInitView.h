#pragma once


// CDocPropertiesInitView dialog

class CDocPropertiesInitView : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDocPropertiesInitView)

public:
	CDocPropertiesInitView(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_);   // standard constructor
	virtual ~CDocPropertiesInitView();

// Dialog Data
	enum { IDD = IDD_PROPERTIES_INITIAL_VIEW };

  DebenuPDFLibraryAX0912& qpdf;

  BOOL LoadInfo(void);
  BOOL SaveInfo(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

  CComboBox m_cmbNavigationTab;
  CComboBox m_cmbPageLayout;
  CString m_strOpentoPage;
  CString m_strTotalPages;

  CEdit m_edOpentoPage;
  CStatic m_sTotalPages;
  CComboBox m_cmbMagnification;
  CButton m_bHideToolbar;
  CButton m_bHideMenuBar;
  CButton m_bHideWindowUI;
  CButton m_bFitWindow;
  CButton m_bCenterWindow;
  CButton m_bFullScreen;
  CComboBox m_cmbDocumentTitleOrFileName;
};
