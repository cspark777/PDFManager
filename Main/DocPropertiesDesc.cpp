// DocPropertiesDesc.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "DebenuPDFLibraryAX0912.h"
#include "DocPropertiesDesc.h"
#include "PDFUtils.h"

// CDocPropertiesDesc dialog

IMPLEMENT_DYNAMIC(CDocPropertiesDesc, CDialog)

CDocPropertiesDesc::CDocPropertiesDesc(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_)
: CDialog(CDocPropertiesDesc::IDD, pParent)
, qpdf   (qpdf_)
{
}

CDocPropertiesDesc::~CDocPropertiesDesc()
{
}

BOOL CDocPropertiesDesc::OnInitDialog()
{
  CDialog::OnInitDialog();

  LoadInfo();

  return TRUE;
}

BOOL CDocPropertiesDesc::LoadInfo()
{
  long rtn = 0; //general return values
//  SetDlgItemText(IDC_FILENAME, pdfPath.c_str());

  //BSTR metadata;
  String metadata;

  // PDF Version
  metadata = qpdf.GetInformation(0);
  if (!metadata.empty())
  {
    m_strPDFVersion = metadata;
    SetDlgItemText(IDC_PDF_VERSION, m_strPDFVersion.c_str());
  }

  // Author
  metadata = qpdf.GetInformation(1);
  if (!metadata.empty())
  {
    m_strAuthor = metadata;
    SetDlgItemText(IDC_AUTHOR,  m_strAuthor.c_str());
  }

  // Title
  metadata = qpdf.GetInformation(2);
  if (!metadata.empty())
  {
    m_strTitle = metadata;
    SetDlgItemText(IDC_TITLE,  m_strTitle.c_str());
  }

  // Subject
  metadata = qpdf.GetInformation(3);
  if (!metadata.empty())
  {
    m_strSubject = metadata;
    SetDlgItemText(IDC_SUBJECT,  m_strSubject.c_str());
  }

  // Keywords
  metadata = qpdf.GetInformation(4);
  if (!metadata.empty())
  {	
    m_strKeywords = metadata;
    SetDlgItemText(IDC_KEYWORDS,  m_strKeywords.c_str());
  }

  // Creator
  metadata = qpdf.GetInformation(5);
  if (!metadata.empty())
  {	
    m_strCreator = metadata;
    SetDlgItemText(IDC_CREATOR, m_strCreator.c_str());
  }

  // Producer
  metadata = qpdf.GetInformation(6);
  if (!metadata.empty())
  {
    m_strProducer = metadata;
    SetDlgItemText(IDC_PRODUCER, m_strProducer.c_str());
  }

  // Creation date
  metadata = qpdf.GetInformation(7);
  if (!metadata.empty())
  {
    m_strCreated = metadata;
    m_strCreated = DecodeDate(m_strCreated.c_str()).GetString();
    SetDlgItemText(IDC_CREATED, m_strCreated.c_str());
  }

  // Modification date
  metadata = qpdf.GetInformation(8);
  if (!metadata.empty())
  {
    m_strModified = metadata;
    m_strModified = DecodeDate(m_strModified.c_str()).GetString();
    SetDlgItemText(IDC_MODIFIED, m_strModified.c_str());
  }


  // Number of pages
  CString strPageCount;
  long lPageCount = 0;
  lPageCount = qpdf.PageCount();
  if (lPageCount)
  {
	strPageCount.Format(_T("%d"), lPageCount);
	m_strPageCount = strPageCount.GetBuffer();
    SetDlgItemText(IDC_PAGECOUNT, m_strPageCount.c_str());
  }

  // Fast web view
  long lLinearized = 0;
  lLinearized = qpdf.Linearized();
  if(lLinearized==1)
  {
    m_strFastWeb = L"Yes";
  }
  else
  {
    m_strFastWeb = L"No";
  }
  SetDlgItemText(IDC_FASTWEB, m_strFastWeb.c_str());

  return TRUE;
}


BOOL CDocPropertiesDesc::SaveInfo(void)
{
  long rtn = 0;//general return values

  // Retrieve data from dialog
  CString text;

  GetDlgItemText(IDC_TITLE, text);
  m_strTitle = text.GetString();
  GetDlgItemText(IDC_AUTHOR, text);
  m_strAuthor = text.GetString();
  GetDlgItemText(IDC_SUBJECT, text);
  m_strSubject = text.GetString();
  GetDlgItemText(IDC_KEYWORDS, text);
  m_strKeywords = text.GetString();

  // Update document
  rtn = qpdf.SetInformation(1, m_strAuthor);
  rtn = qpdf.SetInformation(2, m_strTitle);
  rtn = qpdf.SetInformation(3, m_strSubject);
  rtn = qpdf.SetInformation(4, m_strKeywords);

  return 0;
}
