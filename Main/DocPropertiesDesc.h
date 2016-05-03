#pragma once


// CDocPropertiesDesc dialog

class CDocPropertiesDesc : public CDialog
{
	DECLARE_DYNAMIC(CDocPropertiesDesc)

public:
	CDocPropertiesDesc(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_);   // standard constructor
	virtual ~CDocPropertiesDesc();

// Dialog Data
	enum { IDD = IDD_PROPERTIES_DESC };

  String       pdfPath;
  DebenuPDFLibraryAX0912& qpdf;

  BOOL LoadInfo();
  BOOL SaveInfo(void);
protected:
  virtual BOOL OnInitDialog();

  String m_strTitle;
  String m_strAuthor;
  String m_strSubject;
  String m_strKeywords;

  String m_strCreated;
  String m_strModified;
  String m_strProducer;
  String m_strCreator;
  String m_strPDFVersion;
  String m_strPageCount;
  String m_strFastWeb;
};
