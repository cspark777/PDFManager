// DocPropertiesSecurity.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "PDFDoc.h"
#include "MainFrm.h"
#include "DebenuPDFLibraryAX0912.h"
#include "DocPropertiesSecurity.h"
#include "PasswordSettingsDlg.h"
#include "RequestPasswordDlg.h"
#include "PDFUtils.h"
#include "Define.h"

// CDocPropertiesSecurity dialog

IMPLEMENT_DYNAMIC(CDocPropertiesSecurity, CBCGPDialog)

CDocPropertiesSecurity::CDocPropertiesSecurity(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_)
: CBCGPDialog(CDocPropertiesSecurity::IDD, pParent)
, qpdf   (qpdf_)
{
}

CDocPropertiesSecurity::~CDocPropertiesSecurity()
{
}

void CDocPropertiesSecurity::DoDataExchange(CDataExchange* pDX)
{
  CBCGPDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SECURITYMETHOD, _securityMethod);
}

BOOL CDocPropertiesSecurity::OnInitDialog()
{
	int nError = 0;
  CBCGPDialog::OnInitDialog();

    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, pChild->GetActiveDocument ());

  _securityMethod.InsertString(0, _T("No Security"));
  _securityMethod.InsertString(1, _T("Password Security"));

  _securityMethod.SetCurSel(pDoc->password.empty() ? 0 : 1);
  OnCbnSelchangeSecuritymethod();

  {
    String fname = qpdf.GetDocumentFileName();
	/*
    DebenuPDFLibraryAX0912 qpdfInfo;
	nError = qpdfInfo.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	nError = qpdfInfo.LastErrorCode();
	*/
    //int analyseID = qpdfInfo.AnalyseFile(fname, pDoc->password);
	int analyseID = qpdf.AnalyseFile(fname, pDoc->password);

	nError = qpdf.LastErrorCode();
    SetDlgItemText(IDC_STATIC_PRINTING, pdfSecurityInfoStr(qpdf, analyseID, 3).c_str());
    SetDlgItemText(IDC_STATIC_CHANGING, pdfSecurityInfoStr(qpdf, analyseID, 4).c_str());
    SetDlgItemText(IDC_STATIC_ACC, pdfSecurityInfoStr(qpdf, analyseID, 9).c_str());
    SetDlgItemText(IDC_STATIC_COPYING, pdfSecurityInfoStr(qpdf, analyseID, 5).c_str());
    SetDlgItemText(IDC_STATIC_COPYING_ACC, pdfSecurityInfoStr(qpdf, analyseID, 8).c_str());
    SetDlgItemText(IDC_STATIC_PAGEEXTR, pdfSecurityInfoStr(qpdf, analyseID, 5).c_str());
    SetDlgItemText(IDC_STATIC_COMMENTING, pdfSecurityInfoStr(qpdf, analyseID, 6).c_str());
    SetDlgItemText(IDC_STATIC_FORMFILLING, pdfSecurityInfoStr(qpdf, analyseID, 6).c_str());
    SetDlgItemText(IDC_STATIC_SIGNING, pdfSecurityInfoStr(qpdf, analyseID, 7).c_str());
  }

  return TRUE;
}

BEGIN_MESSAGE_MAP(CDocPropertiesSecurity, CBCGPDialog)
  ON_BN_CLICKED(IDC_CHANGESETTINGS, &CDocPropertiesSecurity::OnBnClickedChangesettings)
  ON_CBN_SELCHANGE(IDC_SECURITYMETHOD, &CDocPropertiesSecurity::OnCbnSelchangeSecuritymethod)
END_MESSAGE_MAP()

void CDocPropertiesSecurity::OnBnClickedChangesettings()
{
    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, pChild->GetActiveDocument ());

  if (pDoc->hasOwnerPassword && pDoc->passwordType != PT_OWNER)
  {
    CRequestPasswordDlg passDlg;
    passDlg.pdfPath = pDoc->pdfPath;
    if (passDlg.DoModal() == IDCANCEL)
      return;

    DebenuPDFLibraryAX0912 qpdf;
	qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));

    int    analyseID = qpdf.AnalyseFile(pDoc->pdfPath, passDlg.password);
    String passType = qpdf.GetAnalysisInfo(analyseID, 13);

    _wcslwr_s(&*passType.begin(), passType.length() + 1);
    if (passType != _T("owner"))
    {
      AfxMessageBox(_T("Password is incorrect."), MB_OK | MB_ICONEXCLAMATION);
      return;
    }
    pDoc->password = passDlg.password;
    pDoc->passwordType = PT_OWNER;
    pDoc->hasOwnerPassword = true;
    if (qpdf.LoadFromFile(pDoc->pdfPath, pDoc->password) != 1)
    {
      CString strMessage;
      strMessage.Format(L"Error accessing %s!", pDoc->pdfPath.c_str());
      AfxMessageBox(strMessage, MB_ICONSTOP);
      return;
    }
  }

  CPasswordSettingsDlg passSettingsDlg(NULL, qpdf);

  passSettingsDlg.DoModal();
}

void CDocPropertiesSecurity::OnCbnSelchangeSecuritymethod()
{
  GetDlgItem(IDC_CHANGESETTINGS)->EnableWindow(TRUE);
}
