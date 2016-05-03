// DocPropertiesInitView.cpp : implementation file
//

#include "stdafx.h"
#include "Benubird.h"
#include "DebenuPDFLibraryAX0912.h"
#include "DocPropertiesInitView.h"

const TCHAR* pageNavigationArr[] =
{
  _T("Page Only"),
  _T("Bookmarks Panel and Page"),
  _T("Pages Panel and Page"),
  _T("Content Group Panel and Page"),
  _T("Attachments Panel and Page"),
};

const TCHAR* pageLayoutArr[] =
{
  _T("Single Page"),
  _T("Single Page Continuous"),
  _T("Two-up Facing"),
  _T("Two-up Column Facing")
};

const TCHAR* magnificationArr[] =
{
  _T("Default"),
  _T("Fit Page"),
  _T("Fit Width"),
  _T("Fit Height"),
  _T("Fit Visible"),
  _T("25%"),
  _T("50%"),
  _T("75%"),
  _T("100%"),
  _T("200%"),
  _T("400%"),
  _T("800%"),
  _T("1600%"),
  _T("3200%"),
  _T("6400%"),
};

const TCHAR* titleArr[] =
{
  _T("File Name"),
  _T("Document Title")
};

// CDocPropertiesInitView dialog

IMPLEMENT_DYNAMIC(CDocPropertiesInitView, CBCGPDialog)

CDocPropertiesInitView::CDocPropertiesInitView(CWnd* pParent, DebenuPDFLibraryAX0912& qpdf_)
: CBCGPDialog(CDocPropertiesInitView::IDD, pParent)
, qpdf   (qpdf_)
{
}

CDocPropertiesInitView::~CDocPropertiesInitView()
{
}

void CDocPropertiesInitView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_NAVIGATIONTAB, m_cmbNavigationTab);
  DDX_Control(pDX, IDC_PAGELAYOUT, m_cmbPageLayout);
  DDX_Control(pDX, IDC_OPENTOPAGE, m_edOpentoPage);
  DDX_Control(pDX, IDC_TOTALPAGES, m_sTotalPages);
  DDX_Control(pDX, IDC_MAGNIFICATION, m_cmbMagnification);
  DDX_Control(pDX, IDC_HIDETOOLBAR, m_bHideToolbar);
  DDX_Control(pDX, IDC_HIDEMENUBAR, m_bHideMenuBar);
  DDX_Control(pDX, IDC_HIDEWINDOW, m_bHideWindowUI);
  DDX_Control(pDX, IDC_FITWINDOW, m_bFitWindow);
  DDX_Control(pDX, IDC_CENTERWINDOW, m_bCenterWindow);
  DDX_Control(pDX, IDC_FULLSCREEN, m_bFullScreen);
  DDX_Control(pDX, IDC_DOCUMENTTITLE, m_cmbDocumentTitleOrFileName);
}

BOOL CDocPropertiesInitView::OnInitDialog()
{
  CBCGPDialog::OnInitDialog();

  for (int k = 0; k < sizeof(pageNavigationArr) / sizeof(pageNavigationArr[0]); ++k)
  {
    m_cmbNavigationTab.InsertString(k, pageNavigationArr[k]);
  }
  for (int k = 0; k < sizeof(pageLayoutArr) / sizeof(pageLayoutArr[0]); ++k)
  {
    m_cmbPageLayout.InsertString(k, pageLayoutArr[k]);
  }
  for (int k = 0; k < sizeof(magnificationArr) / sizeof(magnificationArr[0]); ++k)
  {
    m_cmbMagnification.InsertString(k, magnificationArr[k]);
  }
  for (int k = 0; k < sizeof(titleArr) / sizeof(titleArr[0]); ++k)
  {
    m_cmbDocumentTitleOrFileName.InsertString(k, titleArr[k]);
  }

  LoadInfo();

  return TRUE;
}

BOOL CDocPropertiesInitView::LoadInfo(void)
{
  long rtn = 0;//general return values

  // Page Mode (none, outlines [bookmarks], thumbails, and fullscreen)

  /* 
  GetPageMode()
  0 UseNone
  1 UseOutlines
  2 UseThumbs
  3 FullScreen
  */

  long lPageMode = 0;
  lPageMode = qpdf.GetPageMode();
  switch(lPageMode)
  {
  case 0:
  case 1:
  case 2:
  case 4:
  case 5:
    if (lPageMode >= 4 && lPageMode <= 5)
    {
      --lPageMode;
    }
    m_cmbNavigationTab.SetCurSel(lPageMode);
    m_bFullScreen.SetCheck(BST_UNCHECKED); // false
    break;
  case 3:
    {
      long lNonFullScreenPageMode = 0;
      lNonFullScreenPageMode = qpdf.GetViewerPreferences(7);
      m_cmbNavigationTab.SetCurSel(lNonFullScreenPageMode);
      m_bFullScreen.SetCheck(BST_CHECKED); // true
    }
    break;
  default:
    // Default to none and no full screen if error
    m_cmbNavigationTab.SetCurSel(0);
    m_bFullScreen.SetCheck(BST_UNCHECKED); // false
    break;
  }

  // Page Layout mode (single page, one column, two column left, two column right)

  /*
  GetPageLayout()
  0 SinglePage
  1 OneColumn
  2 TwoColumnLeft
  3 TwoColumnRight
  */

  //It appears that some options are unsupported...
  long lPageLayout = 0;
  lPageLayout = qpdf.GetPageLayout();
  m_cmbPageLayout.SetCurSel(lPageLayout);

  // Number of Pages
  long lPageCount = 0;
  lPageCount = qpdf.PageCount();
  if(lPageCount)
  {
    CString strPageCount;
    strPageCount.Format(_T("of %d"), lPageCount);
    m_sTotalPages.SetWindowText(strPageCount);
  }

  // Open actions
  // GetDestType
  /*
  Return values
  1 "XYZ" - the target page is positioned at the Left and Top properties of the
  destination, and the Zoom property specifies the zoom percentage
  2 "Fit" - the entire page is zoomed to fit the window
  3 "FitH" - the page is zoomed so that the entire width of the page is visible. The height
  of the page may be greater or less than the height of the window. The page is
  positioned vertically at the Top property of the destination.
  4 "FitV" - the page is zoomed so that the entire height of the page can be seen. The
  width of the page may be greater or less than the width of the window. The page is
  positioned horizontally at the Left property of the destination.
  5 "FitR" - the page is zoomed so that a certain rectangle on the page is visible. The
  Left, Top, Right and Bottom properties of the destination define the rectangle on the
  page.
  6 "FitB" - the page is zoomed so that it's bounding box is visible
  7 "FitBH" - the page is positioned vertically at the value of the Top property of the
  destination, and the page is zoomed so that the entire width of the page's bounding
  box is visible
  8 "FitBV" - the page is positioned at the value of the Left property of the destination is
  visible, and the page is zoomed just enough to fit the entire height of the bounding
  box into the window
  */

  // Get open action
  // ____________________________________________________

  long lOpenActionDestinationID = 0; // Set to default
  long iOpenPage = 1;
  long lDestType = 1;
  CString strMagnification;
  lOpenActionDestinationID = qpdf.GetOpenActionDestination();
  if(lOpenActionDestinationID!=0)
  {
    double dZoom = 0;
    lDestType = qpdf.GetDestType(lOpenActionDestinationID); // Fit is default (2)
    dZoom = qpdf.GetDestValue(lOpenActionDestinationID, 5); // Get Zoom
    m_cmbMagnification.SetCurSel(lDestType-1); // zero-based index | 1 = Fit Page
    if(dZoom!=0)
    {
      strMagnification.Format(L"%d%%", (long) dZoom);
      m_cmbMagnification.SetCurSel(lDestType-1); // zero-based index | 1 = Fit Page
      m_cmbMagnification.SetWindowText(strMagnification);
    }
    iOpenPage = qpdf.GetDestPage(lOpenActionDestinationID);
  }

  // Set the open action 
  if(lOpenActionDestinationID==0)
  {
    m_cmbMagnification.SetCurSel(lDestType-1); // zero-based index | 1 = Fit Page
  }

  // Update the open page in the dialog (1 by default)
  // ____________________________________________________
  CString strOpenToPage;
  strOpenToPage.AppendFormat(L"%d", iOpenPage);
  m_edOpentoPage.SetWindowText(strOpenToPage);
  // ____________________________________________________


  // Viewer Preferences ...............
  // Toolbar
  long lHideToolbar = 0;
  lHideToolbar = qpdf.GetViewerPreferences(1);
  if(lHideToolbar==1)
  {
    m_bHideToolbar.SetCheck(BST_CHECKED);	
  }
  else
  {
    m_bHideToolbar.SetCheck(BST_UNCHECKED);
  }

  // Menu Bar
  long lHideMenuBar = 0;
  lHideMenuBar = qpdf.GetViewerPreferences(2);
  if(lHideMenuBar==1)
  {
    m_bHideMenuBar.SetCheck(BST_CHECKED);	
  }
  else
  {
    m_bHideMenuBar.SetCheck(BST_UNCHECKED);
  }

  // Window UI
  long lHideWindowUI = 0;
  lHideWindowUI = qpdf.GetViewerPreferences(3);
  if(lHideWindowUI==1)
  {
    m_bHideWindowUI.SetCheck(BST_CHECKED);	
  }
  else
  {
    m_bHideWindowUI.SetCheck(BST_UNCHECKED);
  }

  // Fit Window
  long lFitWindow = 0;
  lFitWindow = qpdf.GetViewerPreferences(4);
  if(lFitWindow==1)
  {
    m_bFitWindow.SetCheck(BST_CHECKED);	
  }
  else
  {
    m_bFitWindow.SetCheck(BST_UNCHECKED);
  }

  // Center Window
  long lCenterWindow = 0;
  lCenterWindow = qpdf.GetViewerPreferences(5);
  if(lCenterWindow==1)
  {
    m_bCenterWindow.SetCheck(BST_CHECKED);	
  }
  else
  {
    m_bCenterWindow.SetCheck(BST_UNCHECKED);
  }

  // Display Doc Title
  long lDisplayDocTitle = 0;
  lDisplayDocTitle = qpdf.GetViewerPreferences(6);
  m_cmbDocumentTitleOrFileName.SetCurSel(lDisplayDocTitle);

  return TRUE;
}

BOOL CDocPropertiesInitView::SaveInfo(void)
{
  long rtn = 0;//general return values
  AfxGetApp()->DoWaitCursor(1); // display the hourglass cursor


  // Page Mode (none, outlines [bookmarks], thumbails, and fullscreen)
  /* 
  GetPageMode()
  0 UseNone
  1 UseOutlines
  2 UseThumbs
  3 FullScreen
  */
  long lPageMode = m_cmbNavigationTab.GetCurSel(); // (Normal, Bookmarks, Thumbnails)
  if (lPageMode >= 3 && lPageMode <= 4)
  {
    ++lPageMode;
  }
  rtn = qpdf.SetPageMode(lPageMode); // Set to Normal, bookmarks or thumnails
  rtn = qpdf.SetViewerPreferences(7, lPageMode); // Also set the restore after full screen to the same valud
  if (m_bFullScreen.GetCheck()==BST_CHECKED) // If the full screen checkbox has been selected, then
  {
    rtn = qpdf.SetPageMode(3); // Set to use fullscreen
  }

  // Page Layout mode (single page, one column, two column left, two column right)
  /*
  GetPageLayout()
  0 SinglePage
  1 OneColumn
  2 TwoColumnLeft
  3 TwoColumnRight
  */

  //It appears that some options are unsupported...
  long lPageLayout = m_cmbPageLayout.GetCurSel();
  rtn = qpdf.SetPageLayout(lPageLayout);

  // Open actions
  // DestType
  /*
  Return values
  1 "XYZ" - the target page is positioned at the Left and Top properties of the
  destination, and the Zoom property specifies the zoom percentage
  2 "Fit" - the entire page is zoomed to fit the window
  3 "FitH" - the page is zoomed so that the entire width of the page is visible. The height
  of the page may be greater or less than the height of the window. The page is
  positioned vertically at the Top property of the destination.
  4 "FitV" - the page is zoomed so that the entire height of the page can be seen. The
  width of the page may be greater or less than the width of the window. The page is
  positioned horizontally at the Left property of the destination.
  5 "FitR" - the page is zoomed so that a certain rectangle on the page is visible. The
  Left, Top, Right and Bottom properties of the destination define the rectangle on the
  page.
  6 "FitB" - the page is zoomed so that it's bounding box is visible
  7 "FitBH" - the page is positioned vertically at the value of the Top property of the
  destination, and the page is zoomed so that the entire width of the page's bounding
  box is visible
  8 "FitBV" - the page is positioned at the value of the Left property of the destination is
  visible, and the page is zoomed just enough to fit the entire height of the bounding
  box into the window
  */

  // Get open action || need to support add open action
  long lOpenActionDestinationID = 0;
  long iOpenPage = 1;
  long lDestType = 1; // Default to "XYZ" where Zoom parameter specifies zoom percentage

  // Create an open action even if the view is default and page = 1
  CString strMagnification;
  m_cmbMagnification.GetWindowText(strMagnification);
  strMagnification.Remove(_T('%')); // Remove the percentage symbol
  strMagnification.MakeLower();

  double dZoom = 0;
  if(strMagnification==_T("fit page"))
  {
    lDestType = 2;
  }
  else if (strMagnification ==_T("fit width"))
  {
    lDestType = 3;
  }
  else if (strMagnification ==_T("fit height"))
  {
    lDestType = 4;
  }
  else if (strMagnification ==_T("fit visible"))
  {
    lDestType = 5;
  }
  else if (strMagnification == _T("default"))
  {
    lDestType = 3; // Fit width appears to be the default
  }
  else // otherwise zoom
  {
    lDestType = 1;
    dZoom = _wtoi(strMagnification);
    // Check for out of range values
    if(dZoom==0 || dZoom > 6400)
    {
      dZoom = 100; // If outside range then default to 100	
    }
  }

  // Get the open page
  CString strOpenToPage;
  m_edOpentoPage.GetWindowText(strOpenToPage);
  iOpenPage = _wtoi(strOpenToPage);

  // Store the open page, view settings and dest type settings

  double dLeft = qpdf.GetDestValue(lOpenActionDestinationID, 1); // Get Left
  double dTop = qpdf.GetDestValue(lOpenActionDestinationID, 2); // Get Top
  double dRight = qpdf.GetDestValue(lOpenActionDestinationID, 3); // Get Right
  double dBottom = qpdf.GetDestValue(lOpenActionDestinationID, 4); // Get Bottom

  rtn = qpdf.SetOpenActionDestinationFull(iOpenPage, (long) dZoom, lDestType, dLeft, dTop, dRight, dBottom);
  if(rtn==0)
  {
    AfxMessageBox(L"Could not set destination");
  }

  // Viewer Preferences
  // Toolbar
  long lHideToolbar = 0; // False
  if(m_bHideToolbar.GetCheck()==BST_CHECKED)
  {
    lHideToolbar = 1; // True
  }
  rtn = qpdf.SetViewerPreferences(1, lHideToolbar);

  // Menu Bar
  long lHideMenuBar = 0; // False
  if(m_bHideMenuBar.GetCheck()==BST_CHECKED)
  {
    lHideMenuBar = 1; // True
  }
  rtn = qpdf.SetViewerPreferences(2, lHideMenuBar);

  // Window UI
  long lHideWindowUI = 0; // False
  if(m_bHideWindowUI.GetCheck()==BST_CHECKED)
  {
    lHideWindowUI = 1; // True
  }
  rtn = qpdf.SetViewerPreferences(3, lHideWindowUI);

  // Fit Window
  long lFitWindow = 0; // False
  if(m_bFitWindow.GetCheck()==BST_CHECKED)
  {
    lFitWindow = 1; // True
  }
  rtn = qpdf.SetViewerPreferences(4, lFitWindow);

  // Center Window
  long lCenterWindow = 0; // False
  if(m_bCenterWindow.GetCheck()==BST_CHECKED)
  {
    lCenterWindow = 1; // True
  }
  rtn = qpdf.SetViewerPreferences(5, lCenterWindow);

  // Display Doc Title
  long lDisplayDocTitle = 0; // False
  lDisplayDocTitle = m_cmbDocumentTitleOrFileName.GetCurSel();
  rtn = qpdf.SetViewerPreferences(6, lDisplayDocTitle);

  AfxGetApp()->DoWaitCursor(-1); // clear the hourglass cursor

  return TRUE;
}
