// DocPropertiesFonts.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "DebenuPDFLibraryAX0912.h"
#include "DocPropertiesFonts.h"

// CDocPropertiesFonts dialog

IMPLEMENT_DYNAMIC(CDocPropertiesFonts, CBCGPDialog)

CDocPropertiesFonts::CDocPropertiesFonts(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_)
: CBCGPDialog(CDocPropertiesFonts::IDD, pParent)
, qpdf   (qpdf_)
{
}

CDocPropertiesFonts::~CDocPropertiesFonts()
{
}

void CDocPropertiesFonts::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BOOL CDocPropertiesFonts::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

//?  CLibraryDoc* pDoc = DYNAMIC_DOWNCAST(CLibraryDoc, ((CMainFrame*)AfxGetMainWnd())->GetActiveDocument());

  CRect rectGrid;
  CStatic& _fontsGridRect = *(CStatic*)GetDlgItem(IDC_STATICGRID);

  _fontsGridRect.GetClientRect(&rectGrid);
  _fontsGridRect.MapWindowPoints(this, &rectGrid);
  _fontsGridRect.ShowWindow(SW_HIDE);

  _fontsGrid.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
//?  _fontsGrid.SetImageList(&pDoc->imageList);

  _fontsGrid.EnableColumnAutoSize(TRUE);
  _fontsGrid.SetSingleSel(TRUE);
  _fontsGrid.EnableGroupByBox(FALSE);
  _fontsGrid.SetReadOnly();
  _fontsGrid.SetWholeRowSel();
  _fontsGrid.EnableHeader(FALSE, 0);

  CBCGPGridColors colors;
  colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;

  _fontsGrid.SetColorTheme (colors);
  _fontsGrid.InsertColumn(0, _T("Fonts"), 300);

  loadFonts();

  return TRUE;
}

void CDocPropertiesFonts::loadFonts()
{
  qpdf.FindFonts();
  int fontCnt = qpdf.FontCount();
  for (int k = 0; k < fontCnt; ++k)
  {
    CBCGPGridRow* row = _fontsGrid.CreateRow(1);
    row->GetItem(0)->SetImage(1);
    _fontsGrid.AddRow(row);

    qpdf.SelectFont(qpdf.GetFontID(k + 1));

    String fName = qpdf.FontName();
    String fFamily = qpdf.FontFamily();
    int    fType = qpdf.FontType();
    String typeStr;

    switch (fType)
    {
    case 2:
      typeStr = _T("Standard");
      break;
    case 3:
      typeStr = _T("TrueType");
      break;
    case 4:
      typeStr = _T("Embedded TrueType");
      break;
    case 5:
      typeStr = _T("Packaged");
      break;
    case 6:
      typeStr = _T("Type1");
      break;
    case 7:
      typeStr = _T("Subsetted");
      break;
    case 8:
      typeStr = _T("Type3");
      break;
    default:
      typeStr = _T("Unknown");
      break;
    }

    row->GetItem(0)->SetValue(fName.c_str());
    row->AllowSubItems();

    if (!fFamily.empty())
    {
      CBCGPGridRow* rowFamily = _fontsGrid.CreateRow(1);
      rowFamily->GetItem(0)->SetValue((_T("Family: ") + fFamily).c_str());
      row->AddSubItem(rowFamily);
    }

    CBCGPGridRow* rowType = _fontsGrid.CreateRow(1);
    rowType->GetItem(0)->SetValue((_T("Type: ") + typeStr).c_str());
    row->AddSubItem(rowType);
  }

  _fontsGrid.RedrawWindow();
}

BEGIN_MESSAGE_MAP(CDocPropertiesFonts, CBCGPDialog)
END_MESSAGE_MAP()
