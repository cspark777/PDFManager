// PasswordSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "MainFrm.h"
#include "PDFDoc.h"
#include "DebenuPDFLibraryAX0912.h"
#include "PasswordSettingsDlg.h"


// CPasswordSettingsDlg dialog

IMPLEMENT_DYNAMIC(CPasswordSettingsDlg, CBCGPDialog)

CPasswordSettingsDlg::CPasswordSettingsDlg(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_)
: CBCGPDialog(CPasswordSettingsDlg::IDD, pParent)
, qpdf       (qpdf_)
{
}

CPasswordSettingsDlg::~CPasswordSettingsDlg()
{
}

void CPasswordSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_ENCRYPTIONLEVEL, _encryption);
}

BOOL CPasswordSettingsDlg::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

    CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, pChild->GetActiveDocument ());

  CheckDlgButton(IDC_PASS_OPEN_DOC, !pDoc->password.empty() ? BST_CHECKED : BST_UNCHECKED);
  SetDlgItemText(IDC_PASS_OPEN1, pDoc->password.c_str());
  SetDlgItemText(IDC_PASS_OPEN2, pDoc->password.c_str());

  _encryption.InsertString(0, _T("40-bit RC4 (Adobe Reader 3 and above)"));
  _encryption.InsertString(1, _T("128-bit RC4 (Adobe Reader 6 and above)"));
  _encryption.InsertString(2, _T("128-bit AES (Adobe Reader 7 and above)"));

  int encLevel = qpdf.SecurityInfo(10);
  int encSel;

  if (encLevel == 7) // 40-bit RC4 Acrobat 3, 4
  {
    encSel = 0;
  }
  else if (encLevel == 8) // 128-bit RC4 Acrobat 5
  {
    encSel = 1;
  }
  else /*if (encMethod == 12)*/ // 128-bit AES Acrobat 7
  {
    encSel = 2;
  }

  _encryption.SetCurSel(encSel);

  updateState();

  return TRUE;
}

void CPasswordSettingsDlg::updateState()
{
  GetDlgItem(IDC_PASS_OPEN1)->EnableWindow(IsDlgButtonChecked(IDC_PASS_OPEN_DOC));
  GetDlgItem(IDC_PASS_OPEN2)->EnableWindow(IsDlgButtonChecked(IDC_PASS_OPEN_DOC));
  GetDlgItem(IDC_PASS_CHANGE1)->EnableWindow(IsDlgButtonChecked(IDC_PASS_CHANGE_DOC));
  GetDlgItem(IDC_PASS_CHANGE2)->EnableWindow(IsDlgButtonChecked(IDC_PASS_CHANGE_DOC));
  GetDlgItem(IDC_PRINTING_ALLOWED)->EnableWindow(IsDlgButtonChecked(IDC_PASS_CHANGE_DOC));
  GetDlgItem(IDC_CHANGES_ALLOWED)->EnableWindow(IsDlgButtonChecked(IDC_PASS_CHANGE_DOC));
  GetDlgItem(IDC_ENABLE_COPYING)->EnableWindow(IsDlgButtonChecked(IDC_PASS_CHANGE_DOC));
  GetDlgItem(IDC_ENABLE_TEXTACCESS)->EnableWindow(IsDlgButtonChecked(IDC_PASS_CHANGE_DOC));
}

BEGIN_MESSAGE_MAP(CPasswordSettingsDlg, CBCGPDialog)
  ON_BN_CLICKED(IDC_PASS_OPEN_DOC, &CPasswordSettingsDlg::OnBnClickedPassOpenDoc)
  ON_BN_CLICKED(IDC_PASS_CHANGE_DOC, &CPasswordSettingsDlg::OnBnClickedPassChangeDoc)
  ON_BN_CLICKED(IDOK, &CPasswordSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CPasswordSettingsDlg::OnBnClickedPassOpenDoc()
{
  updateState();
}

void CPasswordSettingsDlg::OnBnClickedPassChangeDoc()
{
  updateState();
}

void CPasswordSettingsDlg::OnBnClickedOk()
{
  CString pass1;
  CString pass2;

  if (IsDlgButtonChecked(IDC_PASS_OPEN_DOC))
  {
    GetDlgItemText(IDC_PASS_OPEN1, pass1);
    GetDlgItemText(IDC_PASS_OPEN2, pass2);

    if (pass1 != pass2)
    {
      AfxMessageBox(_T("Document open passwords don't match!"), MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  }
  if (IsDlgButtonChecked(IDC_PASS_CHANGE_DOC))
  {
    GetDlgItemText(IDC_PASS_CHANGE1, pass1);
    GetDlgItemText(IDC_PASS_CHANGE2, pass2);

    if (pass1 != pass2)
    {
      AfxMessageBox(_T("Change permissions passwords don't match!"), MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  }

  OnOK();
}
