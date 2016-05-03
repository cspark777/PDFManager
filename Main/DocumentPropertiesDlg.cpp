// DocumentPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Define.h"
#include "Benubird.h"
#include "PDFDoc.h"
#include "MainFrm.h"
#include "DebenuPDFLibraryAX0912.h"
#include "DocumentPropertiesDlg.h"
#include "CommandArgs.h"

// CDocumentPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CDocumentPropertiesDlg, CBCGPDialog)

CDocumentPropertiesDlg::CDocumentPropertiesDlg(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDocumentPropertiesDlg::IDD, pParent)
, _descDlg    (NULL, qpdf)
, _securityDlg(NULL, qpdf)
, _fontsDlg   (NULL, qpdf)
, _initViewDlg(NULL, qpdf)
, _customDlg  (NULL, qpdf)
{
	qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
}

CDocumentPropertiesDlg::~CDocumentPropertiesDlg()
{
}

void CDocumentPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}
//
BOOL CDocumentPropertiesDlg::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

//  SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_PDFPREVIEWICON)), TRUE);
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, pChild->GetActiveDocument ());

  SetDlgItemText(IDC_PDFFILE, pdfPath.c_str());

  CRect rect;
  GetDlgItem(IDC_STATICTAB)->GetWindowRect(&rect);
  ScreenToClient(&rect);
  GetDlgItem(IDC_STATICTAB)->ShowWindow(FALSE);

  _wndTab.Create(CBCGPTabWnd::STYLE_3D_ROUNDED, rect, this, 1, CBCGPTabWnd::LOCATION_TOP);

  qpdf.LoadFromFile(pdfPath, pDoc->password);
  
  _descDlg.pdfPath = pdfPath.c_str();
  _descDlg.Create(CDocPropertiesDesc::IDD, &_wndTab);
  _wndTab.AddTab(&_descDlg, _T("Description"));
  _securityDlg.Create(CDocPropertiesSecurity::IDD, &_wndTab);
  _wndTab.AddTab(&_securityDlg, _T("Security"));
  _fontsDlg.Create(CDocPropertiesFonts::IDD, &_wndTab);
  _wndTab.AddTab(&_fontsDlg, _T("Fonts"));
  _initViewDlg.Create(CDocPropertiesInitView::IDD, &_wndTab);
  _wndTab.AddTab(&_initViewDlg, _T("Initial View"));
  _customDlg.Create(CDocPropertiesCustom::IDD, &_wndTab);
  _wndTab.AddTab(&_customDlg, _T("Custom"));

  int tab;

  if (cmd == CMD_DOCUMENTSEC)
  {
    tab = 1;
  }
  else if (cmd == CMD_DOCUMENTFONTS)
  {
    tab = 2;
  }
  else if (cmd == CMD_DOCUMENTINITV)
  {
    tab = 3;
  }
  else if (cmd == CMD_DOCUMENTCUSTOM)
  {
    tab = 4;
  }
  else
  {
    tab = 0; // Description
  }

  _wndTab.SetActiveTab(tab);

  return TRUE;
}

BEGIN_MESSAGE_MAP(CDocumentPropertiesDlg, CBCGPDialog)
  ON_BN_CLICKED(IDOK, &CDocumentPropertiesDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CDocumentPropertiesDlg::OnBnClickedOk()
{
  _descDlg.SaveInfo();
  _initViewDlg.SaveInfo();

  int nRet = qpdf.SaveToFile(pdfPath);



  OnOK();
}
