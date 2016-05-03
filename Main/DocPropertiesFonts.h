#pragma once


// CDocPropertiesFonts dialog

class CDocPropertiesFonts : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDocPropertiesFonts)

public:
	CDocPropertiesFonts(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_);   // standard constructor
	virtual ~CDocPropertiesFonts();

// Dialog Data
	enum { IDD = IDD_PROPERTIES_FONTS };

  DebenuPDFLibraryAX0912& qpdf;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  void loadFonts();

  CBCGPGridCtrl _fontsGrid;

	DECLARE_MESSAGE_MAP()
};
