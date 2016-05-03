#include "stdafx.h"
#include <Winuser.h>
#include "Define.h"
#include "Benubird.h"
#include "PDFDoc.h"
#include "DebenuPDFLibraryAX0912.h"
#include "PDFView.h"
#include "MainFrm.h"
#include "DocumentPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CView* IsOpenedLibPath(CString path);

IMPLEMENT_DYNCREATE(CPDFView, CScrollView)

BEGIN_MESSAGE_MAP(CPDFView, CScrollView)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_SIZING()
  ON_WM_KEYDOWN()
  ON_COMMAND(ID_PDF_PRINT, &CView::OnFilePrint)
  ON_UPDATE_COMMAND_UI(ID_PDF_PRINT, &CPDFView::OnUpdateAlwaysEnabledButton/*OnUpdateAlwaysDisabledButton*/)
  ON_COMMAND(ID_PDF_DEFAULT_VIEWER, &CPDFView::OnOpenDefaultPDFViewer)
  ON_UPDATE_COMMAND_UI(ID_PDF_DEFAULT_VIEWER, &CPDFView::OnUpdateAlwaysEnabledButton/*OnUpdateAlwaysDisabledButton*/)

  ON_COMMAND(ID_PDF_SAVE, &CPDFView::OnSavePdf)
  ON_UPDATE_COMMAND_UI(ID_PDF_SAVE, &CPDFView::OnUpdateAlwaysEnabledButton)
  ON_COMMAND(ID_PDF_SAVE_AS, &CPDFView::OnSaveAs)
  ON_UPDATE_COMMAND_UI(ID_PDF_SAVE_AS, &CPDFView::OnUpdateAlwaysEnabledButton)
  ON_COMMAND(ID_PDF_SAVE_EXPORT, &CPDFView::OnExportPdf)
  ON_UPDATE_COMMAND_UI(ID_PDF_SAVE_AS, &CPDFView::OnUpdateAlwaysEnabledButton)

  ON_COMMAND(ID_PDF_PROPERTIES, &CPDFView::OnDocProperties)
  ON_UPDATE_COMMAND_UI(ID_PDF_PROPERTIES, &CPDFView::OnUpdateAlwaysEnabledButton)
  ON_COMMAND(ID_PDF_ZOOM_OUT, &CPDFView::OnZoomOut)
  ON_UPDATE_COMMAND_UI(ID_PDF_ZOOM_OUT, &CPDFView::OnUpdateZoomOut)
  ON_COMMAND(ID_PDF_ZOOM_IN, &CPDFView::OnZoomIn)
  ON_UPDATE_COMMAND_UI(ID_PDF_ZOOM_IN, &CPDFView::OnUpdateZoomIn)
  ON_COMMAND(ID_PDF_ZOOMCOMBO, &CPDFView::OnZoomChange)
  ON_UPDATE_COMMAND_UI(ID_PDF_ZOOMCOMBO, &CPDFView::OnUpdateAlwaysEnabledButton)
  ON_CONTROL(CBN_SELCHANGE, ID_PDF_ZOOMCOMBO, &CPDFView::OnZoomChange)
  ON_COMMAND(ID_PDF_PREVPAGE, &CPDFView::OnPagePrev)
  ON_UPDATE_COMMAND_UI(ID_PDF_PREVPAGE, &CPDFView::OnUpdatePagePrev)
  ON_COMMAND(ID_PDF_PAGEEDIT, &CPDFView::OnPageGo)
  ON_UPDATE_COMMAND_UI(ID_PDF_PAGEEDIT, &CPDFView::OnUpdateAlwaysEnabledButton)
  ON_COMMAND(ID_PDF_PAGES, &CPDFView::OnPagePages)
  ON_UPDATE_COMMAND_UI(ID_PDF_PAGES, &CPDFView::OnUpdateAlwaysEnabledButton)
  ON_COMMAND(ID_PDF_NEXTPAGE, &CPDFView::OnPageNext)
  ON_UPDATE_COMMAND_UI(ID_PDF_NEXTPAGE, &CPDFView::OnUpdatePageNext)
  ON_COMMAND(ID_PDF_HOMEPAGE, &CPDFView::OnPageHome)
  ON_COMMAND(ID_PDF_ENDPAGE, &CPDFView::OnPageEnd)
  ON_WM_CLOSE()
END_MESSAGE_MAP()

#define PAGE_BG_COLOR RGB(100, 100, 100)
extern const TCHAR* magnificationArr[];

CPDFView::CPDFView()
: _imageBuf   (NULL)
, _hbmp       (NULL)
, _imageWidth (0)
, _imageHeight(0)
, _pageNum    (1)
, _pageCount  (0)
, _zoom       (1.0)
, _dpi        (96)
{
	m_bOpenDefaultPDFViewer = false;
}

CPDFView::~CPDFView()
{
	 int rtn = IDCANCEL;

	if(m_bIsChanged != 0)
	{ 
		//pdf is changed
		rtn = ::MessageBox(NULL,_T("This PDF has been modified - do you wish to save?") ,_T("Warning"),MB_OKCANCEL);
		if(rtn == IDCANCEL)
		{
			
		}
	}

	if(rtn != IDCANCEL)
	{
		rtn = _qpdf.DAAppendFile(_pdfId);
	}
	else
	{
		rtn = _qpdf.DACloseFile(_pdfId);
	}


	String ss = _strFile;
	if(m_bOpenDefaultPDFViewer)
		ShellExecute(NULL, _T("open"), _strFile, NULL, NULL, SW_SHOWNORMAL);


  DeleteObject(_hbmp);

}

void CPDFView::RenderPage()
{
  static int cnt = 1;
  TRACE("CPDFView::RenderPage %d\n", cnt++);

  HDC screenDC = ::GetDC(NULL);
  HDC memoryDC = CreateCompatibleDC(screenDC);
  ::ReleaseDC(NULL, screenDC);

  double dWidth = _qpdf.DAGetPageWidth(_pdfId, _pageRef);
  _imageWidth = int( dWidth * _dpi / 72 * _zoom);
  _imageHeight = int(_qpdf.DAGetPageHeight(_pdfId, _pageRef) * _dpi / 72 * _zoom);
  int rot = _qpdf.DAPageRotation(_pdfId, _pageRef);
  if (rot == 90 || rot == 270)
  {
    std::swap(_imageWidth, _imageHeight);
  }

  memset(&_bi, 0, sizeof(_bi));
  _bi.bmiHeader.biSize = sizeof(_bi.bmiHeader);
  _bi.bmiHeader.biPlanes = 1;
  _bi.bmiHeader.biBitCount = 24;
  _bi.bmiHeader.biWidth = _imageWidth;
  _bi.bmiHeader.biHeight = _imageHeight;
  _bi.bmiHeader.biCompression = BI_RGB;

  DeleteObject(_hbmp);
  _hbmp = CreateDIBSection(memoryDC, &_bi, DIB_RGB_COLORS, (void**)&_imageBuf, 0, 0);
  HBITMAP hbmpOld = (HBITMAP)SelectObject(memoryDC, _hbmp);

  memset(_imageBuf, 0xFF, _imageWidth * _imageHeight * 3);
  _qpdf.DARenderPageToDC(_pdfId, _pageRef, int(_dpi * _zoom), memoryDC);

  SelectObject(memoryDC, hbmpOld);
  DeleteDC(memoryDC);

  SetScrollSizes(MM_TEXT, CSize(_imageWidth, _imageHeight));

  CMainFrame* mainFrm = (CMainFrame*)GetParent();
  CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
//  CBCGPToolbarEditBoxButton* pageEdit = (CBCGPToolbarEditBoxButton*)mainFrm->m_wndToolBar.GetButton(mainFrm->m_wndToolBar.CommandToIndex(ID_PAGE_EDIT));

  if (pDoc->pdfPage.c_str())
  {
    CString pageStr;
    pageStr.Format(L"%d", _pageNum);
//?    pageEdit->GetEditBox()->SetWindowText(pageStr); // to UI_UPAD

    mainFrm->SetFocus();
  }

//?  CBCGPToolbarButton* pagesButton = (CBCGPToolbarButton*)mainFrm->m_wndToolBar.GetButton(mainFrm->m_wndToolBar.CommandToIndex(ID_PAGE_PAGES));
  //if (pagesButton)
  //{
  //  CString pagesStr;
  //  pagesStr.Format(L"/ %d", _pageCount);
  //  pagesButton->m_strText = pagesStr;
  //}
}

void CPDFView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  //part for structurer
	m_bIsChanged = 0;

	m_nZoom = 100;

	_qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	if(_qpdf.Unlocked() != 1)
	{
		return;
	}

  ((CMainFrame*)AfxGetApp()->m_pMainWnd)->ActivateCategory(1);

  CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

  if(pDoc == NULL)
	  return;

  _strFile = pDoc->pdfPath.c_str();
  //_pdfId = _qpdf.DAOpenFileReadOnly(_strFile.GetBuffer(MAX_PATH), pDoc->password);
  _pdfId = _qpdf.DAOpenFile(_strFile.GetBuffer(MAX_PATH), pDoc->password);
  _pageRef = _qpdf.DAFindPage(_pdfId, _pageNum);
  _pageCount = _qpdf.DAGetPageCount(_pdfId);

  pDoc->m_nPdfTotalPage = _pageCount;
  pDoc->m_nPdfCurrentPageNo = 1;

  HDC screenDC = ::GetDC(NULL);
  _dpi = GetDeviceCaps(screenDC, LOGPIXELSY);
  ::ReleaseDC(NULL, screenDC);

  _imageWidth = int(_qpdf.DAGetPageWidth(_pdfId, _pageRef) * _dpi / 72 * _zoom);
  _imageHeight = int(_qpdf.DAGetPageHeight(_pdfId, _pageRef) * _dpi / 72 * _zoom);


  InitView();
}

void CPDFView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ActivateCategory(1);
}

void CPDFView::InitView()
{
	long lOpenActionDestinationID = 0;
	long iOpenPage = 1;
	long lDestType = 1;
	CString strMagnification;

	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

	DebenuPDFLibraryAX0912 qpdf;
	qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
	if(qpdf.Unlocked() != 1)
	{
		return;
	}

  if (qpdf.LoadFromFile(_strFile.GetBuffer(MAX_PATH), pDoc->password) == 1)
  {
    lOpenActionDestinationID = qpdf.GetOpenActionDestination();
    if (lOpenActionDestinationID != 0)
    {
      double dZoom = 0;
      lDestType = qpdf.GetDestType(lOpenActionDestinationID); // Fit is default (2)
      dZoom = qpdf.GetDestValue(lOpenActionDestinationID, 5); // Get Zoom

      //?CMainFrame* mainFrm = (CMainFrame*)GetParent();
      //CBCGPToolbarComboBoxButton* zoomCombo = (CBCGPToolbarComboBoxButton*)mainFrm->m_wndToolBar.GetButton(mainFrm->m_wndToolBar.CommandToIndex(ID_PDF_ZOOMCOMBO));

      //if (dZoom != 0)
      //{
      //  strMagnification.Format(L"%d%%", (long) dZoom);
      //  zoomCombo->GetEditCtrl()->SetWindowText(strMagnification);
      //}
      //else
      //{
      //  zoomCombo->GetEditCtrl()->SetWindowText(magnificationArr[lDestType - 1]);
      //}
      iOpenPage = qpdf.GetDestPage(lOpenActionDestinationID);
      GoToPage(iOpenPage);
    }
  }

  OnZoomChange();
}

BOOL CPDFView::PreCreateWindow(CREATESTRUCT& cs) 
{
  if (!CScrollView::PreCreateWindow(cs))
    return FALSE;

  cs.dwExStyle |= WS_EX_CLIENTEDGE;
  cs.style &= ~WS_BORDER;
  cs.style &= ~FWS_ADDTOTITLE;
  cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
                   ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

  return TRUE;
}

int CPDFView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CScrollView::OnCreate(lpCreateStruct) == -1)
	return -1;

  SIZE sz;
  sz.cx = 0;
  sz.cy = 0;
  SetScrollSizes(MM_TEXT, sz);

  return 0;
}

void CPDFView::OnDestroy()    
{   
  CScrollView ::OnDestroy();   

  _pageNum = 0;
}

void CPDFView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar) {
	case VK_PRIOR:
		OnPagePrev();
		break;
	case VK_NEXT:
		OnPageNext();
		break;
	case VK_HOME:
		OnPageHome();
		break;
	case VK_END:
		OnPageEnd();
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	return;
}

void CPDFView::GoToPage(int page)
{
  _pageNum = max(1, min(page, _pageCount));
  _pageRef = _qpdf.DAFindPage(_pdfId, _pageNum);

  CMainFrame* mainFrm = (CMainFrame*)(::AfxGetApp()->m_pMainWnd);
  mainFrm->SetCurrentPageNo(page);

  RenderPage();

  Invalidate();

  SetScrollPos(SB_HORZ, 0, TRUE);
  SetScrollPos(SB_VERT, 0, TRUE);

  UpdateWindow();
}

int CPDFView::GetZoom()
{
  CMainFrame* mainFrm = (CMainFrame*)(::AfxGetApp()->m_pMainWnd);
  /*
  CBCGPRibbonCategory* lCategory = mainFrm->m_pRibbonCategory[1];
  CBCGPRibbonPanel*    lPanel = lCategory->GetPanel(1);
  CBCGPRibbonComboBox* zoomCombo = (CBCGPRibbonComboBox*)lPanel->GetElement(1);
  */
  int nZoomNo = mainFrm->GetZoom_FromCombo();

  m_nZoom = stdZoomVals[nZoomNo];

  CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

  if (pDoc)
  {
    CRect cliRect;
    GetWindowRect(&cliRect);
    ScreenToClient(&cliRect);

	if (m_nZoom == ZOOM_FIT_PAGE)
    {
      int width = cliRect.right - cliRect.left;
      int height = cliRect.bottom - cliRect.top;
      if ((double)_imageHeight / _imageWidth > (double)height / width)
      {
        m_nZoom = ZOOM_FIT_HEIGHT;
      }
      else
      {
        m_nZoom = ZOOM_FIT_WIDTH;
      }
    }
    if (m_nZoom == ZOOM_FIT_WIDTH)
    {
      int zoom = (cliRect.right - cliRect.left) * 100 / (int)(_qpdf.DAGetPageWidth(_pdfId, _pageRef) * _dpi / 72);
      int width = int(_qpdf.DAGetPageWidth(_pdfId, _pageRef) * _dpi / 72 * zoom / 100.0);
      int height = int(_qpdf.DAGetPageHeight(_pdfId, _pageRef) * _dpi / 72 * zoom / 100.0);
      if (height > (cliRect.bottom - cliRect.top))
      {
        zoom = (cliRect.right - cliRect.left - GetSystemMetrics(SM_CXVSCROLL)) * 100 / (int)(_qpdf.DAGetPageWidth(_pdfId, _pageRef) * _dpi / 72);
      }
      return zoom - 1;
    }
    if (m_nZoom == ZOOM_FIT_HEIGHT)
    {
      int zoom = (cliRect.bottom - cliRect.top) * 100 / (int)(_qpdf.DAGetPageHeight(_pdfId, _pageRef) * _dpi / 72);
      int height = int(_qpdf.DAGetPageHeight(_pdfId, _pageRef) * _dpi / 72 * zoom / 100.0);
      int width = int(_qpdf.DAGetPageWidth(_pdfId, _pageRef) * _dpi / 72 * zoom / 100.0);
      if (width > (cliRect.right - cliRect.left))
      {
        zoom = (cliRect.bottom - cliRect.top - GetSystemMetrics(SM_CYHSCROLL)) * 100 / (int)(_qpdf.DAGetPageHeight(_pdfId, _pageRef) * _dpi / 72);
      }
      return zoom - 1;
    }
  }

  return m_nZoom;
}

void CPDFView::SetZoom(int zoom)
{
  CMainFrame* mainFrm = (CMainFrame*)(::AfxGetApp()->m_pMainWnd);

  mainFrm->SetZoom_FromCombo(zoom);
//  CBCGPToolbarComboBoxButton* zoomCombo = (CBCGPToolbarComboBoxButton*)mainFrm->m_wndToolBar.GetButton(mainFrm->m_wndToolBar.CommandToIndex(ID_PDF_ZOOMCOMBO));
  CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

  if (pDoc)
  {
    m_nZoom = zoom;
    _zoom = zoom / 100.;

    CString zoomValStr;
    zoomValStr.Format(L"%d%%", zoom);
//?    zoomCombo->GetEditCtrl()->SetWindowText(zoomValStr); // to UI_UPADATES
    mainFrm->SetFocus();
    RenderPage();
    Invalidate();
    UpdateWindow();
  }
}

void CPDFView::OnDraw(CDC* pDC)
{
  CRect cliRect;
  GetClientRect(&cliRect);
  CSize scrollSize = GetTotalSize();

  int xPos;
  int yPos;
  int cx;
  int cy;

  if (scrollSize.cx < cliRect.right)
  {
    xPos = (cliRect.right - _imageWidth) / 2;
    cx = cliRect.right;
  }
  else
  {
    xPos = 0;
    cx = scrollSize.cx;
  }

  if (scrollSize.cy < cliRect.bottom)
  {
    yPos = (cliRect.bottom - _imageHeight) / 2;
    cy = cliRect.bottom;
  }
  else
  {
    yPos = 0;
    cy = scrollSize.cy;
  }

  pDC->FillSolidRect(0, 0, cx, yPos, PAGE_BG_COLOR);
  pDC->FillSolidRect(0, 0, xPos, cy, PAGE_BG_COLOR);
  pDC->FillSolidRect(cx - xPos - 1, 0, xPos + 1, cy, PAGE_BG_COLOR);
  pDC->FillSolidRect(0, cy - yPos - 1, cx, yPos + 1, PAGE_BG_COLOR);

  StretchDIBits(pDC->GetSafeHdc(), xPos, yPos, _imageWidth, _imageHeight, 0, 0, _imageWidth, _imageHeight, _imageBuf, &_bi, DIB_RGB_COLORS, SRCCOPY);
}

BOOL CPDFView::OnEraseBkgnd(CDC* pDC)
{
  return FALSE;
}

void CPDFView::OnSize(UINT nType, int cx, int cy)
{
  CScrollView::OnSize(nType, cx, cy);

  Invalidate();
  UpdateWindow();
}

void CPDFView::OnSizing(UINT fwSide, LPRECT pRect)
{
  CScrollView::OnSizing(fwSide, pRect);

  Invalidate();
  UpdateWindow();
}

BOOL CPDFView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->SetMaxPage(_pageCount);
  return DoPreparePrinting(pInfo);
}

void CPDFView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPDFView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPDFView::OnUpdateAlwaysEnabledButton(CCmdUI* pCmdUI)
{
  TCHAR szStr[0x20];
  CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
  

  if( pCmdUI->m_nID == ID_PDF_SAVE )
  {
	  if(!m_bIsChanged)
		  pCmdUI->Enable(FALSE);
	  return;
  }

  pCmdUI->Enable(TRUE);



  
/*
  if( pCmdUI->m_nID == ID_PDF_ZOOMCOMBO )
  {
	  if(m_nZoom == ZOOM_FIT_PAGE)
	  {
		 wsprintf(szStr,_T("Fit Page"));
	  }
	  else if(m_nZoom == ZOOM_FIT_WIDTH)
	  {
		  wsprintf(szStr,_T("Fit Width"));
	  }
	  else if(m_nZoom == ZOOM_FIT_HEIGHT)
	  {
		  wsprintf(szStr,_T("Fit Height"));
	  }
	  else
	  {
		wsprintf(szStr,_T("%d%%"),m_nZoom);
	  }
	  pCmdUI->SetText(szStr);
  }
  
  if(pCmdUI->m_nID == ID_PDF_PAGEEDIT)
  {
	  wsprintf(szStr,_T("%d"),pDoc->m_nPdfCurrentPageNo);
	  pCmdUI->SetText(szStr);
  }
 
  else */ if(pCmdUI->m_nID == ID_PDF_PAGES)
  {
	  wsprintf(szStr,_T("/%d "),pDoc->m_nPdfTotalPage);
	  pCmdUI->SetText(szStr);
  }
   
  
}

void CPDFView::OnUpdateAlwaysDisabledButton(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(FALSE);
}

void CPDFView::OnSaveAs()
{
	
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, this->GetDocument());

	CString csNewName;
	int retVal = 0,nErrorCode = 0;

	//don't save changes, and export original file.
	CFileDialog savePdf1(FALSE, L"pdf", _strFile.GetBuffer(MAX_PATH), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.pdf|*.pdf||");
	savePdf1.m_pOFN->lpstrTitle = _T("Save As");
	if (savePdf1.DoModal() == IDOK)
	{
		csNewName = savePdf1.GetPathName();
		retVal = _qpdf.DASaveAsFile(_pdfId,csNewName.GetBuffer());
		if(retVal == 0)
		{
			nErrorCode = _qpdf.LastErrorCode();
			AfxMessageBox(_T("Create New PDF error"));
			return;
		}

		CString czDBName = pDoc->m_csDBPathName;

		//CView* pView = IsOpenedLibPath(czDBName);
		//if(pView == NULL)
		//{
			//this means that library view was closed. you have to insert new file to db directly.
			CDB mDB;
			BOOL ret = mDB.Initialize(czDBName.GetBuffer());
			if(!ret)
				return;

			PARAM_FOR_CDB mPARAM_CDB;
			mPARAM_CDB.pDB = &mDB;
			wcscpy(mPARAM_CDB.szLibPathName,czDBName.GetBuffer());

			CStringList lFileNames;
			lFileNames.AddTail(csNewName);
			CIntArray lAddedFilesIDs;

			
			CTFiles lTFiles(&mPARAM_CDB);
			int nAddedFiles = lTFiles.AddFiles_OverWrite(lFileNames, NULL, &lAddedFilesIDs);
			mDB.Close();
		//}
		
		
		
		_qpdf.DACloseFile(_pdfId);

		CFrameWnd * pChild = ((CMainFrame*)(theApp.m_pMainWnd))->GetActiveFrame();
		pChild->DestroyWindow();

		theApp.OpenDocumentFile_ForPDFView(csNewName.GetBuffer(),czDBName.GetBuffer());
	}
}

void CPDFView::OnSavePdf()
{
	int rtn = 0;

	rtn = _qpdf.DAAppendFile(_pdfId);
	if(rtn != 1)
	{
		AfxMessageBox(_T("Save PDF Error"));
	}

	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	if(pDoc == NULL)
		return;

	_strFile = pDoc->pdfPath.c_str();
	_pdfId = _qpdf.DAOpenFile(_strFile.GetBuffer(MAX_PATH), pDoc->password);
	if(_pdfId == 0)
	{
		AfxMessageBox(_T("PDF Open Error1"));
	}

	m_bIsChanged = 0;
}

void CPDFView::OnExportPdf()
{
	int rtn = IDCANCEL;

	if(m_bIsChanged != 0)
	{ 
		//pdf is changed
		rtn = ::MessageBox(NULL,_T("This PDF has been modified - do you wish to save?") ,_T("Warning"),MB_OKCANCEL);
		if(rtn == IDCANCEL)
		{
			
		}
	}

	if(rtn != IDCANCEL)
	{
		//save current changes
		rtn = _qpdf.DAAppendFile(_pdfId);

		CFileDialog savePdf(FALSE, L"pdf", _strFile.GetBuffer(MAX_PATH), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.pdf|*.pdf||");
		savePdf.m_pOFN->lpstrTitle = _T("Export to");
		if (savePdf.DoModal() == IDOK)
		{
			CopyFile(_strFile.GetBuffer(MAX_PATH), savePdf.GetPathName(), FALSE);
		}

		CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

		if(pDoc == NULL)
			return;

		_strFile = pDoc->pdfPath.c_str();
		_pdfId = _qpdf.DAOpenFile(_strFile.GetBuffer(MAX_PATH), pDoc->password);
		if(_pdfId == 0)
		{
			AfxMessageBox(_T("PDF Open Error2"));
		}
		_pageRef = _qpdf.DAFindPage(_pdfId, _pageNum);

		m_bIsChanged = 0;
	}
	else
	{
		//don't save changes, and export original file.
		CFileDialog savePdf1(FALSE, L"pdf", _strFile.GetBuffer(MAX_PATH), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.pdf|*.pdf||");
		savePdf1.m_pOFN->lpstrTitle = _T("Export to");
		if (savePdf1.DoModal() == IDOK)
		{
			CopyFile(_strFile.GetBuffer(MAX_PATH), savePdf1.GetPathName(), FALSE);
		}
	}
}


void CPDFView::OnDocProperties()
{
	int rtn = IDCANCEL;

	if(m_bIsChanged != 0)
	{ 
		//pdf is changed
		rtn = ::MessageBox(NULL,_T("This PDF has been modified - do you wish to save?") ,_T("Warning"),MB_OKCANCEL);
		if(rtn == IDCANCEL)
		{
			
		}
	}

	if(rtn != IDCANCEL)
	{
		//save
		rtn = _qpdf.DAAppendFile(_pdfId);
	}
	else
	{
		//don't save
		rtn = _qpdf.DACloseFile(_pdfId);
	}

	CDocumentPropertiesDlg documentPropertiesDlg;
	documentPropertiesDlg.pdfPath = _strFile;
	documentPropertiesDlg.cmd = L"CMD_DOCUMENTSEC";
	if(documentPropertiesDlg.DoModal() == IDOK)
	{
		CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, this->GetDocument());
		CString csNewName = _strFile;
		CString czDBName = pDoc->m_csDBPathName;

		CDB mDB;
		BOOL ret = mDB.Initialize(czDBName.GetBuffer());
		if(!ret)
			return;

		PARAM_FOR_CDB mPARAM_CDB;
		mPARAM_CDB.pDB = &mDB;
		wcscpy(mPARAM_CDB.szLibPathName,czDBName.GetBuffer());

		CStringList lFileNames;
		lFileNames.AddTail(csNewName);
		CIntArray lAddedFilesIDs;

			
		CTFiles lTFiles(&mPARAM_CDB);
		int nAddedFiles = lTFiles.AddFiles_OverWrite(lFileNames, NULL, &lAddedFilesIDs);
		mDB.Close();
	}

	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

	if(pDoc == NULL)
		return;

	_strFile = pDoc->pdfPath.c_str();
	_pdfId = _qpdf.DAOpenFile(_strFile.GetBuffer(MAX_PATH), pDoc->password);
	if(_pdfId == 0)
	{
		AfxMessageBox(_T("PDF Open Error3"));
	}
	_pageRef = _qpdf.DAFindPage(_pdfId, _pageNum);

	m_bIsChanged = 0;
}

void CPDFView::OnZoomChange()
{
  int zoom = GetZoom();
  zoom = max(10, min(zoom, 400));
  SetZoom(zoom);
}

void CPDFView::OnZoomOut()
{
  int zoom = GetZoom();

  for (int k = 8 ; k >= 0 ; k--)
  {
    if (stdZoomVals[k] < zoom)
    {
      zoom = stdZoomVals[k];
      break;
    }
  }

  SetZoom(zoom);  
}

void CPDFView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
  int zoom = GetZoom();
  pCmdUI->Enable(zoom > stdZoomVals[0]);
}

void CPDFView::OnZoomIn()
{
  int zoom = GetZoom();

  for (int k = 0; k < 9; k++)
  {
    if (stdZoomVals[k] > zoom)
    {
      zoom = stdZoomVals[k];
      break;
    }
  }

  SetZoom(zoom);
}

void CPDFView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
  int zoom = GetZoom();
  pCmdUI->Enable(zoom < 400);
}

void CPDFView::OnPagePrev()
{
     CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	 if(pDoc->m_nPdfCurrentPageNo > 1)
		 pDoc->m_nPdfCurrentPageNo--;

     GoToPage( pDoc->m_nPdfCurrentPageNo);
}

void CPDFView::OnUpdatePagePrev(CCmdUI* pCmdUI)
{
    CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	pCmdUI->Enable(pDoc->m_nPdfCurrentPageNo>1);
}

void CPDFView::OnPageGo()
{
  CMainFrame* mainFrm = (CMainFrame*)(::AfxGetApp()->m_pMainWnd);
  CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());

  pDoc->m_nPdfCurrentPageNo = mainFrm->GetCurrentPageNo();
  if( pDoc->m_nPdfCurrentPageNo > pDoc->m_nPdfTotalPage)
	  pDoc->m_nPdfCurrentPageNo = pDoc->m_nPdfTotalPage;
  else if( pDoc->m_nPdfCurrentPageNo < 1)
	  pDoc->m_nPdfCurrentPageNo = 1;

  GoToPage( pDoc->m_nPdfCurrentPageNo);
}

void CPDFView::OnPagePages()
{
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	pDoc->m_nPdfCurrentPageNo = pDoc->m_nPdfTotalPage;
	GoToPage(pDoc->m_nPdfCurrentPageNo);
}

void CPDFView::OnPageNext()
{
	 CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	 if(pDoc->m_nPdfCurrentPageNo<pDoc->m_nPdfTotalPage)
		 pDoc->m_nPdfCurrentPageNo++;

     GoToPage( pDoc->m_nPdfCurrentPageNo);
}

void CPDFView::OnUpdatePageNext(CCmdUI* pCmdUI)
{
	CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	pCmdUI->Enable(pDoc->m_nPdfCurrentPageNo<pDoc->m_nPdfTotalPage);
}

void CPDFView::OnPageHome()
{
	 CPDFDoc* pDoc = DYNAMIC_DOWNCAST(CPDFDoc, GetDocument());
	 pDoc->m_nPdfCurrentPageNo = 1;
     GoToPage(pDoc->m_nPdfCurrentPageNo);
}

void CPDFView::OnPageEnd()
{
  OnPagePages();
}

void CPDFView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
  int pageRef = _qpdf.DAFindPage(_pdfId, pInfo->m_nCurPage);
  int dpi = pDC->GetDeviceCaps(LOGPIXELSY);
  _qpdf.DARenderPageToDC(_pdfId, pageRef, dpi, pDC->GetSafeHdc());
}



void CPDFView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnClose();
}

void CPDFView::OnOpenDefaultPDFViewer()
{
	CFrameWnd * pChild = ((CMainFrame*)(theApp.m_pMainWnd))->GetActiveFrame();

	if(AfxMessageBox(_T("This PDF preview tab will close. Are you sure?"),MB_YESNO) != IDYES)
		return;

	m_bOpenDefaultPDFViewer = true;
	pChild->DestroyWindow();
	
}
