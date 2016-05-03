#pragma once

const int stdZoomVals[] =
{
  10,
  25,
  50,
  75,
  100,
  125,
  150,
  200,
  400,
  10000,//Fit Page
  10001,//Fit Width
  10002 //Fit Height
};

#define ZOOM_FIT_PAGE  10000
#define ZOOM_FIT_WIDTH  10001
#define ZOOM_FIT_HEIGHT 10002

class CPDFView : public CScrollView
{
public:
	CPDFView();
	DECLARE_DYNCREATE(CPDFView)
  
protected:
	virtual ~CPDFView();

public:
  CString      _strFile;

  BOOL			m_bIsChanged;
  BOOL			m_bOpenDefaultPDFViewer;

protected:
  DebenuPDFLibraryAX0912 _qpdf;
  int         _pdfId;
  int         _pageRef;
  int         _pageNum;
  int         _pageCount;

  BITMAPINFO  _bi;
  BYTE*       _imageBuf;
  HBITMAP     _hbmp;
  int         _imageWidth;
  int         _imageHeight;
  double      _zoom;
  int		  m_nZoom;
  int         _dpi;

public:
  void InitView();
  void RenderPage();
  void PrevPage();
  void NextPage();
  void GoToPage(int page);
  int  GetZoom();
  void SetZoom(int zoom);

protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void OnDraw(CDC* pDC);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnInitialUpdate();
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnUpdateAlwaysEnabledButton(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAlwaysDisabledButton(CCmdUI* pCmdUI);
  afx_msg void OnSavePdf();
  afx_msg void OnSaveAs();
  afx_msg void OnExportPdf();
  afx_msg void OnDocProperties();
  afx_msg void OnZoomChange();
  afx_msg void OnZoomOut();
  afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
  afx_msg void OnZoomIn();
  afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
  afx_msg void OnPagePrev();
  afx_msg void OnUpdatePagePrev(CCmdUI* pCmdUI);
  afx_msg void OnPageGo();
  afx_msg void OnPagePages();
  afx_msg void OnPageNext();
  afx_msg void OnUpdatePageNext(CCmdUI* pCmdUI);
  afx_msg void OnPageHome();
  afx_msg void OnPageEnd();
  afx_msg void OnOpenDefaultPDFViewer();
public:
	afx_msg void OnClose();
};
