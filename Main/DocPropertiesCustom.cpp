// DocPropertiesCustom.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "DebenuPDFLibraryAX0912.h"
#include "DocPropertiesCustom.h"


// CDocPropertiesCustom dialog

IMPLEMENT_DYNAMIC(CDocPropertiesCustom, CBCGPDialog)

CDocPropertiesCustom::CDocPropertiesCustom(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_)
: CBCGPDialog(CDocPropertiesCustom::IDD, pParent)
, qpdf   (qpdf_)
{
}

CDocPropertiesCustom::~CDocPropertiesCustom()
{
}

void CDocPropertiesCustom::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROPSLIST, _propsList);
}

BOOL CDocPropertiesCustom::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

  _propsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
  _propsList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 300);

  _propsList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  _propsList.EnableMarkSortedColumn();

  loadProperties();

  return TRUE;
}

void CDocPropertiesCustom::loadProperties()
{
  String keys = qpdf.GetCustomKeys(1);
  int start = 0;
  int stop = -1;

  std::list<String> keysList;
  String            keyName;
  for (;;)
  {
    start = keys.find('\"', stop + 1);
    if (start == String::npos)
      break;
    stop = keys.find('\"', start + 1);
    keyName = keys.substr(start + 1, stop - start - 1);
    keysList.push_back(keyName);
  }
  std::list<String>::iterator it = keysList.begin();
  std::list<String>::iterator itEnd = keysList.end();

  _propsList.DeleteAllItems();

  for (int k = 0; it != itEnd; ++it, ++k)
  {
    String val = qpdf.GetCustomInformation(*it);

    _propsList.InsertItem(k, it->c_str());
    _propsList.SetItemText(k, 1, val.c_str());
  }

  updateButtonsState();
}

void CDocPropertiesCustom::updateButtonsState()
{
  POSITION pos = _propsList.GetFirstSelectedItemPosition();
  BOOL haveSelected = pos != NULL;

  GetDlgItem(IDC_DELETE)->EnableWindow(haveSelected);
}

BEGIN_MESSAGE_MAP(CDocPropertiesCustom, CBCGPDialog)
  ON_BN_CLICKED(IDC_ADD, &CDocPropertiesCustom::OnBnClickedAdd)
  ON_BN_CLICKED(IDC_DELETE, &CDocPropertiesCustom::OnBnClickedDelete)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROPSLIST, &CDocPropertiesCustom::OnLvnItemchangedFilesList)
END_MESSAGE_MAP()

void CDocPropertiesCustom::OnBnClickedAdd()
{
	int nRet = 0;
  CString keyName;
  CString keyValue;

  GetDlgItemText(IDC_PROPNAME, keyName);
  if (keyName.IsEmpty())
  {
    AfxMessageBox(_T("Property name can't be empty."), MB_OK | MB_ICONEXCLAMATION);
    GetDlgItem(IDC_PROPNAME)->SetFocus();
    return;
  }
  GetDlgItemText(IDC_PROPVALUE, keyValue);
  nRet = qpdf.SetCustomInformation(keyName.GetString(), keyValue.GetString());

  SetDlgItemText(IDC_PROPNAME, _T(""));
  SetDlgItemText(IDC_PROPVALUE, _T(""));

  loadProperties();
}

void CDocPropertiesCustom::OnBnClickedDelete()
{
  POSITION pos = _propsList.GetFirstSelectedItemPosition();
  int      selItem = _propsList.GetNextSelectedItem(pos);
  String   keyName = _propsList.GetItemText(selItem, 0).GetString();
  if (qpdf.SetCustomInformation(keyName, _T("")) != 1)
  {
    AfxMessageBox(_T("Error deleting custom key."), MB_ICONEXCLAMATION);
  }
  else
  {
    loadProperties();
  }
}

void CDocPropertiesCustom::OnLvnItemchangedFilesList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW nmlv = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  if (nmlv->uChanged & LVIF_STATE)
  {
    BOOL bHaveSelected = nmlv->uNewState & LVIS_SELECTED;
    updateButtonsState();
  }

  *pResult = 0;
}
